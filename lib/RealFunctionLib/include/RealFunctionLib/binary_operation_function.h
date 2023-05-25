#ifndef BINARY_OPERATION_FUNCTION_H
#define BINARY_OPERATION_FUNCTION_H

#include <functional>
#include <memory>
#include <ostream>
#include <string>

#include "real_function_base.h"

namespace RealFunctionLib_impl{
class BinaryOperationFunction : public RealFunctionBase{

    using operation_type = std::function<real_type (real_type, real_type)>;

    private:
        operation_type operation; 
        std::shared_ptr<RealFunctionBase> leftOperand;   
        std::shared_ptr<RealFunctionBase> rightOperand; 

    public:
        BinaryOperationFunction(std::shared_ptr<RealFunctionBase>, std::shared_ptr<RealFunctionBase>, const operation_type &);
        
                    
        real_type operator()(real_type) const override;
        bool isDefined(real_type) const override;

    protected: 
        inline std::shared_ptr<RealFunctionBase> getLeftOperand() const {return leftOperand;}
        inline std::shared_ptr<RealFunctionBase> getRightOperand() const {return rightOperand;};
        
};


}

#endif
