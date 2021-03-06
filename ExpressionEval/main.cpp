#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <cmath>

#include "ExpressionNode.h"
#include "Expression.h"

using namespace std;

int main(int argc, char* argv[], char* env[]) {
	OperationNodeInfo("+", 0, [](FloatType a, FloatType b) -> FloatType { return a + b; });
	OperationNodeInfo("-", 0, [](FloatType a, FloatType b) -> FloatType { return a - b; });
	OperationNodeInfo("*", 1, [](FloatType a, FloatType b) -> FloatType { return a * b; });
	OperationNodeInfo("/", 1, [](FloatType a, FloatType b) -> FloatType { return a / b; });
	OperationNodeInfo("^", 2, [](FloatType a, FloatType b) -> FloatType { return pow(a, b); });
	OperationNodeInfo("sin", [](FloatType x) -> FloatType { return sin(x); });
	OperationNodeInfo("cos", [](FloatType x) -> FloatType { return cos(x); });
	OperationNodeInfo("tan", [](FloatType x) -> FloatType { return tan(x); });
	OperationNodeInfo("ceil", [](FloatType x) -> FloatType { return ceil(x); });
	OperationNodeInfo("floor", [](FloatType x) -> FloatType { return floor(x); });
	OperationNodeInfo("rad", [](FloatType x) -> FloatType { return x * M_PI / 180.0; });
	OperationNodeInfo("deg", [](FloatType x) -> FloatType { return x * 180.0 / M_PI; });
	
	string inEqu;
	
	while (1) {
		cout << "Enter Expression: ";
		getline(cin, inEqu);
	
		cout << Expression(inEqu) << endl;
		cout << "= " << Expression(inEqu).solve() << endl;
	}
}