#include <iostream>

#include "ExpressionTree.h"

#include "_ExpressionNode.h"

using namespace std;

/*
string -> tokens
*/

int main(int argc, char* argv[], char* env[]) {


	//ExpressionOperationInfo::declareOperations({
	//	ExpressionOperationInfo("+", OperationId(1, 0), [](float a, float b)->float {return a + b;}),
	//	ExpressionOperationInfo("-", OperationId(1, 0), [](float a, float b)->float {return a - b; }),
	//	ExpressionOperationInfo("*", OperationId(1, 1), [](float a, float b)->float {return a * b; }),
	//	ExpressionOperationInfo("/", OperationId(1, 1), [](float a, float b)->float {return a / b; }),
	//	ExpressionOperationInfo("^", OperationId(1, 2), [](float a, float b)->float {return pow(a, b); }),
	//	ExpressionOperationInfo("sin", OperationId(0), [](float a)->float {return sin(a); }),
	//	ExpressionOperationInfo("cos", OperationId(0), [](float a)->float {return cos(a); }),
	//	ExpressionOperationInfo("tan", OperationId(0), [](float a)->float {return tan(a); }),
	//});
	//
	//std::string in;
	//
	//cout << "Expression = ";
	//getline(cin, in);
	//
	//Expression expr(in);
	//ExpressionTree tree(expr);
	//
	//cout << tree.solve() << endl;
}