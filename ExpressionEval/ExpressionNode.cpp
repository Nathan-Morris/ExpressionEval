#include "ExpressionNode.h"

//
// ==============================================================
//

ExpressionNode::ExpressionNode(unsigned char type, unsigned char subType) : mType(type), mSubType(subType) { }

ExpressionNode::ExpressionNode() : ExpressionNode(0xF, 0xF) { }

ExpressionNode::ExpressionNode(FloatType value) : ExpressionNode(EXPR_NODE_OPERAND, OPERAND_NODE_VALUE) {
	this->mData.operand.value = value;
}

ExpressionNode::ExpressionNode(char variableOrScope) {
	switch (variableOrScope)
	{
	case '(':
	case '[':
		this->mType = EXPR_NODE_SCOPE;
		this->mSubType = SCOPE_NODE_INCREMENT;
		return;

	case ')':
	case ']':
		this->mType = EXPR_NODE_SCOPE;
		this->mSubType = SCOPE_NODE_DECREMENT;
		return;

	case OPERAND_NODE_VAR_CHAR:
		this->mType = EXPR_NODE_OPERAND;
		this->mSubType = OPERAND_NODE_VARIABLE;
		this->mData.operand.variable = OPERAND_NODE_VAR_CHAR;
		return;
	}
}

ExpressionNode::ExpressionNode(const OperationNodeInfo& opInfo) {
	this->mType = EXPR_NODE_OPERATION;
	this->mSubType = opInfo.type();
	this->mData.operation.id = opInfo.opId();
}

unsigned char ExpressionNode::type() const {
	return this->mType;
}

unsigned char ExpressionNode::subType() const {
	return this->mSubType;
}

FloatType ExpressionNode::value() const {
	if (this->mType != EXPR_NODE_OPERAND || this->mSubType != OPERAND_NODE_VALUE) {
		throw std::runtime_error("Unable To Get Value From Non Operand Node");
	}
	return this->mData.operand.value;
}

char ExpressionNode::variable() const {
	if (this->mType != EXPR_NODE_OPERAND || this->mSubType != OPERAND_NODE_VARIABLE) {
		throw std::runtime_error("Unable To Get Variable From Non Operand Node");
	}
	return this->mData.operand.variable;
}

const OperationNodeInfo& ExpressionNode::operation() const {
	if (this->mType != EXPR_NODE_OPERATION) {
		throw std::runtime_error("Unable To Get Operation Information From Non Operation Node");
	}

	const OperationNodeInfo* declared;

	if (!OperationNodeInfo::isDeclared(this->mData.operation.id, &declared)) {
		throw std::runtime_error("Unable To Find Declared Operation From Id");
	}

	return *declared;
}

size_t ExpressionNode::parseAt(const char* at, ExpressionNode& store) {
	void* storagePtr = NULL;

	// handles value operand nodes
	if (*at == '.' || isdigit(*at)) {
		store = ExpressionNode(strtoft(at, (char**)&storagePtr));
		return (size_t)((char*)storagePtr - at);
	}

	// handles scope nodes and variable operand nodes
	switch (*at)
	{
	case '(':
	case ')':
	case '[':
	case ']':
	case OPERAND_NODE_VAR_CHAR:
		store = ExpressionNode(*at);
		return 1;
	}

	// handles operation nodes
	unsigned int opId = OperationNodeInfo::findId(at, (const OperationNodeInfo**)&storagePtr);
	if (opId) {
		store = ExpressionNode(*((const OperationNodeInfo*)storagePtr));
		return ((const OperationNodeInfo*)storagePtr)->tokenLen();
	}

	return 0;
}

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node) {
	switch (node.mType)
	{
	case EXPR_NODE_OPERAND:
		switch (node.mSubType)
		{
		case OPERAND_NODE_VALUE:
			return out << node.value();
		case OPERAND_NODE_VARIABLE:
			return out << node.variable();
		default:
			return out;
		}
	case EXPR_NODE_OPERATION:
		return out << node.operation();
	case EXPR_NODE_SCOPE:
		return out << (node.mSubType == SCOPE_NODE_INCREMENT ? '(' : ')');
	default:
		return out;
	}
}

//
// ==============================================================
//

