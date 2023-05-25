#include <functional>
#include <memory>
#include <iostream>

#include "binary_operation_function.h"

using std::shared_ptr;

namespace RealFunctionLib_impl{


BinaryOperationFunction::BinaryOperationFunction(shared_ptr<RealFunctionBase> left,
                                                shared_ptr<RealFunctionBase> right,
                                                const operation_type &op):
    leftOperand{std::move(left)}, rightOperand{std::move(right)}, operation{op} {}

real_type BinaryOperationFunction::operator()(real_type x) const {
    return operation((*leftOperand)(x), (*rightOperand)(x));
}

bool BinaryOperationFunction::isDefined(real_type x) const {
    return leftOperand->isDefined(x) &&
    rightOperand->isDefined(x);
}




}
