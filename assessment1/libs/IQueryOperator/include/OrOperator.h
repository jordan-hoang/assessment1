#ifndef OROPERATOR_H
#define OROPERATOR_H

#include "AndOperator.h" // Includes LogicalOperator base class

/**
 * @brief Implements the logical OR operation, inheriting the children list
 * from LogicalOperator. All of it's children will eventually at somepoint be ORed.
 * And all of its children when evaaluated will be a leafNode.
 *
 */
class OrOperator : public LogicalOperator {
private:
    std::unique_ptr<IQueryOperator> executeOR(std::unique_ptr<IQueryOperator> accumulator,
                                               std::unique_ptr<IQueryOperator> nodeToCombine) const;

public:
    OrOperator() = default;
    std::unique_ptr<IQueryOperator> evaluate() const override;
};

#endif //OROPERATOR_H
