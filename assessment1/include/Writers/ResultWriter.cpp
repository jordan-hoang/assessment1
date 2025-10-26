#include "ResultWriter.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

// ... other necessary includes

namespace ResultWriter {

    void writeToTextFile(const std::vector<InspectionGroup>& records,
                         const std::string& filename
    ) {

        std::cout << "Writing results to 'solution folder" << std::endl;

        // Takes in a bunch of records and writes it to a file.
        // We'll need to sort it by y and x, ascending I guess. not too hard.
        // I guess we'll copy over the vector, sort it and then write it.
        std::vector<InspectionGroup> sorted_list = records; // copy it.
        std::sort(sorted_list.begin(),sorted_list.end(),
            [](const InspectionGroup &a, const InspectionGroup &b) {
                    // return true if a comes before b. A is closer to the beginning.
                    if(a.get_y_coordinate() == b.get_y_coordinate()) { // y is the same use x.
                        return a.get_x_coordinate() < b.get_x_coordinate();
                    }
                    return a.get_y_coordinate() < b.get_y_coordinate();
                }
        );

        /// Write to the text file now.
        std::ofstream output_file(filename);

        constexpr int precision = std::numeric_limits<double>::max_digits10;
        output_file << std::fixed << std::setprecision(precision);


        if(!output_file.is_open()) {
            std::cerr << "Error opening output file, and writing to it." << filename << std::endl;
        }

        for(const auto &a : sorted_list) {
            output_file << "(" << a.get_y_coordinate() << "," << a.get_x_coordinate() << ")" << std::endl;
        }

        output_file.close();

    }

}