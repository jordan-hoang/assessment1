#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;


int main(int argc, char* argv[])
{

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help message")
        ("name,n", po::value<std::string>(), "Your name");


    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // 3. Handle options
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if (vm.count("name")) {
        std::cout << "Hello, " << vm["name"].as<std::string>() << "!" << std::endl;
    } else {
        std::cout << "Hello, world!!!" << std::endl;
    }



    return 0;
}
