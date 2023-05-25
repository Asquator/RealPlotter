#ifndef FRACTION_FUNCTION_H
#define FRACTION_FUNCTION_H

#include <memory>
#include <ostream>

#include "binary_operation_function.h"


namespace RealFunctionLib_impl{

class FractionFunction : public BinaryOperationFunction {

    public:
        FractionFunction(std::shared_ptr<RealFunctionBase>, std::shared_ptr<RealFunctionBase>);
        bool isDefined(real_type) const override;

    private:
        std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
        void print(std::ostream &os) const override;

};

}

#endif
