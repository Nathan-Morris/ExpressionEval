#include <iostream>
#include <string>
#include <vector>

#include "ExpressionNode.h"

#pragma once

class ExpressionBranch {
private:
	ExpressionNode mNode;
	ExpressionBranch* mLeft;
	ExpressionBranch* mRight;

public:
	ExpressionBranch(const ExpressionNode& node, ExpressionBranch* left, ExpressionBranch* right);

	ExpressionNode& node();
	const ExpressionNode& node() const;

	ExpressionBranch* left() const;
	ExpressionBranch* right() const;
};

class Expression
{
private:
	ExpressionBranch* mRoot = NULL;
	std::vector<ExpressionNode> mExpressionNodes;

public:
	Expression(const char* cstr, size_t len);
	Expression(const std::string& str);

	size_t nodeCount() const;

	std::vector<ExpressionNode>::const_iterator begin() const;
	std::vector<ExpressionNode>::const_iterator end() const;

public:
	friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

 