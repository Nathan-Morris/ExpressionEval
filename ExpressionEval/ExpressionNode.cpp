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
	default: this->mType = ExpressionNodeType::VARIABLE; this->mData.varChar = c; break;
	}
}
