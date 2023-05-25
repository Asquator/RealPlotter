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

        friend RealFunction createPolynomial(const std::vector<real_type> &coefficients_vector);

        friend RealFunction createPolynomial(std::vector<real_type>&& coefficients_vector);

        friend RealFunction createPolynomial(std::initializer_list<real_type> il);

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
RealFunction createPolynomial(const std::vector<real_type> &);
RealFunction createPolynomial(std::vector<real_type> &&);
RealFunction createPolynomial(std::initializer_list<real_type>);

}

#endif
