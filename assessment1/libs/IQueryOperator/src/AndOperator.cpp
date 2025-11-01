#include "AndOperator.h"
#include <LeafNode.h>
#include <iostream>

/**
 * It will be a leafNdoe because if it a leafNode ISN"T RETURNED THAN EVALUATE WAS CALLED ON AN OR
 * or it was called on an AND which will recursively go down / simplify / reduce until you get a leaf node.
 * @param accumulator This is the accumulator the one that combines all the and stuff.
 * @param nodeToCombine This is a "leafNode  or children you will combine with."
 * @return The combined nodes.
 */
std::unique_ptr<LeafNode> AndOperator::executeAND(std::unique_ptr<LeafNode> accumulator,
                                           std::unique_ptr<LeafNode> nodeToCombine) const{

    auto leafA = std::move(accumulator);
    auto leafB = std::move(nodeToCombine);

    // We need to combine these leafNodes by creating a new one.
    CropQueryParameters crop_query;
    ///////////// REQUIRED ///////////////
    crop_query.list_region = Region::intersectRegions(leafA->getCropParams().list_region, leafB->getCropParams().list_region);



    ///////////////////////////////////////
    ///
    /// 3 optionals.
    auto catASet = leafA->getCropParams().list_category;
    auto catBSet = leafB->getCropParams().list_category;
    for(const auto &a : catASet) {
        if(catBSet.find(a) != catBSet.end()) {
            crop_query.list_category.insert(a);
        }
    }


    // Do proper.
    bool properA = leafA->getCropParams().proper.value_or(false);  // If missing value make it false.
    bool properB = leafB->getCropParams().proper.value_or(false);
    crop_query.proper = (properA == properB) ? properA : false;


    // Take whats in common from both of the sets and create a new set of whats in common between them.
    const auto& groupsA = leafA->getCropParams().one_of_groups.value_or(std::set<int64_t>{});
    const auto& groupsB = leafB->getCropParams().one_of_groups.value_or(std::set<int64_t>{});
    std::set<std::int64_t> combined;
    std::set_intersection(
        groupsA.begin(), groupsA.end(),
        groupsB.begin(), groupsB.end(),
        std::inserter(combined, combined.begin())
    );

    crop_query.one_of_groups = combined;
    return std::make_unique<LeafNode>(crop_query);
}


std::unique_ptr<LeafNode> AndOperator::evaluate() const {
    if (children_.empty()) {
        std::cout << "OR Operator called with no children. This is unexpected terminating program..." << std::endl;
        exit(-1);
    }

    std::cout << "Evaluating AND with " << children_.size() << " children" << std::endl;
    auto accumulator = children_[0]->evaluate(); // We reduce like compiler class... this results in some values being lost?
    for(unsigned int i = 1; i < children_.size(); i++) {
        accumulator = executeAND(std::move(accumulator), children_[i]->evaluate());
    }
    return accumulator;
}