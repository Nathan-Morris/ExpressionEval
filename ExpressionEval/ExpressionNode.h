#include <iostream>
#include <vector>
#include <map>

#pragma once

#ifdef _EXPR_EVAL_DOUBLE_PRECISION
	typedef double FloatType;
#	define strtoft strtod
#else
	typedef float FloatType;
#	define strtoft strtof
#endif

/*
[Expression Node Types]
- expression node
	- operation
		- binary
		- unary
		- function?
	- operand
		- number
		- variable
	- scope
		- increment
		- decrement
*/

//
// ==============================================================
//

enum _ExpressionNodeType : unsigned char {
	EXPR_NODE_OPERATION,
	EXPR_NODE_OPERAND,
	EXPR_NODE_SCOPE
};

enum OperationNodeType : unsigned char {
	OPERATION_NODE_BINARY,
	OPERATION_NODE_UNARY,
	OPERATION_NODE_FUNCTION
};


enum OperandNodeType : unsigned char {
	OPERAND_NODE_VALUE,
	OPERAND_NODE_VARIABLE
};

enum ScopeNodeType : unsigned char {
	SCOPE_NODE_INCREMENT,
	SCOPE_NODE_DECREMENT
};

//
// ==============================================================
//

// (err code, err msg)
typedef void(*OperationNodeFunctionErrorCallback)(int, const char*);

// (a, b)
typedef FloatType(*OperationNodeBinaryCallback)(FloatType, FloatType);

// (a)
typedef FloatType(*OperationNodeUnaryCallback)(FloatType);

// (err callback, n count, ...n)
typedef FloatType(*OperationNodeFunctionCallback)(OperationNodeFunctionErrorCallback, unsigned char, ...);

class OperationNodeInfo {
private:
	union OperationNodeCallbackData {
		OperationNodeBinaryCallback bin = NULL;
		OperationNodeUnaryCallback un;
		OperationNodeFunctionCallback func;
	} mCallbackData;

	const char* mToken = NULL;
	unsigned char mTokenLen = 0;

	unsigned char mOperationPrecedent = 0xFF;
	unsigned char mOperationFuncArgExpect = 0;

	unsigned char mCallbackType = 0;

	OperationNodeInfo(
		const char* token,
		void* callback,
		unsigned char callbackType,
		unsigned char precedent,
		unsigned char funcArgExpect
	);

public:
	OperationNodeInfo(); // dont use

	OperationNodeInfo(
		const char* token,
		unsigned char precedent,
		OperationNodeBinaryCallback callback
	);

	OperationNodeInfo(
		const char* token,
		OperationNodeUnaryCallback callback
	);

	OperationNodeInfo(
		const char* token,
		unsigned char funcArgExpect,
		OperationNodeFunctionCallback callback
	);

public:
	unsigned int opId() const;
	const char* token() const;
	unsigned char tokenLen() const;
	unsigned char type() const;
	unsigned char precedent() const;

	OperationNodeBinaryCallback binary() const;
	OperationNodeUnaryCallback unary() const;
	OperationNodeFunctionCallback function() const;

private:
	static std::map<unsigned int, OperationNodeInfo> DECLARED_OPERATIONS;

	static void declareOperations();

	static constexpr inline unsigned int hashOperationId(const char* token) {
		if (!token) {
			return 0;
		}
		unsigned int operationId = 5381;
		for (unsigned char i = 0; token[i]; i++) {
			operationId = ((operationId << 5) + operationId) + token[i];
		}
		return operationId;
	}

public:

	template<typename ...OpRest>
	static void declareOperations(const OperationNodeInfo& opInfo, const OpRest& ...rest) {
		declareOperations(rest...);
	}

	static const OperationNodeInfo& find(const char* token);
	static const OperationNodeInfo& find(unsigned int id);

	static bool isDeclared(
		const char* token,
		const OperationNodeInfo** infoStore = NULL
	);
	static bool isDeclared(
		unsigned int id,
		const OperationNodeInfo** infoStore = NULL
	);

	static constexpr unsigned int idOfToken(const char* token) {
		return hashOperationId(token);
	}

	// returns 0 if not found, im "pretty" sure no input to djb2 I
	// will provide results in 0 being returned. *pretty sure*
	static unsigned int findId(
		const char* token, 
		const OperationNodeInfo** infoStore = NULL
	);

public:
	
