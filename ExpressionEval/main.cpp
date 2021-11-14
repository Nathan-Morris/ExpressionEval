#include <iostream>

#include "Expression.h"
#include "ExpressionOperation.h"

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
	});

	Expression expr("123+7*sin(x)");

	cout << expr.nodeCount() << " -> " << expr;
}