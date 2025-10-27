//
// Created by jorda on 10/26/2025.
//

#ifndef LEAFNODE_H
#define LEAFNODE_H
#include "IQueryOperator.h"
#include "QueryFileStructure.h"

#include "json.hpp"

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

        // DUMMY CONSTRUCTOR TO MAKE COMPILER HAPPY DO NOT USE.
        explicit LeafNode();

        std::unique_ptr<IQueryOperator> evaluate() const override;

        // Getter for retrieving cropParams
        const CropQueryParameters& getCropParams() const;
};



#endif //LEAFNODE_H
