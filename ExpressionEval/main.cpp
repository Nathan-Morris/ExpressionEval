#include <iostream>

#include "ExpressionOperation.h"

using namespace std;

/*

*/

int main(int argc, char* argv[], char* env[]) {
	BinaryOperationInfo::declareOperations(
		"+", 1, "-", 1, "*", 2, "/", 3, "^", 4
	);

	UnaryOperationInfo::declareOperations(
		"sin", "cos", "tan", "csc"
	);

	for (const auto& p : UnaryOperationInfo::DECLARED_OPERATIONS) {
		cout << p.first.id() << "|" << p.first.precedent() << " -> " << p.second.token() << endl;
	}

	const char* equStr = "sincos";
	const void* ptr;
	
	cout << UnaryOperationInfo::findOperation(equStr + 3, &ptr) << endl;
	cout << ((UnaryOperationInfo*)ptr)->token() << endl;
}