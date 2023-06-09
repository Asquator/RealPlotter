#include <cmath>

#include "plot_calculations.h"

real_type round_zero(real_type x, real_type epsilon){
    if(std::abs(x) < epsilon)
        return 0;

    return x;
}
