#include <iostream>

#include <RealFunctionLib/real_function.h>

#include <memory>

using namespace std;
using namespace RealFunctionLib;

int main(){
    RealFunction func = getSpecial(Specials::Cos);
	RealFunction pol = createPolynomial({2,4,5});

    RealFunction der = func.getDerivative(); //-sin

	RealFunction target = func * pol + der;

	cout << target << endl;
    RealFunction target_der = target.getDerivative();
	cout << target_der << endl;
	//cout << target << endl << target_der << endl;

	cout << target(2) << "  " << target_der(2) << endl;

    RealFunction log = getSpecial(Specials::Ln);
    RealFunction sin = getSpecial(Specials::Sin);
	RealFunction sinlog = sin(log);
	cout << sinlog(3.2);

	return 0;
}
