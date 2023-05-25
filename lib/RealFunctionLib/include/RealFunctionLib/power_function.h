#ifndef POWER_FUNCTION_H
#define POWER_FUNCTION_H

#include "binary_operation_function.h"
#include <memory>
#include <ostream>

namespace RealFunctionLib_impl{

class PowerFunction : public BinaryOperationFunction {

    public:

     //   PowerFunction(const RealFunctionBase &, const RealFunctionBase &);
        PowerFunction(std::shared_ptr<RealFunctionBase>, std::shared_ptr<RealFunctionBase>);


        bool isDefined(real_type) const override;


    private:
        std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
        void print(std::ostream &os) const override;

};

}


#endif
