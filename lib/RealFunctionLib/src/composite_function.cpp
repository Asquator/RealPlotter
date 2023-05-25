#include <ctime>
#include <functional>
#include <memory>
#include <ostream>

#include "composite_function.h"
#include "product_function.h"
#include "real_function.h"
#include "real_math.h"

using std::shared_ptr;
using std::ostream;

namespace RealFunctionLib_impl{


CompositeFunction::CompositeFunction(shared_ptr<RealFunctionBase> left, shared_ptr<RealFunctionBase> right):
    outer(std::move(left)), inner(std::move(right)) {}



real_type CompositeFunction::operator()(real_type x) const{
    return outer->operator()(inner->operator()(x));
}

void CompositeFunction::print(ostream &os) const{
    os << "(" << *outer << ") composite (" << *inner << ")";  
}

bool CompositeFunction::isDefined(real_type x) const {
    return inner->isDefined(x) && outer->isDefined(inner->operator()(x));
}

std::shared_ptr<RealFunctionBase> CompositeFunction::calculateDerivative() const{
    shared_ptr<RealFunctionBase> factor1 = std::make_shared<CompositeFunction>(outer->getDerivative(), inner);
    shared_ptr<RealFunctionBase> factor2 = inner->getDerivative();

    return std::make_shared<ProductFunction>(factor1, factor2);
}

}

