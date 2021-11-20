#include "_ExpressionNode.h"

//
// ==============================================================
//

_ExpressionNode::_ExpressionNode(unsigned char type, unsigned char subType) : mType(type), mSubType(subType) { }

_ExpressionNode::_ExpressionNode() : _ExpressionNode(0, 0) { }

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

FloatType OperandNode::value() const {
	return this->mData.value;
}

char OperandNode::variable() const {
	return this->mData.variable;
}

//
// ==============================================================
//


std::map<unsigned int, OperationNode> OperationNode::DEFINED_OPERATIONS = { };

OperationNode::OperationNode() : _ExpressionNode() {

}

// http://www.cse.yorku.ca/~oz/hash.html <-- hash algoritm
OperationNode::OperationNode(const char* token, unsigned char subType) : _ExpressionNode(EXPR_NODE_OPERATION, subType) {
	this->mToken = token;
	this->mTokenLen = strlen(token);

	this->mOperationId = 5381;
	
	for (unsigned char i = 0; token[i]; i++) {
		this->mOperationId = ((this->mOperationId << 5) + this->mOperationId) + token[i];
	}

	DEFINED_OPERATIONS[this->mOperationId] = *this;
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

unsigned int OperationNode::opId() const {
	return this->mOperationId;
}

const char* OperationNode::token() const {
	return this->mToken;
}

unsigned char OperationNode::tokenLen() const {
	return this->mTokenLen;
}

bool OperationNode::isOperationStr(const char* c, _ExpressionNode** dynStore) {
	for (const auto& opInfo : DEFINED_OPERATIONS) {
		if (!strncmp(c, opInfo.second.mToken, opInfo.second.mTokenLen)) {
			if (dynStore) {
				*dynStore = new OperationNode(opInfo.second);
			}
			return true;
		}
	}
	return false;
}

bool OperationNode::isOperationId(unsigned int id, _ExpressionNode** dynStore) {
	const auto iter = DEFINED_OPERATIONS.find(id);

	if (iter != DEFINED_OPERATIONS.end()) {
		if (dynStore) {
			*dynStore = new OperationNode(iter->second);
		}
		return true;
	}

	return false;
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
{ }

bool ScopeNode::isScopeStr(const char* c, _ExpressionNode** dynStore) {
	if (std::find(SCOPE_INC_CHARS.begin(), SCOPE_INC_CHARS.end(), *c) != SCOPE_INC_CHARS.end()) {
		if (dynStore) {
			*dynStore = new ScopeNode(SCOPE_NODE_INCREMENT);
		}
		return true;
	}
	else if (std::find(SCOPE_DEC_CHARS.begin(), SCOPE_DEC_CHARS.end(), *c) != SCOPE_DEC_CHARS.end()) {
		if (dynStore) {
			*dynStore = new ScopeNode(SCOPE_NODE_DECREMENT);
		}
		return true;
	}

	return false;
}

//
// ==============================================================
//
