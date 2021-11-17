#include <iostream>
#include <string>
#include <vector>

#include "_ExpressionNode.h"

#pragma once

class Expression
{
private:
	std::vector<_ExpressionNode> mExpressionNodes;

public:
	Expression(const char* cstr, size_t len);
	Expression(const std::string& str);
	Expression(const Expression& expressionRef);

	size_t nodeCount() const;

	std::vector<_ExpressionNode>::const_iterator begin() const;
	std::vector<_ExpressionNode>::const_iterator end() const;

public:
	Expression& operator=(const Expression& expressionRef);

public:
	friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

 