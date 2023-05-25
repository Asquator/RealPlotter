#ifndef DIFFERENCE_FUNCTION_H
#define DIFFERENCE_FUNCTION_H

#include <memory>

#include "binary_operation_function.h"

namespace RealFunctionLib_impl{
class DifferenceFunction : public BinaryOperationFunction{

    public:
     //   DifferenceFunction(const RealFunctionBase &, const RealFunctionBase &);
        DifferenceFunction(std::shared_ptr<RealFunctionBase>, std::shared_ptr<RealFunctionBase>);
        
    private:
        std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
        void print(std::ostream &os) const override;
};

}

#endif