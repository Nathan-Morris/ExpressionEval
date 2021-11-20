#include <iostream>
#include <string>

#include "ExpressionNode.h"
#include "Expression.h"

using namespace std;

/*
string -> tokens
*/

static std::vector<ExpressionNode> strToEquList(const std::string& equStr) {
	std::vector<ExpressionNode> nodes;
	ExpressionNode nodeFound;
	char* numEndP = NULL;


	for (size_t i = 0; i != equStr.size(); i++) {
		const char& c = equStr.at(i);
		size_t tokenInc = ExpressionNode::parseAt(&c, nodeFound);

		if (tokenInc) {
			nodes.push_back(nodeFound);
			i += tokenInc - 1;
		}
	}

	return nodes;
}

static void equListPrint(const std::vector<ExpressionNode>& equList) {
	for (ExpressionNode node : equList) {
		cout << node;
	}
}


int main(int argc, char* argv[], char* env[]) {
	OperationNodeInfo("+", 1, [](FloatType a, FloatType b) -> FloatType { return a + b; });
	OperationNodeInfo("-", 1, [](FloatType a, FloatType b) -> FloatType { return a - b; });
	OperationNodeInfo("*", 1, [](FloatType a, FloatType b) -> FloatType { return a * b; });
	OperationNodeInfo("/", 1, [](FloatType a, FloatType b) -> FloatType { return a / b; });
	OperationNodeInfo("^", 1, [](FloatType a, FloatType b) -> FloatType { return pow(a, b); });
	OperationNodeInfo("sin", [](FloatType x) -> FloatType { return sin(x); });
	OperationNodeInfo("cos", [](FloatType x) -> FloatType { return cos(x); });
	OperationNodeInfo("tan", [](FloatType x) -> FloatType { return tan(x); });

	string inEqu;
	while (1) {
		cout << "Equation = ";
		getline(cin, inEqu);
		cout << Expression(inEqu) << endl;
	}
}