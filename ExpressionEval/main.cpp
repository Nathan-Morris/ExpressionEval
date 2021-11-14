#include <iostream>

#include "ExpressionTree.h"

using namespace std;

/*
string -> tokens
*/

int main(int argc, char* argv[], char* env[]) {
	ExpressionOperationInfo::declareOperations({
		ExpressionOperationInfo("+", OperationId(1, 0)),
		ExpressionOperationInfo("-", OperationId(1, 0)),
		ExpressionOperationInfo("*", OperationId(1, 1)),
		ExpressionOperationInfo("/", OperationId(1, 1)),
		ExpressionOperationInfo("^", OperationId(1, 2)),
		ExpressionOperationInfo("sin", OperationId(0)),
		ExpressionOperationInfo("cos", OperationId(0)),
		ExpressionOperationInfo("tan", OperationId(0)),
	});

	Expression expr("cos(cos(x) * cos(x))");

	ExpressionTree tree(expr);
}