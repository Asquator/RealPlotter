#ifndef REAL_FUNCTION_H
#define REAL_FUNCTION_H


#include <initializer_list>
#include <memory>
#include <ostream>
#include <vector>

#include "real_math.h"
#include "real_function_base.h"
#include "special_function.h"

namespace RealFunctionLib{
	
using Specials = RealFunctionLib_impl::SpecialFunction::Specials;

class RealFunction{
	private:
		//abstract base of any real function object
        using RealFunctionBase = RealFunctionLib_impl::RealFunctionBase;

        explicit RealFunction(std::shared_ptr<RealFunctionBase>);
		std::shared_ptr<RealFunctionBase> functionPtr = nullptr;
	
    public:
        friend RealFunction getSpecial(Specials type);

		friend RealFunction createConstant(real_type x);

		friend std::ostream &operator<<(std::ostream &, const RealFunction &);
		
        RealFunction() = default;

		RealFunction(const RealFunction &) = default;
		RealFunction &operator=(const RealFunction &) = default;

		RealFunction(RealFunction &&) = default;
		RealFunction &operator=(RealFunction &&) = default;

        inline bool isValid() const {return functionPtr != nullptr;}

		RealFunction getDerivative() const;
        RealFunction getDerivative(int order) const;

		bool isDefined(real_type) const;

		real_type operator()(real_type) const;

		bool isDifferentiable(real_type) const;

		real_type diff(real_type x);

		/*
        Assymetric operators
		*/
		RealFunction &operator+=(const RealFunction &);
		RealFunction &operator-=(const RealFunction &);
		RealFunction &operator*=(const RealFunction &);
        RealFunction &operator/=(const RealFunction &);
        RealFunction &applyPower(const RealFunction &);

        /*
         Symmetric operators
         */
        RealFunction operator()(const RealFunction &);
		friend RealFunction operator+(const RealFunction&, const RealFunction&);
		
		friend RealFunction operator-(const RealFunction&, const RealFunction&);

		friend RealFunction operator*(const RealFunction&, const RealFunction&);

		friend RealFunction operator/(const RealFunction&, const RealFunction&);

        RealFunction power(const RealFunction&);

		friend RealFunction operator-(const RealFunction&);


};


//Operators on real function wrapper

RealFunction operator+(const RealFunction&, const RealFunction&);

RealFunction operator-(const RealFunction&, const RealFunction&);

RealFunction operator*(const RealFunction&, const RealFunction&);

RealFunction operator/(const RealFunction&, const RealFunction&);

RealFunction operator-(const RealFunction&);

//Real function factory interface

RealFunction getSpecial(Specials);
RealFunction createConstant(real_type);

/**
 * @brief evaluate evaluates a given function on a |x1, x2| interval
 * @param function function to evaluate
 * @param x1 left end
 * @param x2 right end
 * @param delta delta between points to evaluate
 * @return vector of function values.
 * The first value is f(x1) and the others are calculated by adding delta to previous points
 */
std::vector<real_type> evaluate(RealFunctionLib::RealFunction function,
                                real_type x1, real_type x2, real_type delta = Real_Math::real_epsilon);

}



#endif