//OperandNode::OperandNode(unsigned char subType) : ExpressionNode(EXPR_NODE_OPERAND, subType) { }
//
//OperandNode::OperandNode(FloatType value) : OperandNode(OPERAND_NODE_VALUE) {
//	this->mData.value = value;
//}
//
//OperandNode::OperandNode(char variable) : OperandNode(OPERAND_NODE_VARIABLE) {
//	this->mData.variable = variable;
//}
//
//FloatType OperandNode::value() const {
//	return this->mData.value;
//}
//
//char OperandNode::variable() const {
//	return this->mData.variable;
//}
//
//std::ostream& operator<<(std::ostream& out, const OperandNode& node) {
//	switch (node.mSubType)
//	{
//	case OPERAND_NODE_VALUE:
//		return out << node.mData.value;
//	case OPERAND_NODE_VARIABLE:
//		return out << node.mData.variable;
//	default:
//		return out;
//	}
//}

//
// ==============================================================
//

OperationNodeInfo::OperationNodeInfo(
	const char* token,
	void* callback,
	unsigned char callbackType,
	unsigned char precedent,
	unsigned char funcArgExpect
) {
	if (!token) {
		return;
	}


	unsigned int id = hashOperationId(token);
	const OperationNodeInfo* declaredPtr;

	if (isDeclared(id, &declaredPtr)) {
		*this = *declaredPtr;
	}
	else {
		this->mToken = token;
		this->mTokenLen = strlen(token);
		this->mCallbackData.bin = (OperationNodeBinaryCallback)callback;
		this->mCallbackType = callbackType;
		this->mOperationPrecedent = precedent;
		this->mOperationFuncArgExpect = funcArgExpect;

		printf("Put Operation `%s`\n", this->mToken);

		DECLARED_OPERATIONS[id] = *this;
	}
}

OperationNodeInfo::OperationNodeInfo() : OperationNodeInfo(NULL, NULL, 0, 0, 0) {

}

OperationNodeInfo::OperationNodeInfo(
	const char* token,
	unsigned char precedent,
	OperationNodeBinaryCallback callback
) : OperationNodeInfo(token, callback, OPERATION_NODE_BINARY, precedent, 0) { }

OperationNodeInfo::OperationNodeInfo(
	const char* token,
	OperationNodeUnaryCallback callback
) : OperationNodeInfo(token, callback, OPERATION_NODE_UNARY, 0xFF, 0) { }

OperationNodeInfo::OperationNodeInfo(
	const char* token,
	unsigned char funcArgExpect,
	OperationNodeFunctionCallback callback
) : OperationNodeInfo(token, callback, OPERATION_NODE_FUNCTION, 0xFF, funcArgExpect) { }

unsigned int OperationNodeInfo::opId() const {
	return hashOperationId(this->mToken);
}

const char* OperationNodeInfo::token() const {
	return this->mToken;
}

unsigned char OperationNodeInfo::tokenLen() const {
	return this->mTokenLen;
}

unsigned char OperationNodeInfo::type() const {
	return this->mCallbackType;
}

unsigned char OperationNodeInfo::precedent() const {
	return this->mOperationPrecedent;
}

OperationNodeBinaryCallback OperationNodeInfo::binary() const {
	return this->mCallbackData.bin;
}

OperationNodeUnaryCallback OperationNodeInfo::unary() const {
	return this->mCallbackData.un;
}

OperationNodeFunctionCallback OperationNodeInfo::function() const {
	return this->mCallbackData.func;
}

//
//
//

std::map<unsigned int, OperationNodeInfo> OperationNodeInfo::DECLARED_OPERATIONS = {  };

void OperationNodeInfo::declareOperations() { }

const OperationNodeInfo& OperationNodeInfo::find(const char* token) {
	return find(hashOperationId(token));
}

const OperationNodeInfo& OperationNodeInfo::find(unsigned int id) {
	return DECLARED_OPERATIONS[id];
}

bool OperationNodeInfo::isDeclared(
	const char* token,
	const OperationNodeInfo** infoStore
) {
	return isDeclared(hashOperationId(token), infoStore);
}

bool OperationNodeInfo::isDeclared(
	unsigned int id,
	const OperationNodeInfo** infoStore
) {
	const auto& iter = (std::map<unsigned int, OperationNodeInfo>::const_iterator)DECLARED_OPERATIONS.find(id);
	if (iter == DECLARED_OPERATIONS.end()) {
		return false;
	}
	if (infoStore) {
		*infoStore = &iter->second;
	}
	return true;
}

unsigned int OperationNodeInfo::findId(
	const char* token,
	const OperationNodeInfo** infoStore
) {
	for (const auto& opInfoEntry : DECLARED_OPERATIONS) {
		if (!strncmp(token, opInfoEntry.second.mToken, opInfoEntry.second.mTokenLen)) {
			if (infoStore) {
				*infoStore = &opInfoEntry.second;
			}
			return opInfoEntry.first;
		}
	}

	return 0;
}

std::ostream& operator<<(std::ostream& out, const OperationNodeInfo& opInfo) {
	return out << opInfo.mToken;
}

