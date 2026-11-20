//
// Database access for InspectionRegion. Only compiled when libpqxx is found
// (see libs/InspectionRegion/CMakeLists.txt).
//

#include "InspectionRegion.h"

#include <pqxx/pqxx>
#include "ThrowWithContext.hpp"


/**
* @param row A pqxx::row object containing the fetched data, and turning it to a record.
* @return A fully populated Record object.
*/
InspectionRegion InspectionRegion::map_row_to_record(const pqxx::row& row) {
    // Fetch values and convert using the type-safe 'as<T>()' method
    auto x = row["coord_x"].as<double>();
    auto y = row["coord_y"].as<double>();
    auto category_val = row["category"].as<std::int64_t>();
    auto group_val = row["group_id"].as<std::int64_t>();

    return InspectionRegion(x, y, category_val, group_val);
}


/**
 * @return vector of records from the inspection_region Database table.
 */
std::vector<InspectionRegion> InspectionRegion::readRecordsFromDB(const std::string& conn_string) {

    std::vector<InspectionRegion> list_records;
    try
    {
        pqxx::connection myconnection(conn_string);
        pqxx::work W(myconnection);

        pqxx::result inspection_region_result = W.exec("SELECT * From inspection_region;");
        for (const pqxx::row& row : inspection_region_result) {
            list_records.push_back(InspectionRegion::map_row_to_record(row));
        }
    } catch (std::exception& e) {
        ThrowWithContext::throw_with_context(e.what(),__FILE__, __LINE__ );
    }

    return list_records;
}