	friend std::ostream& operator<<(std::ostream& out, const OperationNodeInfo& opInfo);
};

//
// ==============================================================
//

#define OPERAND_NODE_VAR_CHAR	((char)'x')

class ExpressionNode {
public:

	unsigned char mType : 4;		// _ExpressionNodeType direct values
	unsigned char mSubType : 4;		// sub type derived from mType types

	union ExpressionNodeData {
		struct OperationNodeData {
			unsigned int id;
		} operation;

		union OperandNodeData {
			FloatType value;
			char variable;
		} operand;
	} mData;						// stores data for mSubType usages

	ExpressionNode(unsigned char type, unsigned char subType);

public:
	ExpressionNode();
	ExpressionNode(FloatType value);
	ExpressionNode(char variableOrScope);
	ExpressionNode(const OperationNodeInfo& opInfo);

public:
	unsigned char type() const;
	unsigned char subType() const;

	FloatType value() const;
	char variable() const;

	const OperationNodeInfo& operation() const;

public:
	// returns size of parsed token, if none is found it returns '0'
	static size_t parseAt(const char* at, ExpressionNode& store);

public:
	friend std::ostream& operator<<(std::ostream& out, const ExpressionNode& node);
};

//
// ==============================================================
//


//class OperandNode : public ExpressionNode {
//private:
//	union OperandNodeData {
//		FloatType value;
//		char variable;
//	} mData;
//
//	OperandNode(unsigned char subType);
//
//public:
//	OperandNode(FloatType value);
//	OperandNode(char variable);
//
//	FloatType value() const;
//	char variable() const;
//
//public:
//	friend std::ostream& operator<<(std::ostream& out, const OperandNode& node);
//};

//
// ==============================================================
//

//class OperationNode : public ExpressionNode {
//private:
//	static std::map<unsigned int, OperationNode> DEFINED_OPERATIONS;
//
//private:
//	static void defineOperations();
//
//private:
//
//	union OperationNodeCallbackData {
//		OperationNodeBinaryCallback bin;
//		OperationNodeUnaryCallback un;
//		OperationNodeFunctionCallback func;
//	} mCallbackData;
//
//	const char* mToken;				// string representation of the operation, eg. "+" "sin"
//	unsigned char mTokenLen;	
//
//	unsigned int mOperationId;
//	unsigned char mOperationPrecedent = 0xFF;
//	unsigned char mOperationFuncArgExpect = 0;
//
//	OperationNode(const char* token, unsigned char subType);
//
//public:
//	OperationNode();
//
//	OperationNode(
//		const char* token,
//		unsigned char precedent,
//		OperationNodeBinaryCallback callback
//	);
//
//	OperationNode(
//		const char* token,
//		OperationNodeUnaryCallback callback
//	);
//
//	OperationNode(
//		const char* token,
//		unsigned char functionArgExpect,
//		OperationNodeFunctionCallback callback
//	);
//
//	unsigned int opId() const;
//	const char* token() const;
//	unsigned char tokenLen() const;
//
//
//public:
//	template<typename ...VarRest>
//	static void defineOperations(const OperationNode& opNode, const VarRest& ...rest) {
//		OperationNode::DEFINED_OPERATIONS[opNode.mOperationId] = opNode;
//		OperationNode::defineOperations(rest...);
//	}
//
//	static bool isOperationStr(const char* c, ExpressionNode** dynStore = NULL);
//	static bool isOperationId(unsigned int id, ExpressionNode** dynStore = NULL);
//
//public:
//	friend std::ostream& operator<<(std::ostream& out, const OperationNode& node);
//};

//
// ==============================================================
//

//enum ScopeNodeType : unsigned char {
//	SCOPE_NODE_INCREMENT,
//	SCOPE_NODE_DECREMENT
//};
//
//class ScopeNode : public ExpressionNode {
//private:
//	static std::vector<char> SCOPE_INC_CHARS;
//	static std::vector<char> SCOPE_DEC_CHARS;
//
//private:
//	ScopeNode(unsigned char subType);
//
//public:
//	ScopeNode(char scopeChar);
//
//public:
//	static bool isScopeStr(const char* c, ExpressionNode** dynStore = NULL);
//
//public:
//	friend std::ostream& operator<<(std::ostream& out, const ScopeNode& node);
//};
