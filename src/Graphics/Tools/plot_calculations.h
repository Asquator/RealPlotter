#ifndef PLOT_CALC_H
#define PLOT_CALC_H

#include <RealFunctionLib/real_math.h>

/**
 * @brief round_zero rounds number to zero if it's close enough
 * @param x real number
 * @param epsilon small epsilon to use in rounding
 * @return 0 iff |x| < epsilon, otherwise x
 */
real_type round_zero(real_type x, real_type epsilon = Real_Math::real_epsilon);


#endif
