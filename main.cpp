#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>
#include <algorithm>
#include <iomanip>

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


// Writes to the database using lib.
void writeToDB(const std::vector<Record>& records) {
    /// Write to db here.


    return;
}



int main(int argc, char* argv[])
{

    std::vector<Record> myRecord = parseFile("asdf");




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
