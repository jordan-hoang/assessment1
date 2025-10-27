#ifndef OROPERATOR_H
#define OROPERATOR_H

#include "InspectionRegion.h" // ADDED: Definition of InspectionGroup and InspectionGroupCollection
#include "AndOperator.h" // Includes LogicalOperator base class
#include <set>           // Required for efficient union operation (guaranteeing unique results)
#include <algorithm>
#include <iostream>

/**
 * @brief Implements the logical OR operation, inheriting the children list
 * from LogicalOperator. All of it's children will eventually at somepoint be ORed.
 * And all of its children when evaaluated will be a leafNode.
 *
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


        auto leafA = dynamic_cast<LeafNode*>(accumulator.get());
        auto leafB = dynamic_cast<LeafNode*>(nodeToCombine.get());

        // We need to combine these leafNodes by creating a new one.
        CropQueryParameters crop_query;

        ///////////// REQUIRED ///////////////
        crop_query.region = Region::unionRegions(leafA->getCropParams().region, leafB->getCropParams().region);
        ///////////////////////////////////////
        ///
        /// 3 optionals.

        // --- CATEGORY: take any present value we can't put them in a list unfortunatley. ---
        auto catA = leafA->getCropParams().category;
        auto catB = leafB->getCropParams().category;
        if (catA.has_value()) {
            crop_query.category = catA;
        } else if (catB.has_value()) {
            crop_query.category = catB;
        } else {
            crop_query.category.reset();
        }

        // Do proper.
        bool properA = leafA->getCropParams().proper.value_or(false);  // If missing value make it false.
        bool properB = leafB->getCropParams().proper.value_or(false);
        crop_query.proper = (properA || properB);


        // Take whats in common from both of the sets and create a new set of whats in common between them.
        const auto& groupsA = leafA->getCropParams().one_of_groups.value_or(std::set<int64_t>{});
        const auto& groupsB = leafB->getCropParams().one_of_groups.value_or(std::set<int64_t>{});
        std::set<std::int64_t> combined;
        std::set_union(
            groupsA.begin(), groupsA.end(),
            groupsB.begin(), groupsB.end(),
            std::inserter(combined, combined.begin())
        );

        crop_query.one_of_groups = combined;

        return std::make_unique<LeafNode>(crop_query);

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
