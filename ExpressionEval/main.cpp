#include <iostream>
#include <string>

#include "_ExpressionNode.h"

using namespace std;

/*
string -> tokens
*/

static std::vector<_ExpressionNode> strToEquList(const std::string& equStr) {
	std::vector<_ExpressionNode> nodes;
	_ExpressionNode nodeFound;
	char* numEndP = NULL;


	for (size_t i = 0; i != equStr.size(); i++) {
		const char& c = equStr.at(i);
		size_t tokenInc = _ExpressionNode::parseAt(&c, nodeFound);

		if (tokenInc) {
			nodes.push_back(nodeFound);
			i += tokenInc - 1;
		}
	}

	return nodes;
}

static void equListPrint(const std::vector<_ExpressionNode>& equList) {
	for (_ExpressionNode node : equList) {
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
		equListPrint(strToEquList(inEqu));
		cout << endl;
	}
}