#include <fstream>
#include <iostream>
#include <cstdint>

#include <boost/program_options.hpp>
#include "json.hpp"

using json = nlohmann::json;
namespace po = boost::program_options;



int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help message")
        ("query,q", po::value<std::string>(), "Path to json you want to parse")
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
    if (vm.count("query")) {
        std::string path = vm["query"].as<std::string>();
        std::cout << "path entered: " << path << std::endl;

    }
    else {
        std::cout << "No arguments entered" << std::endl;
    }

    return 0;
}
