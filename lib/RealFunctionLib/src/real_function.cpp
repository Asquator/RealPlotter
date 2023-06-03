
#include <functional>
#include <initializer_list>
#include <memory>
#include <ostream>

#include "fraction_function.h"

#include "composite_function.h"
#include "constant_function.h"
#include "negated_function.h"
#include "polynomial.h"
#include "power_function.h"

#include "real_function.h"
#include "real_function_base.h"
#include "difference_function.h"

#include "product_function.h"
#include "real_math.h"
#include "special_function.h"
#include "sum_function.h"

using namespace std;
using std::vector;


namespace RealFunctionLib_impl{
RealFunctionBase::~RealFunctionBase(){}

std::shared_ptr<RealFunctionBase> RealFunctionBase::getDerivative() const{
    if(!derivative)
        derivative = calculateDerivative();

    return derivative;    
}

ostream &operator<<(ostream &os, const RealFunctionBase &func){
    func.print(os);
    return os;
}

}

using namespace RealFunctionLib_impl;

namespace RealFunctionLib {

/*Real function wrapper to allow value semantics*/

RealFunction::RealFunction(shared_ptr<RealFunctionBase> ptr):
    functionPtr(std::move(ptr)){}


RealFunction RealFunction::getDerivative() const{
    return RealFunction(shared_ptr<RealFunctionBase>(functionPtr->getDerivative()));
}

RealFunction RealFunction::getDerivative(int order) const{
    RealFunction res = *this;

    for(int i = 0; i < order; ++i)
        res = res.getDerivative();

    return res;
}

real_type RealFunction::operator()(real_type x) const {
    return (*functionPtr)(x);
}

bool RealFunction::isDefined(real_type x) const{
    return functionPtr->isDefined(x);
}

bool RealFunction::isDifferentiable(real_type x) const{
    return functionPtr->getDerivative()->isDefined(x);
}

real_type RealFunction::diff(real_type x){
    return functionPtr->getDerivative()->operator()(x);
}

ostream &operator<<(ostream &os, const RealFunction &func){
    return (os << *func.functionPtr);
}

RealFunction createConstant(real_type x){
    return RealFunction(std::make_shared<ConstantFunction>(x));
}

RealFunction getSpecial(Specials type){
    return RealFunction{SpecialFunction::getSpecial(type)};
}

RealFunction createPolynomial(const vector<real_type> &coefficients_vector){
    //passing the given vector to the polynomial constructor
    return RealFunction{shared_ptr<RealFunctionBase>{new Polynomial{coefficients_vector}}};
}

RealFunction createPolynomial(vector<real_type>&& coefficients_vector){
    //moving the given vector to the polynomial constructor
    return RealFunction{shared_ptr<RealFunctionBase>{new Polynomial{std::move(coefficients_vector)}}};
}

RealFunction createPolynomial(std::initializer_list<real_type> il){
       //passing the given initializer list to the polynomial constructor 
       return RealFunction{shared_ptr<RealFunctionBase>{new Polynomial{il}}};
}


//Assignment real function operators

//Lvalue-reference asymmetric operators


RealFunction &RealFunction::operator+=(const RealFunction &other){
    functionPtr = std::make_shared<SumFunction>(functionPtr, other.functionPtr);
    return *this;
}

RealFunction &RealFunction::operator-=(const RealFunction &other){
    functionPtr = std::make_shared<DifferenceFunction>(functionPtr, other.functionPtr);
    return *this;
}

RealFunction &RealFunction::operator*=(const RealFunction &other){
    functionPtr = std::make_shared<ProductFunction>(functionPtr, other.functionPtr);
    return *this;
}

RealFunction &RealFunction::operator/=(const RealFunction &other){
    functionPtr = std::make_shared<FractionFunction>(functionPtr, other.functionPtr);
    return *this;
}

//POWER
RealFunction &RealFunction::applyPower(const RealFunction &other){
    functionPtr = std::make_shared<PowerFunction>(functionPtr, other.functionPtr);
    return *this;
}

//COMPOSITION
RealFunction RealFunction::operator()(const RealFunction &other){
    auto id = SpecialFunction::getSpecial(Specials::Id);
    if(functionPtr == id || other.functionPtr == id)
        return *this;

    return RealFunction(std::make_shared<CompositeFunction>(functionPtr, other.functionPtr));

}


//POWER
RealFunction RealFunction::power(const RealFunction &right){
    RealFunction tmp = *this;
    return tmp.applyPower(right);
}


//NEGATION
RealFunction operator-(const RealFunction &op){
    return RealFunction(std::make_shared<NegatedFunction>(op.functionPtr));
}


/*
Symmetric real function operators
*/


//SUM
RealFunction operator+(const RealFunction &left, const RealFunction &right){
    RealFunction tmp = left;
    return tmp += right;
}


//DIFFERENCE
RealFunction operator-(const RealFunction &left, const RealFunction &right){
    RealFunction tmp = left;
    return tmp -= right;
}

//PRODUCT
RealFunction operator*(const RealFunction &left, const RealFunction &right){
    RealFunction tmp = left;
    return tmp *= right;
}


//FRACTION
RealFunction operator/(const RealFunction &left, const RealFunction &right){
    RealFunction tmp = left;
    return tmp /= right;
}

vector<real_type> evaluate(RealFunction function, real_type x1, real_type x2,
                           real_type delta){
    vector<real_type> ret;

    ret.reserve(round((x2 - x1) / delta) + 1);

    for(real_type p = x1; p <= x2; p += delta)
        ret.push_back(function.isDefined(p) ? function(p) : Real_Math::real_NaN);

    return ret;
}


}

