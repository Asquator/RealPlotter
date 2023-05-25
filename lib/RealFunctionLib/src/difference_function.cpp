
#include <memory>
#include <ostream>

#include "binary_operation_function.h"
#include "difference_function.h"

using std::shared_ptr;

namespace RealFunctionLib_impl{

DifferenceFunction::DifferenceFunction(shared_ptr<RealFunctionBase> left, shared_ptr<RealFunctionBase> right):
    BinaryOperationFunction(std::move(left), std::move(right), std::plus<real_type>()) {}



void DifferenceFunction::print(std::ostream &os) const{
    os << *getLeftOperand() << " - " << *getRightOperand();
}


std::shared_ptr<RealFunctionBase>DifferenceFunction::calculateDerivative() const {
    return std::make_shared<DifferenceFunction>(getLeftOperand()->getDerivative(), getRightOperand()->getDerivative());
}

}
