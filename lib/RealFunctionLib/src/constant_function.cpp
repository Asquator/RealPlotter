#include "constant_function.h"
#include "real_function_base.h"
#include "real_math.h"
#include <memory>


namespace RealFunctionLib_impl {

ConstantFunction::ConstantFunction(real_type val) : val(val){}

std::shared_ptr<RealFunctionBase> ConstantFunction::calculateDerivative() const{
    return std::shared_ptr<RealFunctionBase>{std::make_shared<ConstantFunction>(0)};
}


real_type ConstantFunction::operator()(real_type) const{
    return val;
}

bool ConstantFunction::isDefined(real_type) const{
    return true;
}

std::shared_ptr<RealFunctionBase> ConstantFunction::one(){
    return constantOne;
}

void ConstantFunction::print(std::ostream &os) const{
    os << val;
}

}

