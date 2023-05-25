
/**
 * @file real_math.h
 * @brief basic mathematical declarations for the real function API
 */

#ifndef REAL_MATH_H
#define REAL_MATH_H

#include <limits>
#include <cmath>

using real_type = double;

namespace Real_Math{

//small epsilon value
constexpr real_type real_epsilon {10 * std::numeric_limits<real_type>::epsilon()};

constexpr real_type PI = M_PI;
constexpr real_type E = M_E;

/**
 * @brief Returns true if the distance between two numbers is less than real_epsilon
 */
bool round_compare(real_type, real_type);

}

#endif 
