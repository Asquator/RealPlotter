#ifndef REAL_FUNCTION_BASE_H
#define REAL_FUNCTION_BASE_H


#include <memory>
#include <ostream>
#include "real_math.h"

namespace RealFunctionLib_impl{

class RealFunctionBase{

	friend std::ostream &operator<<(std::ostream &, const RealFunctionBase &);

	private:
		mutable std::shared_ptr<RealFunctionBase> derivative = nullptr;
		
		virtual std::shared_ptr<RealFunctionBase> calculateDerivative() const = 0;
		virtual void print(std::ostream &) const = 0;


	public:
		
		/*asking the synthesized copy control operations*/
		RealFunctionBase() = default;

		RealFunctionBase(const RealFunctionBase &) = default;
		RealFunctionBase &operator=(const RealFunctionBase &) = default;

		RealFunctionBase(RealFunctionBase &&) = default;
		RealFunctionBase &operator=(RealFunctionBase &&) = default;

		 /*polymorphic destructor*/
		virtual ~RealFunctionBase();

		
		std::shared_ptr<RealFunctionBase> getDerivative() const;

		virtual bool isDefined(real_type) const = 0;

		virtual real_type operator()(real_type) const = 0;

};

}

#endif