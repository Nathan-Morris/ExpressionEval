#include <iostream>
#include <string>
#include <vector>

#include "ExpressionNode.h"

#pragma once

class Expression
{
private:
	std::vector<ExpressionNode> mExpressionNodes;

public:
	Expression(const char* cstr, size_t len);
	Expression(const std::string& str);
	Expression(const Expression& expressionRef);

	size_t nodeCount() const;

public:
	Expression& operator=(const Expression& expressionRef);

public:
	friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

 