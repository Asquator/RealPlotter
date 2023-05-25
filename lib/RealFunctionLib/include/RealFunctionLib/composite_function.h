#ifndef COMPOUND_FUNCTION_H
#define COMPOUND_FUNCTION_H

#include "real_function_base.h"
#include <memory>

namespace RealFunctionLib_impl{

class CompositeFunction : public RealFunctionBase {

	public:
        CompositeFunction(std::shared_ptr<RealFunctionBase>, std::shared_ptr<RealFunctionBase>);

        //CompositeFunction(const RealFunctionBase &, const RealFunctionBase &);
        bool isDefined(real_type) const override;
        real_type operator()(real_type x) const override;

    private:
        std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
        void print(std::ostream &os) const override;

		std::shared_ptr<RealFunctionBase> outer;
		std::shared_ptr<RealFunctionBase> inner;

};

}

#endif