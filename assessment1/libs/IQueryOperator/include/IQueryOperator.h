//
// Created by jorda on 10/26/2025.
//

#ifndef IQUERYOPERATOR_H
#define IQUERYOPERATOR_H

// IQueryOperator.h
#include <memory>
#include <vector>

// Represents a single node on the AST (Abstract Syntax Tree). There a 3 sitautions.
// Evaluating a node, evaluating AND and evaluating OR.
class IQueryOperator {
public:
    virtual ~IQueryOperator() = default;
    virtual std::unique_ptr<IQueryOperator> evaluate () const = 0;
};


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




#endif //IQUERYOPERATOR_H
