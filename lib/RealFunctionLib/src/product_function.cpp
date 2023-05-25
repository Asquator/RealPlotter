#include <functional>
#include <memory>

#include "binary_operation_function.h"
#include "product_function.h"
#include "sum_function.h"

using std::ostream;
using std::shared_ptr;


namespace RealFunctionLib_impl {

ProductFunction::ProductFunction(shared_ptr<RealFunctionBase> left, shared_ptr<RealFunctionBase> right):
    BinaryOperationFunction(std::move(left), std::move(right), std::multiplies<real_type>()) {}



void ProductFunction::print(ostream &os) const{
    os << "(" << *getLeftOperand() << ") * (" << *getRightOperand() << ")";  
}

/**
 * @brief Calculating the derivative of a product function using the Leibniz rule
 * 
 * @return std::shared_ptr<RealFunctionBase> shared pointer to the derivative
 */
std::shared_ptr<RealFunctionBase>ProductFunction::calculateDerivative() const {

    /*Applying Leibniz rule to calculate the derivative of a product*/

    shared_ptr<RealFunctionBase> prod1 = std::make_shared<ProductFunction>(getLeftOperand()->getDerivative(), getRightOperand());
    shared_ptr<RealFunctionBase> prod2 = std::make_shared<ProductFunction>(getLeftOperand(), getRightOperand()->getDerivative());
    return std::make_shared<SumFunction>(std::move(prod1), std::move(prod2));
}

}
