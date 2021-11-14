#include "ExpressionOperation.h"

#pragma once

enum class ExpressionNodeType : unsigned char
{
	VALUE,		// floating point numbers
	VARIABLE,	// variables, 'x'
	SCOPE_INC,	// ( [
	SCOPE_DEC,	// ) ]
	OPERATION,	// + - * sin cos
};

class ExpressionNode
{
private:
	ExpressionNodeType mType;

	union ExpressionNodeData {
		float value;
		OperationId opId;
		char varChar;
		
		inline ExpressionNodeData() { }
	} mData;

public:
	ExpressionNode(ExpressionNodeType type, OperationId opId);
	ExpressionNode(float value);
	ExpressionNode(char c);

public:
	friend std::ostream& operator<<(std::ostream& out, const ExpressionNode& node);
};

