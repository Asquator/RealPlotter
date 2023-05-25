#include "negated_function.h"
#include "binary_operation_function.h"
#include "real_function_base.h"
#include <memory>
#include <ostream>


using std::shared_ptr;

namespace RealFunctionLib_impl {


NegatedFunction::NegatedFunction(std::shared_ptr<RealFunctionBase> other):
	operand(std::move(other)) {}


bool NegatedFunction::isDefined(real_type x) const {
	return operand->isDefined(x);
}

real_type NegatedFunction::operator()(real_type x) const{
	return -(*operand)(x);
}

void NegatedFunction::print(std::ostream &os) const{
    os << "-(" << *operand << ")";
}

std::shared_ptr<RealFunctionBase>NegatedFunction::calculateDerivative() const{
    return std::make_shared<NegatedFunction>(operand->getDerivative());
}

}
