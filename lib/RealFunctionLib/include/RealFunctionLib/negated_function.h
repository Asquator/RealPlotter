#ifndef NEGATED_FUNCTION_H
#define NEGATED_FUNCTION_H


#include <memory>
#include "real_function_base.h"

namespace RealFunctionLib_impl {

	class NegatedFunction : public RealFunctionBase {

		private:
			std::shared_ptr<RealFunctionBase> operand;

			std::shared_ptr<RealFunctionBase> calculateDerivative() const override;
			void print(std::ostream &) const override;

		public:

			NegatedFunction(std::shared_ptr<RealFunctionBase>);

			bool isDefined(real_type) const override;
			real_type operator()(real_type) const override;
			

	};

}



#endif