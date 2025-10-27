#ifndef OROPERATOR_H
#define OROPERATOR_H

#include "InspectionGroup.h" // ADDED: Definition of InspectionGroup and InspectionGroupCollection
#include "AndOperator.h" // Includes LogicalOperator base class
#include <set>           // Required for efficient union operation (guaranteeing unique results)
#include <algorithm>
#include <iostream>

/**
 * @brief Implements the logical OR operation, inheriting the children list
 * from LogicalOperator.
 * Applies all child filters independently to the original input data
 * and then takes the union of all resulting filtered sets.
 */
class OrOperator : public LogicalOperator {

    private:
    /**
     * It will be a leafNdoe because if it a leafNode ISN"T RETURNED THAN EVALUATE WAS CALLED ON AN OR
     * or it was called on an AND which will recursively go down / simplify / reduce until you get a leaf node.
     * @param accumulator This is the accumulator the one that combines all the and stuff.
     * @param nodeToCombine This is a "leafNode  or children you will combine with."
     * @return The combined nodes.
     */
    std::unique_ptr<IQueryOperator> executeOR(std::unique_ptr<IQueryOperator> accumulator,
                                               std::unique_ptr<IQueryOperator> nodeToCombine) const{

        // auto leafA = dynamic_cast<LeafNode*>(accumulator.get());
        // auto leafB = dynamic_cast<LeafNode*>(nodeToCombine.get());


        /// THIS IS BROKEN FOR NOW...... OR UNIMPLEMENTED
        return std::make_unique<LeafNode>();
    }
public:
    OrOperator() = default;

    /**
     * @brief Executes the OR logic (Union).
     * Each child is evaluated against the full input data, and the results
     * are merged into a single unique collection.
     */
    std::unique_ptr<IQueryOperator> evaluate() const override {
        if (children_.empty()) {
            std::cout << "OR Operator called with no children. This is unexpected terminating program..." << std::endl;
            exit(-1);
        }

        std::cout << "--- Starting OR Operator Evaluation (" << children_.size() << " children) ---" << std::endl;
        auto accumulator = children_[0]->evaluate(); // We reduce like compilersclass.
        for (size_t i = 1; i < children_.size(); i++) {
            accumulator = executeOR(std::move(accumulator), children_[i]->evaluate());
        }

        return accumulator;
    }
};

#endif //OROPERATOR_H
