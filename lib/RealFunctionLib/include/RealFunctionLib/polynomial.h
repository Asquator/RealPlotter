#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <initializer_list>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <iterator>

#include "real_function_base.h"


namespace RealFunctionLib_impl{
/*
Polynomial: represents a real valued polynomial function
a class invariant is a reduced vector with polynomial coefficients without trailing zeroes
*/
class Polynomial : public RealFunctionBase {

     private:
        std::vector<real_type> coefficients;
        void reduce();

        std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
        void print(std::ostream &os) const override;


    public:
        static const char variableName = 'x'; 

        /*constructors*/
        Polynomial();
        explicit Polynomial(std::initializer_list<real_type>);
        Polynomial(const std::vector<real_type> &);
        Polynomial(std::vector<real_type> &&);

        /*
        copy control is inherited from RealFunctionBase.
        as polynomials are copied/moved from other polynomials, the invariant is always preserved
        */


        inline std::vector<real_type>::size_type getDegree() const;
        
        inline bool isZero() const;
        
        bool isDefined(real_type) const override {return true;}

        /**
         * @brief Compute the Polynomial value at a point
         * 
         * @return real_type Polynomial value
         */
        real_type operator()(real_type) const override;

   
};

}

#endif
