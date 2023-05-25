#include <memory>
#include <ostream>
#include <vector>

#include "real_math.h"
#include "polynomial.h"

using namespace std;

namespace RealFunctionLib_impl{
/**
 * @brief Construct a default zero Polynomial
 * 
 */
Polynomial::Polynomial() : coefficients{0}{};


/**
 * @brief Construct a new Polynomial from initializer list
 * 
 * @param coefficients_list initializer list of coefficients
 */
Polynomial::Polynomial(initializer_list<real_type> coefficients_list) : 
coefficients(coefficients_list) {

    //preserving the invariant: at least one coefficient should exist
    if(coefficients.size() == 0)
        coefficients = {0};
    reduce();
}


/**
 * @brief Construct a new Polynomial from a given vector
 * 
 * @param coefficients_vector vector of coefficients
 */
Polynomial::Polynomial(const vector<real_type> &coefficients_vector) : 
coefficients(coefficients_vector) {
    reduce();
}


Polynomial::Polynomial(vector<real_type> &&coefficients_vector):
coefficients(std::move(coefficients_vector)){
    reduce();
}

/**
 * @brief Remove trailing zeroes from the vector of coefficients
 * 
 */
void Polynomial::reduce(){
    //remove trailing 0 elements except the first one
    while(coefficients.size() > 1 && coefficients.back() == 0)
        coefficients.pop_back();
    coefficients.shrink_to_fit();
}



/**
 * @brief  Compute the Polynomial value at a point
 * Apply Horner rule to calculate the value in linear time
 * @param x given point
 * @return real_type Polynomial value
 */
real_type Polynomial::operator()(real_type x) const {
    real_type val = 0;
    
    for(auto p = coefficients.crbegin(); p != coefficients.crend(); ++p)
        val = x * val + *p;

    return val;
}


void Polynomial::print(ostream &os) const{
    os << coefficients[0] << (coefficients.size() > 1 ? " + " : "");

    for(vector<real_type>::size_type i = 1; i < coefficients.size(); i++)
        os << coefficients[i] << Polynomial::variableName << "^" << i
        << (i < coefficients.size() - 1 ? " + " : "");
}


/**
 * @brief Return true iff the Polynomial equals to zero
 * 
 */
bool Polynomial::isZero() const{
    return coefficients.size() == 1 && coefficients[0] == 0;
}


/**
 * @brief Return degree of the polynomial. Zero polynomial is said to have 0 degree.
 * @return int degree of the polynomial
*/
vector<real_type>::size_type Polynomial::getDegree() const{
    return coefficients.size() - 1;
}


std::shared_ptr<RealFunctionBase>Polynomial::calculateDerivative() const{
    Polynomial *deriv = new Polynomial{};

    //if the polynomial isn't const
    if(getDegree()){
        deriv->coefficients = {}; //emptying the derivative coefficients
        for(size_t i = 1; i < coefficients.size(); ++i)
            deriv->coefficients.push_back(coefficients[i] * i);
    }
    
    deriv->reduce();
    return std::unique_ptr<RealFunctionBase>(deriv);
}

}