//
// ==============================================================
//

//std::map<unsigned int, OperationNode> OperationNode::DEFINED_OPERATIONS = { };
//
//OperationNode::OperationNode() : ExpressionNode() {
//
//}
//
//// http://www.cse.yorku.ca/~oz/hash.html <-- hash algoritm
//OperationNode::OperationNode(const char* token, unsigned char subType) : ExpressionNode(EXPR_NODE_OPERATION, subType) {
//	this->mToken = token;
//	this->mTokenLen = strlen(token);
//
//	this->mOperationId = 5381;
//	
//	for (unsigned char i = 0; token[i]; i++) {
//		this->mOperationId = ((this->mOperationId << 5) + this->mOperationId) + token[i];
//	}
//
//	DEFINED_OPERATIONS[this->mOperationId] = *this;
//}
//
//OperationNode::OperationNode(const char* token, unsigned char precedent, OperationNodeBinaryCallback callback) 
//	: OperationNode(token, OPERATION_NODE_BINARY) {
//	this->mOperationPrecedent = precedent;
//	this->mCallbackData.bin = callback;
//}
//
//OperationNode::OperationNode(const char* token, OperationNodeUnaryCallback callback) 
//	: OperationNode(token, OPERATION_NODE_UNARY) {
//	this->mCallbackData.un = callback;
//}
//
//OperationNode::OperationNode(const char* token, unsigned char functionArgExpect, OperationNodeFunctionCallback callback)
//	: OperationNode(token, OPERATION_NODE_FUNCTION) {
//	this->mOperationFuncArgExpect = functionArgExpect;
//	this->mCallbackData.func = callback;
//}
//
//unsigned int OperationNode::opId() const {
//	return this->mOperationId;
//}
//
//const char* OperationNode::token() const {
//	return this->mToken;
//}
//
//unsigned char OperationNode::tokenLen() const {
//	return this->mTokenLen;
//}
//
//bool OperationNode::isOperationStr(const char* c, ExpressionNode** dynStore) {
//	for (const auto& opInfo : DEFINED_OPERATIONS) {
//		if (!strncmp(c, opInfo.second.mToken, opInfo.second.mTokenLen)) {
//			if (dynStore) {
//				*dynStore = new OperationNode(opInfo.second);
//			}
//			return true;
//		}
//	}
//	return false;
//}
//
//bool OperationNode::isOperationId(unsigned int id, ExpressionNode** dynStore) {
//	const auto iter = DEFINED_OPERATIONS.find(id);
//
//	if (iter != DEFINED_OPERATIONS.end()) {
//		if (dynStore) {
//			*dynStore = new OperationNode(iter->second);
//		}
//		return true;
//	}
//
//	return false;
//}
//
//std::ostream& operator<<(std::ostream& out, const OperationNode& node) {
//	return out << node.mToken;
//}

//
// ==============================================================
//

//std::vector<char> ScopeNode::SCOPE_INC_CHARS = { '[', '(' };
//std::vector<char> ScopeNode::SCOPE_DEC_CHARS = { ']', ')' };
//
//ScopeNode::ScopeNode(unsigned char subType) : ExpressionNode(EXPR_NODE_SCOPE, subType) {
//
//}
//
//ScopeNode::ScopeNode(char scopeChar) 
//	: ScopeNode(
//		(std::find(SCOPE_INC_CHARS.begin(), SCOPE_INC_CHARS.end(), scopeChar) != SCOPE_INC_CHARS.end()) 
//		? SCOPE_NODE_INCREMENT : SCOPE_NODE_DECREMENT
//	)
//{ }
//
//bool ScopeNode::isScopeStr(const char* c, ExpressionNode** dynStore) {
//	if (std::find(SCOPE_INC_CHARS.begin(), SCOPE_INC_CHARS.end(), *c) != SCOPE_INC_CHARS.end()) {
//		if (dynStore) {
//			*dynStore = new ScopeNode(SCOPE_NODE_INCREMENT);
//		}
//		return true;
//	}
//	else if (std::find(SCOPE_DEC_CHARS.begin(), SCOPE_DEC_CHARS.end(), *c) != SCOPE_DEC_CHARS.end()) {
//		if (dynStore) {
//			*dynStore = new ScopeNode(SCOPE_NODE_DECREMENT);
//		}
//		return true;
//	}
//
//	return false;
//}
//
//std::ostream& operator<<(std::ostream& out, const ScopeNode& node) {
//	return out << (node.mSubType == SCOPE_NODE_INCREMENT ? '(' : ')');
//}

//
// ==============================================================
//
