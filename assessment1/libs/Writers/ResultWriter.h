//
// Created by jorda on 10/26/2025.
//

#ifndef RESULTWRITER_H
#define RESULTWRITER_H

#include <vector>
#include <string>
#include "InspectionRegion.h"

/**
 * A function that writes to a text file. Maybe making it a seperate file is overkill but I guess it will be needed for solution #3 as well.
 */
namespace ResultWriter {
    // Helper function that writes ascending y,x records to a text file.
    // The file nam
    void writeToTextFile(
        const std::vector<InspectionRegion>& records,
        const std::string& outputFile = "../output.txt"
    );
}


#endif //RESULTWRITER_H
