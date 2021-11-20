#include <iostream>
#include <string>

#include "_ExpressionNode.h"

using namespace std;

/*
string -> tokens
*/

static std::vector<_ExpressionNode*> strToEquList(const std::string& equStr) {
	std::vector<_ExpressionNode*> nodes;
	_ExpressionNode* nodeFoundPtr;
	char* numEndP = NULL;


	for (size_t i = 0; i != equStr.size(); i++) {
		const char& c = equStr.at(i);

		if (ScopeNode::isScopeStr(&c, &nodeFoundPtr)) {
			nodes.push_back(nodeFoundPtr);
		}
		else if (OperationNode::isOperationStr(&c, &nodeFoundPtr)) {
			nodes.push_back(nodeFoundPtr);
			i += static_cast<const OperationNode*>(nodeFoundPtr)->tokenLen() - 1;
		}
		else if (isdigit(c) || c == '.') {
			// todo -x numbers
			nodes.push_back(new OperandNode(strtoft(&c, &numEndP)));
			if (*numEndP == 0) {
				break;
			}
			else {
				i += (numEndP - &c) - 1;
			}
		}
		else if (c == 'x') {
			nodes.push_back(new OperandNode(c));
		}
	}

	return nodes;
}

static void equListPrint(const std::vector<_ExpressionNode*>& equList) {
	OperandNode* andNodePtr;
	OperationNode* opNodePtr;
	ScopeNode* scNodePtr;

	for (_ExpressionNode* node : equList) {
		andNodePtr = (OperandNode*)node;
		opNodePtr = (OperationNode*)node;
		scNodePtr = (ScopeNode*)node;

		switch (node->type())
		{
		case EXPR_NODE_OPERAND:
			switch (node->subType())
			{
			case OPERAND_NODE_VALUE:
				std::cout << andNodePtr->value();
				break;
			case OPERAND_NODE_VARIABLE:
				std::cout << andNodePtr->variable();
			}
			break;

		case EXPR_NODE_OPERATION:
			std::cout << opNodePtr->token();
			break;

		case EXPR_NODE_SCOPE:
			std::cout << (node->subType() == SCOPE_NODE_INCREMENT) ? '(' : ')';
			break;
		}
	}
}


int main(int argc, char* argv[], char* env[]) {

	OperationNode("+", 1, [](FloatType a, FloatType b) -> FloatType { return a + b; });
	OperationNode("-", 1, [](FloatType a, FloatType b) -> FloatType { return a - b; });
	OperationNode("*", 1, [](FloatType a, FloatType b) -> FloatType { return a * b; });
	OperationNode("/", 1, [](FloatType a, FloatType b) -> FloatType { return a / b; });
	OperationNode("^", 1, [](FloatType a, FloatType b) -> FloatType { return pow(a, b); });

	string inEqu;
	cout << "Equation = ";
	getline(cin, inEqu);

	equListPrint(strToEquList(inEqu));


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