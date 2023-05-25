#include <functional>
#include <memory>
#include <cmath>

#include "constant_function.h"
#include "fraction_function.h"
#include "negated_function.h"
#include "product_function.h"
#include "real_function_base.h"
#include "real_math.h"
#include "special_function.h"

using std::shared_ptr;
using std::map;
using Specials = RealFunctionLib_impl::SpecialFunction::Specials;

using namespace Real_Math;

namespace RealFunctionLib_impl {

SpecialFunction::SpecialFunction(Specials specialType,
						const mapping_type &function_mapping,
						const domain_type &domain,
						const std::string &name):

specialType(specialType), function_mapping(function_mapping), 
					domain(domain), name(name) {}


/*
Special function mappings info
*/
map<Specials, std::shared_ptr<RealFunctionBase>> SpecialFunction::specialFunctions {
    {Specials::Exp, shared_ptr<RealFunctionBase> (new SpecialFunction (Specials::Exp, exp,
                                    [](real_type){return true;}, "exp"))},
	{Specials::Ln, shared_ptr<RealFunctionBase> (new SpecialFunction (Specials::Ln, log,
                                    [](real_type x){return x > 0;}, "ln"))},
	{Specials::Sin, shared_ptr<RealFunctionBase> (new SpecialFunction (Specials::Sin, sin, 
									[](real_type){return true;}, "sin"))},
	{Specials::Cos, shared_ptr<RealFunctionBase> (new SpecialFunction (Specials::Cos, cos, 
									[](real_type){return true;}, "cos"))},
	{Specials::Tan, shared_ptr<RealFunctionBase> (new SpecialFunction (Specials::Tan, tan,
									[](real_type x){return !round_compare(cos(x), 0);}, "tan"))},
	{Specials::Id, shared_ptr<RealFunctionBase> (new SpecialFunction (Specials::Id, [](real_type x){return x;}, 
													[](real_type){return true;}, "x"))}
};


/*
Derivatives of special functions
*/
map<Specials, std::shared_ptr<RealFunctionBase>> SpecialFunction::derivativesFunctions{
    {Specials::Exp, shared_ptr<RealFunctionBase> (specialFunctions[Specials::Exp])},

    {Specials::Ln, shared_ptr<RealFunctionBase>(new FractionFunction{ConstantFunction::one(), specialFunctions[Specials::Id]})},

    {Specials::Sin, std::shared_ptr<RealFunctionBase> (specialFunctions[Specials::Cos])},

    {Specials::Cos, std::shared_ptr<RealFunctionBase> (new NegatedFunction(specialFunctions[Specials::Sin]))},

    {Specials::Tan, std::shared_ptr<RealFunctionBase>(new FractionFunction{shared_ptr<RealFunctionBase>{ConstantFunction::one()},
                                        shared_ptr<RealFunctionBase>{new ProductFunction{specialFunctions[Specials::Cos],
                                                                                                           specialFunctions[Specials::Cos]}
                                                                                                                                  }})},

	{Specials::Id, ConstantFunction::one()}
};


std::shared_ptr<RealFunctionBase> SpecialFunction::getSpecial(Specials spec){
	return specialFunctions[spec];
}


bool SpecialFunction::isDefined(real_type x) const {return domain(x);}

real_type SpecialFunction::operator()(real_type x) const {return function_mapping(x);}


void SpecialFunction::print(std::ostream &os) const {
	os << name;
}


std::shared_ptr<RealFunctionBase>SpecialFunction::calculateDerivative() const {
	return derivativesFunctions[specialType];
}

}
