#include <fstream>
#include <iostream>
#include <cstdint>
#include <QueryFileJsonParser.h>
#include <solution2_core.h>
#include <boost/program_options.hpp>
#include <pqxx/pqxx>
#include <nlohmann_json/json.hpp>


#include "QueryFileStructure.h"
#include "InspectionRegion.h"
#include "InspectionGroupFilter.h"
#include "ResultFileIO.h"

using json = nlohmann::json;
namespace po = boost::program_options;



/**
 *
 * @param query_struct - The query struct you are going to filter list_records by.
 * @param list_records - List of records.
 * @return A filtered list of list_records.
 */
std::vector<InspectionRegion> filterWithQueryStruct(const QueryFileStructure &query_struct, const std::vector<InspectionRegion> &list_records, bool debug = false) {

    /* For debugging */

    if(debug) {
        QueryFileStructure::dumpQueryStruct(query_struct); // For debugging can remove later... not needed
        std::cout << std::endl;
        std::cout << std::endl;
        for(InspectionRegion a : list_records) {
            std::cout << a.toString() << std::endl;
        }
    }

    std::vector<InspectionRegion> filtered_records = InspectionGroupFilter::applyFilter(query_struct, list_records);

    // More debugging dumps
    if(debug) {
        std::cout << "\nDumping out filtered records" << std::endl;
        for(InspectionRegion a : filtered_records) {
            std::cout << a.toString() << std::endl;
        }
    }


    return filtered_records;
}




int main(int argc, char* argv[])
{
    solution2_core::parseArguments(argc, argv);
    return 0;
}
