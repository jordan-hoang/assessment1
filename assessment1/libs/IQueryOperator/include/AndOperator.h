#ifndef ANDOPERATOR_H
#define ANDOPERATOR_H

#include <vector>
#include <memory>
#include <iostream>
#include "IQueryOperator.h"
#include "QueryFileStructure.h"
#include "LeafNode.h"

/**
 * @brief Base class for Composite operators (AND, OR).
 * Manages the collection of child IQueryOperator nodes.
 */
class LogicalOperator : public IQueryOperator {
protected:
    // Stores the list of child nodes. Think a tree that can have as many children as you want.
    std::vector<std::unique_ptr<IQueryOperator>> children_;
public:
    // Allows the QueryBuilder to add parsed child nodes.
    void add_child(std::unique_ptr<IQueryOperator> op) {
        children_.push_back(std::move(op));
    }

    ~LogicalOperator() override = default;
};


/**
 * @brief Implements the logical AND operation, inheriting the children list
 * from LogicalOperator.
 * Filters data sequentially: the result of one child is the input to the next.
 */
class AndOperator : public LogicalOperator {
    private:
        /**
         * It will be a leafNdoe because if it a leafNode ISN"T RETURNED THAN EVALUATE WAS CALLED ON AN OR
         * or it was called on an AND which will recursively go down / simplify / reduce until you get a leaf node.
         * @param accumulator This is the accumulator the one that combines all the and stuff.
         * @param nodeToCombine This is a "leafNode  or children you will combine with."
         * @return The combined nodes.
         */
        std::unique_ptr<IQueryOperator> executeAND(std::unique_ptr<IQueryOperator> accumulator,
                                                   std::unique_ptr<IQueryOperator> nodeToCombine) const;

    public:
        AndOperator() = default;
        std::unique_ptr<IQueryOperator> evaluate() const override;

};

#endif //ANDOPERATOR_H
