#include "ExpressionNode.h"

ExpressionNode::ExpressionNode(ExpressionNodeType type, OperationId opId) : mType(type) {
	this->mData.opId = opId;
}

ExpressionNode::ExpressionNode(float value) : mType(ExpressionNodeType::VALUE) {
	this->mData.value = value;
}

ExpressionNode::ExpressionNode(char c) {
	switch (c)
	{
	case '[':
	case '(':
		this->mType = ExpressionNodeType::SCOPE_INC;
		break;

	case ']':
	case ')':
		this->mType = ExpressionNodeType::SCOPE_DEC;
		break;

	default: 
		this->mType = ExpressionNodeType::VARIABLE; 
		this->mData.varChar = c; 
		break;
	}
}

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node) {
	const ExpressionOperationInfo* infoPtr;

	switch (node.mType)
	{
	case ExpressionNodeType::VALUE:
		out << node.mData.value;
		break;
	case ExpressionNodeType::VARIABLE:
		out << node.mData.varChar;
		break;
	case ExpressionNodeType::SCOPE_INC:
		out << '(';
		break;
	case ExpressionNodeType::SCOPE_DEC:
		out << ')';
		break;
	case ExpressionNodeType::OPERATION:
		if (ExpressionOperationInfo::findOperation(node.mData.opId, &infoPtr)) {
			out << infoPtr->token();
		}
		else {
			out << '.';
		}
		break;
	}
	return out;
}