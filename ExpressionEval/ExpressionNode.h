#include "ExpressionOperation.h"

#pragma once

enum class ExpressionNodeType : unsigned char
{
	VALUE,		// floating point numbers
	VARIABLE,	// variables, 'x'
	SCOPE_INC,	// ( [
	SCOPE_DEC,	// ) ]
	BIN_OP,		// + -
	UN_OP		// cos sin
};

class ExpressionNode
{
private:
	ExpressionNodeType mType;

	union ExpressionNodeData {
		float value;
		OperationId opId;
		char varChar;
	} mData;

public:
	ExpressionNode(ExpressionNodeType type, OperationId opId);
	ExpressionNode(float value);
	ExpressionNode(char c);
};

