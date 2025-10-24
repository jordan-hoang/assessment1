#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include <pqxx/pqxx>
#include <boost/program_options.hpp>

#include "Record.h"


namespace po = boost::program_options;

/**
 *
 * @param fileName Should go to the parent so it should be ...../assessment1/data/0"
 * categories.txt, groups.txt and points.txt should be inside.
 */
std::vector<Record> parseFile(std::string filePath) {

    filePath = "C:/Users/jorda/CLionProjects/assessment/assessment1/data/0"; // For faster testing, REMOVE when done.
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
    }

    std::vector<Record> list_records;

    double x, y, group, category;
    // Read files line by line
    while (files[0] >> x >> y && files[1] >> group && files[2] >> category) {
        Record r(x, y);
        r.set_group(group);
        r.set_category(category);
        list_records.push_back(r);
    }

    return list_records;

}

// Function to find the maximum ID and return the next available ID
long long get_next_available_id(pqxx::connection& C, const std::string& table_name) {
    try {
        pqxx::nontransaction N(C);
        std::string query = "SELECT MAX(id) FROM " + table_name;
        pqxx::result R = N.exec(query);

        if (R[0][0].is_null()) {
            // If the table is empty, MAX(id) returns NULL. Start counter at 0.
            return 0;
        } else {
            // Table has data. Get the max value and increment by 1.
            long long max_id = R[0][0].as<long long>();
            return max_id + 1;
        }
    } catch (const pqxx::sql_error& e) {
        // Handle case where table might not exist yet (though we assume it does).
        std::cerr << "SQL Error finding max ID for " << table_name << ": " << e.what() << std::endl;
        throw; // Re-throw to halt execution if a critical error occurs
    }

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

    try {
        // 2. Establish the connection
        pqxx::connection C(connection_string);
        std::cout << "Connected to database " << C.dbname() << " successfully!" << std::endl;

        pqxx::work W(C);
        std::set<long long> unique_groups;
        for (const auto& record : records) {
            unique_groups.insert(record.get_group());
        }

        // Insertion_group table insert.
        for (long long group_id : unique_groups) {
            W.exec_params(
                "INSERT INTO inspection_group (id) VALUES ($1) ON CONFLICT (id) DO NOTHING",
                group_id
            );
        }

        W.commit();

        // Now we need to do inspection_region.
        /**
         *
            *id       | bigint           |           | not null |
             group_id | bigint           |           |          |
             coord_x  | double precision |           |          |
             coord_y  | double precision |           |          |
             category | integer          |           |          |
         *
         *
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

    std::vector<Record> myRecord = parseFile("asdf");
    writeToDB(myRecord);



    /*
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

    // 3. Handle options
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("data_directory")) {
        std::string path = vm["data_directory"].as<std::string>();
        std::cout << "path entered: " << path << std::endl;

        parseFile(path);

    }
    else {
        std::cout << "No arguments entered" << std::endl;
    }
    */

    return 0;
}
