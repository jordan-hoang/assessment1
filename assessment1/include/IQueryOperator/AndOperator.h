#ifndef ANDOPERATOR_H
#define ANDOPERATOR_H

#include "IQueryOperator.h"
#include <vector>
#include <memory>
#include <iostream>

/**
 * @brief Base class for Composite operators (AND, OR).
 * Manages the collection of child IQueryOperator nodes.
 */
class LogicalOperator : public IQueryOperator {
protected:
    // Stores the list of child operators (Composite or Leaf).
    std::vector<std::unique_ptr<IQueryOperator>> children_;

public:
    // Allows the QueryBuilder to add parsed child nodes.
    void add_child(std::unique_ptr<IQueryOperator> op) {
        children_.push_back(std::move(op));
    }

    // The pure virtual evaluate method is inherited from IQueryOperator.
    // Virtual destructor is necessary for proper cleanup of the composite tree.
    virtual ~LogicalOperator() = default;
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
                                                   std::unique_ptr<IQueryOperator> nodeToCombine) const{



            std::cout << "WOWOWOOW";

            exit(-1);
            // auto leafA = dynamic_cast<LeafNode*>(accumulator.get());
            // auto leafB = dynamic_cast<LeafNode*>(nodeToCombine.get());


            /// THIS IS UNIMPLEMENTED FOR NOW
            return std::make_unique<LeafNode>();
        }

    public:
        AndOperator() = default;

        std::unique_ptr<IQueryOperator> evaluate() const override {

            if (children_.empty()) {
                std::cout << "OR Operator called with no children. This is unexpected terminating program..." << std::endl;
                exit(-1);
            }

            std::cout << "Evaluating AND with " << children_.size() << " children" << std::endl;
            auto accumulator = children_[0]->evaluate(); // We reduce like compioler class.
            for(unsigned int i = 1; i < children_.size(); i++) {
                accumulator = executeAND(std::move(accumulator), children_[i]->evaluate());
            }

            return accumulator;
        }



};

#endif //ANDOPERATOR_H
