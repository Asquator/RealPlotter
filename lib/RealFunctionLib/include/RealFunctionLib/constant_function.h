#ifndef CONSTANT_FUNCTION_H
#define CONSTANT_FUNCTION_H

#include <memory>

#include "real_function_base.h"
#include "real_math.h"

namespace RealFunctionLib_impl{

class ConstantFunction : public RealFunctionBase {

private:
    std::shared_ptr<RealFunctionBase> calculateDerivative() const final;
    void print(std::ostream &) const override;

    real_type val;

    inline static std::shared_ptr<RealFunctionBase> constantOne = std::make_shared<ConstantFunction>(1);


public:
    ConstantFunction(real_type);

    bool isDefined(real_type) const override;
    real_type operator()(real_type) const override;
    static std::shared_ptr<RealFunctionBase> one();
};

}

#endif
