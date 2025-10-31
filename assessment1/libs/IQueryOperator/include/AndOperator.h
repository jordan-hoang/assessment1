#ifndef ANDOPERATOR_H
#define ANDOPERATOR_H

#include <LeafNode.h>
#include <memory>
#include "IQueryOperator.h"


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
        std::unique_ptr<LeafNode> executeAND(std::unique_ptr<LeafNode> accumulator,
                                                   std::unique_ptr<LeafNode> nodeToCombine) const;

    public:
        AndOperator() = default;
        std::unique_ptr<LeafNode> evaluate() const override;

};

#endif //ANDOPERATOR_H
