#include <cmath>
#include <functional>
#include <memory>
#include <ostream>

#include "binary_operation_function.h"
#include "composite_function.h"
#include "power_function.h"
#include "product_function.h"
#include "real_function_base.h"
#include "special_function.h"

#include "real_math.h"


using std::ostream;
using std::shared_ptr;

namespace RealFunctionLib_impl {


PowerFunction::PowerFunction(shared_ptr<RealFunctionBase> left, shared_ptr<RealFunctionBase> right):
    BinaryOperationFunction(std::move(left), std::move(right), [](real_type x, real_type y){return std::pow(x, y);}) {}


void PowerFunction::print(ostream &os) const{
    os << "(" << *getLeftOperand() << ") ^ (" << *getRightOperand() << ")";  
}


bool PowerFunction::isDefined(real_type x) const {
    return BinaryOperationFunction::isDefined(x) && (std::floor(x) == x || x > 0);
}


/*
 * Using the rule to calculate a derivative of Power to construct the derivative
*/
std::shared_ptr<RealFunctionBase> PowerFunction::calculateDerivative() const {

    shared_ptr<RealFunctionBase> exp = SpecialFunction::getSpecial(SpecialFunction::Specials::Exp);
    shared_ptr<RealFunctionBase> baseLn = std::make_shared<CompositeFunction>(SpecialFunction::getSpecial(SpecialFunction::Specials::Ln), getLeftOperand());
    shared_ptr<RealFunctionBase> expProduct = std::make_shared<ProductFunction>(baseLn, getRightOperand());

    return CompositeFunction(exp, expProduct).getDerivative();
}

}
