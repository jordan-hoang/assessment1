//
// Created by jorda on 10/26/2025.
//

#ifndef IQUERYOPERATOR_H
#define IQUERYOPERATOR_H

// IQueryOperator.h
#include <memory>

class IQueryOperator {
public:
    virtual ~IQueryOperator() = default;
    virtual std::unique_ptr<IQueryOperator> evaluate () const = 0;
};

#endif //IQUERYOPERATOR_H
