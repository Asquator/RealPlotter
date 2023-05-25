
#include <memory>
#include <ostream>

#include "sum_function.h"

using std::shared_ptr;

namespace RealFunctionLib_impl{

SumFunction::SumFunction(shared_ptr<RealFunctionBase> left, shared_ptr<RealFunctionBase> right):
    BinaryOperationFunction(left, right, std::plus<real_type>()) {}


void SumFunction::print(std::ostream &os) const{
    os << *getLeftOperand() << " + " << *getRightOperand();
}


std::shared_ptr<RealFunctionBase>SumFunction::calculateDerivative() const {
    return std::make_shared<SumFunction>(getLeftOperand()->getDerivative(), getRightOperand()->getDerivative());
}

}
