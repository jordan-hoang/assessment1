//
// Created by jorda on 10/26/2025.
//

#ifndef LEAFNODE_H
#define LEAFNODE_H
#include <nlohmann_json/json.hpp>
#include "IQueryOperator.h"
#include "QueryFileStructure.h"


/// Contains an instance of "OperatorCrop" based on the example JSON.
/// This is the smallest item think of a binary tree used for evaluating 6 + 4
/// or 8 * 3. But do that for the nodes. instead.
class LeafNode : public IQueryOperator {
    private:
        //Stores all the parsed parameters for the 'operator_crop' operation.
        CropQueryParameters cropParams;  // this would be operator_crop in the json file, everything thats inside that.

    public:
        // @brief Constructs a LeafNode by parsing its specific parameters from a JSON object.
        // @param crop_json_data The JSON object associated with "operator_crop".
        explicit LeafNode(const nlohmann::json& crop_json_data);

        explicit LeafNode(CropQueryParameters p);

        // DUMMY CONSTRUCTOR TO MAKE COMPILER HAPPY DO NOT USE / DELETE LATER NOT NECESSARY.
        explicit LeafNode() {
            throw std::logic_error("Dummy LeafNode constructor called — shouldn’t happen!");
        };

        std::unique_ptr<LeafNode> evaluate() const override;

        // Getter for retrieving cropParams
        CropQueryParameters getCropParams() const;
};



#endif //LEAFNODE_H
