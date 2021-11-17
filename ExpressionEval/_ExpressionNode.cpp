#include "_ExpressionNode.h"

//
// ==============================================================
//

_ExpressionNode::_ExpressionNode(unsigned char type, unsigned char subType) : mType(type), mSubType(subType) { }

unsigned char _ExpressionNode::type() {
	return this->mType;
}

unsigned char _ExpressionNode::subType() {
	return this->mSubType;
}

//
// ==============================================================
//

OperandNode::OperandNode(unsigned char subType) : _ExpressionNode(EXPR_NODE_OPERAND, subType) { }

OperandNode::OperandNode(FloatType value) : OperandNode(OPERAND_NODE_VALUE) {
	this->mData.value = value;
}

OperandNode::OperandNode(char variable) : OperandNode(OPERAND_NODE_VARIABLE) {
	this->mData.variable = variable;
}

FloatType OperandNode::value() {
	return this->mData.value;
}

char OperandNode::variable() {
	return this->mData.variable;
}

//
// ==============================================================
//

unsigned short OperationNode::LAST_OPERATION_ID = 0;

OperationNode::OperationNode(const char* token, unsigned char subType) : _ExpressionNode(EXPR_NODE_OPERATION, subType) {
	this->mOperationId = LAST_OPERATION_ID++;
	this->mToken = token;
	this->mTokenLen = strlen(token);
}

OperationNode::OperationNode(const char* token, unsigned char precedent, OperationNodeBinaryCallback callback) 
	: OperationNode(token, OPERATION_NODE_BINARY) {
	this->mOperationPrecedent = precedent;
	this->mCallbackData.bin = callback;
}

OperationNode::OperationNode(const char* token, OperationNodeUnaryCallback callback) 
	: OperationNode(token, OPERATION_NODE_UNARY) {
	this->mCallbackData.un = callback;
}

OperationNode::OperationNode(const char* token, unsigned char functionArgExpect, OperationNodeFunctionCallback callback)
	: OperationNode(token, OPERATION_NODE_FUNCTION) {
	this->mOperationFuncArgExpect = functionArgExpect;
	this->mCallbackData.func = callback;
}

//
// ==============================================================
//

std::vector<char> ScopeNode::SCOPE_INC_CHARS = { '[', '(' };
std::vector<char> ScopeNode::SCOPE_DEC_CHARS = { ']', ')' };

ScopeNode::ScopeNode(unsigned char subType) : _ExpressionNode(EXPR_NODE_SCOPE, subType) {

}

ScopeNode::ScopeNode(char scopeChar) 
	: ScopeNode(
		(std::find(SCOPE_INC_CHARS.begin(), SCOPE_INC_CHARS.end(), scopeChar) != SCOPE_INC_CHARS.end()) 
		? SCOPE_NODE_INCREMENT : SCOPE_NODE_DECREMENT
	)
{

}

//
// ==============================================================
//
