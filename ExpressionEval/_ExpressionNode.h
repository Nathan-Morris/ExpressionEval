#include <iostream>
#include <vector>

#pragma once

#ifdef _EXPR_EVAL_DOUBLE_PRECISION
	typedef double FloatType;
#else
	typedef float FloatType;
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

class _ExpressionNode {
protected:

	unsigned char mType : 4;		// _ExpressionNodeType direct values
	unsigned char mSubType : 4;		// sub type derived from mType types

	_ExpressionNode(unsigned char type, unsigned char subType);

public:
	unsigned char type();
	unsigned char subType();
};

//
// ==============================================================
//

enum OperandNodeType : unsigned char {
	OPERAND_NODE_VALUE,
	OPERAND_NODE_VARIABLE
};

class OperandNode : public _ExpressionNode {
private:
	union OperandNodeData {
		FloatType value;
		char variable;
	} mData;

	OperandNode(unsigned char subType);

public:
	OperandNode(FloatType value);
	OperandNode(char variable);

	FloatType value();
	char variable();
};

//
// ==============================================================
//

enum OperationNodeType : unsigned char {
	OPERATION_NODE_BINARY,
	OPERATION_NODE_UNARY,
	OPERATION_NODE_FUNCTION
};



// (err code, err msg)
typedef void(*OperationNodeFunctionErrorCallback)(int, const char*);

// (a, b)
typedef FloatType(*OperationNodeBinaryCallback)(FloatType, FloatType);

// (a)
typedef FloatType(*OperationNodeUnaryCallback)(FloatType);

// (err callback, n count, ...n)
typedef FloatType(*OperationNodeFunctionCallback)(void(*)(int, const char*), unsigned char, ...);



class OperationNode : public _ExpressionNode {
private:

	static unsigned short LAST_OPERATION_ID;

private:

	union OperationNodeCallbackData {
		OperationNodeBinaryCallback bin;
		OperationNodeUnaryCallback un;
		OperationNodeFunctionCallback func;
	} mCallbackData;

	const char* mToken;				// string representation of the operation, eg. "+" "sin"
	unsigned char mTokenLen;	

	unsigned short mOperationId;
	unsigned char mOperationPrecedent = 0xFF;
	unsigned char mOperationFuncArgExpect = 0;

	OperationNode(const char* token, unsigned char subType);

public:
	OperationNode(
		const char* token,
		unsigned char precedent,
		OperationNodeBinaryCallback callback
	);

	OperationNode(
		const char* token,
		OperationNodeUnaryCallback callback
	);

	OperationNode(
		const char* token,
		unsigned char functionArgExpect,
		OperationNodeFunctionCallback callback
	);
};

//
// ==============================================================
//

enum ScopeNodeType : unsigned char {
	SCOPE_NODE_INCREMENT,
	SCOPE_NODE_DECREMENT
};

class ScopeNode : public _ExpressionNode {
private:
	static std::vector<char> SCOPE_INC_CHARS;
	static std::vector<char> SCOPE_DEC_CHARS;

private:
	ScopeNode(unsigned char subType);

public:
	ScopeNode(char scopeChar);


};
