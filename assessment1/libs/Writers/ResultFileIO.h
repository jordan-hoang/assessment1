//
// Created by jorda on 10/26/2025.
//

#ifndef RESULTWRITER_H
#define RESULTWRITER_H

#include <iostream>
#include <vector>
#include <string>
#include "InspectionRegion.h"
#include <nlohmann_json/json.hpp>

/**
 * A function that writes to a text file. Maybe making it a seperate file is overkill but I guess it will be needed for solution #3 as well.
 */
namespace ResultFileIO {
    // Helper function that writes ascending y,x records to a text file.
    // The file nam
    void writeToTextFile(
        const std::vector<InspectionRegion>& records,
        const std::string& outputFile = "../output.txt"
    );


    /**
     * Reads a JSON file from path and returns the parsed object.
     * @param queryFilePath The path to the JSON file.
     * @return The parsed json object, or an empty object on failure.
     */
    nlohmann::json readJsonFile(const std::string& queryFilePath);


}


#endif //RESULTWRITER_H
