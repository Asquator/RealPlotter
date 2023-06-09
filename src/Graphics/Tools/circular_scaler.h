#ifndef CIRCULAR_SCALER_H
#define CIRCULAR_SCALER_H

#include <array>

template <int N> class CircularScaler {
public:
    CircularScaler(std::array<double, N> arr, int startUpscalePos):
        factors(arr), pos{startUpscalePos} {}

    double nextUp(){
        return factors[pos];
    }

    double nextDown(){
        return pos == 0 ? factors[factors.size() - 1] : factors[pos];
    }

    double scaleUp(){
        //take this element as a scale factor
        double ret = factors[pos++];
        if(pos == factors.size())
            pos = 0;

        return ret;
    }

    double scaleDown(){
        //take previous value as a scale factor
        if(--pos < 0)
            pos = factors.size()-1;

        return factors[pos];
    }

private:
    int pos;
    std::array<double, N> factors;
};

#endif
