#ifndef PRODUCT_FUNCTION_H
#define PRODUCT_FUNCTION_H

#include "binary_operation_function.h"

namespace RealFunctionLib_impl{

class ProductFunction : public BinaryOperationFunction {

    public:
        ProductFunction(std::shared_ptr<RealFunctionBase>, std::shared_ptr<RealFunctionBase>);

    private:
        std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
        void print(std::ostream &os) const override;

};

}


#endif