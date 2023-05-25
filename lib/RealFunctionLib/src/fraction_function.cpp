#include <functional>
#include <memory>
#include <ostream>

#include "difference_function.h"
#include "fraction_function.h"
#include "product_function.h"
#include "real_math.h"

using std::ostream;
using std::shared_ptr;
using namespace Real_Math;

namespace RealFunctionLib_impl {


FractionFunction::FractionFunction(shared_ptr<RealFunctionBase> left, shared_ptr<RealFunctionBase> right):
    BinaryOperationFunction(std::move(left), std::move(right), std::divides<real_type>()) {}



void FractionFunction::print(ostream &os) const{
    os << "(" << *getLeftOperand() << ") / (" << *getRightOperand() << ")";  
}


bool FractionFunction::isDefined(real_type x) const {
    return BinaryOperationFunction::isDefined(x) && !round_compare(x, 0);
}


/*
 * Using the rule to calculate a derivative of fraction to construct the derivative
*/
std::shared_ptr<RealFunctionBase>FractionFunction::calculateDerivative() const {

    shared_ptr<RealFunctionBase> prod1 = std::make_shared<ProductFunction>(getLeftOperand()->getDerivative(), getRightOperand());
    shared_ptr<RealFunctionBase> prod2 = std::make_shared<ProductFunction>(getLeftOperand(), getRightOperand()->getDerivative());

    shared_ptr<RealFunctionBase> numerator = std::make_shared<DifferenceFunction>(std::move(prod1), std::move(prod2));
    shared_ptr<RealFunctionBase> denominator = std::make_shared<ProductFunction>(getRightOperand(), getRightOperand());

    return std::unique_ptr<RealFunctionBase>{new FractionFunction{std::move(numerator), std::move(denominator)}};
}

}
