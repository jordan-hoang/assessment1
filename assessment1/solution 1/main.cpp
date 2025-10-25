#include <fstream>
#include <iostream>
#include <cstdint>

#include <pqxx/pqxx>
#include <boost/program_options.hpp>

#include "Record.h"


namespace po = boost::program_options;

/**
 * @param filePath Should go to the parent so it should be <somepath>/assessment1/data/0"
 * it will then search inside and look for these 3 files:
 * categories.txt, groups.txt and points.txt should be inside.
 * @param isGood - True if everything was good false if something went wrong.
 */
std::vector<Record> parseFile(std::string filePath, bool &isGood) {

    std::cout << "using path of : " << filePath << std::endl;
    if (!filePath.empty() && filePath.back() != '/') {
        filePath += '/';
    }

    std::vector<std::string> fileNames = {
        filePath + "points.txt",
        filePath + "groups.txt",
        filePath + "categories.txt"
    };

    std::vector<std::ifstream> files;
    for (const auto& f : fileNames) {
        files.emplace_back(f);
        if (!files.back().is_open()) {
            isGood = false;
            return {}; // Invalid file, set false and return empty vector.
        }

    }

    std::vector<Record> list_records;
    // Read files line by line
    std::string line_points, line_groups, line_categories; // Variables to hold lines as strings
    // 1. Use std::getline to read the ENTIRE line of each file as a string
    while (std::getline(files[0], line_points) &&
           std::getline(files[1], line_groups) &&
           std::getline(files[2], line_categories)) {
        try {
            std::stringstream ss_points(line_points);
            double x, y;
            ss_points >> x >> y;

            double group_double = std::stod(line_groups);
            std::int64_t group_id = static_cast<std::int64_t>(group_double);

            double category_double = std::stod(line_categories);
            std::int64_t category_id = static_cast<std::int64_t>(category_double);

            list_records.emplace_back(x, y, category_id, group_id);

        } catch (const std::exception& e) {
            isGood = false;
            return {};
        }
    }

    for (auto& file : files) {
        if (file.is_open()) {
            file.close();
        }
    }

    return list_records;

}

// Function to find the maximum ID and return the next available ID, not sure if needed....
long long get_next_available_id(pqxx::connection& C, const std::string& table_name) {
    try {
        pqxx::nontransaction N(C);
        std::string query = "SELECT MAX(id) FROM " + table_name;
        pqxx::result R = N.exec(query);

        if (R[0][0].is_null()) {
            return 0;
        } else {
            long long max_id = R[0][0].as<long long>();
            return max_id + 1;
        }
    } catch (const pqxx::sql_error& e) {
        // Handle case where table might not exist yet (though we assume it does).
        std::cerr << "SQL Error finding max ID for " << table_name << ": " << e.what() << std::endl;
        throw;
    }

}


// Creates the tables
// If you manually created the tables then you can just comment this out.
int createDB(const std::string& connection_string) {

    try {
        // 2. Establish the connection
        pqxx::connection C(connection_string);
        pqxx::work W(C);

        // 2️⃣ Read the schema file into a string
        std::ifstream file("assessment1/schema.pgsql");

        if (!file.is_open()) {
            std::cerr << "Could not open schema.pgsql\n";
            return 1;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string sql = buffer.str();

        W.exec(sql);
        W.commit();
    }
    catch (const std::exception &e) {
        // 6. Handle errors
        std::cerr << "Database connection or query failed: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}


// Writes to the database using lib.
int writeToDB(const std::vector<Record>& records) {
    /// Write to db here.
    std::string connection_string =
            "host=localhost "
            "port=5432 "      // Standard PostgreSQL port
            "user=postgres "
            "password=Moonshine4me " /// REPLACE WITH YOUR PASSWORD!!!
            "dbname=postgres ";      /// Generic default database.

    if(createDB(connection_string)) {
        std::cerr << "Creating database failed. Check connection string, and path of schema.pgsql. If you manually created and don't wanna deal with path issues you can just comment this out." << std::endl;
        return -1;
    }

    try {
        // 2. Establish the connection
        pqxx::connection C(connection_string);
        std::cout << "Connected to database " << C.dbname() << " successfully!" << std::endl;
        pqxx::work W(C);

        ///// Clear any prexisting values in database tables first, just for faster debugging.
        W.exec_params("TRUNCATE TABLE inspection_group;");
        W.exec_params("TRUNCATE TABLE inspection_region;");
        W.commit();

        std::set<std::int64_t> unique_groups;
        for (const auto& record : records) {
            auto group_id = static_cast<std::int64_t>(record.get_group());
            unique_groups.insert(group_id);
        }

        for (std::int64_t group_id : unique_groups) {
            W.exec_params(
                "INSERT INTO inspection_group (id) VALUES ($1) ON CONFLICT (id) DO NOTHING",
                pqxx::to_string(group_id)
            );
        }

        W.commit();

        /** Now we need to do inspection_region.
         *
            *id       | bigint           |           | not null |
             group_id | bigint           |           |          |
             coord_x  | double precision |           |          |
             coord_y  | double precision |           |          |
             category | integer          |           |          |
         */
         long long region_id_counter = get_next_available_id(C, "inspection_region");
         for (const auto& record : records) {
             W.exec_params(
                 "INSERT INTO inspection_region (id, group_id, coord_x, coord_y, category) "
                 "VALUES ($1, $2, $3, $4, $5)",
                 region_id_counter++,
                 record.get_group(),
                 record.get_x_coordinate(),
                 record.get_y_coordinate(),
                 record.get_category()
             );
         }
         W.commit();

    }
    catch (const std::exception &e) {
        // 6. Handle errors
        std::cerr << "Database connection or query failed: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}



int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help message")
        ("data_directory,p", po::value<std::string>(), "Path to data directory you want to parse")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    // Handle the main parse.
    if (vm.count("data_directory")) {
        std::string path = vm["data_directory"].as<std::string>();
        std::cout << "path entered: " << path << std::endl;

        bool isGood = true;
        std::vector<Record> myRecord = parseFile(path, isGood);
        if(!isGood) {
            std::cerr << "Could not parse files inside of: " << path << std::endl;
            return -1;
        }
        writeToDB(myRecord);
    }
    else {
        std::cout << "No arguments entered" << std::endl;
    }

    return 0;
}
