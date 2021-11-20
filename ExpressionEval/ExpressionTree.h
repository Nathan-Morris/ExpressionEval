#include "Expression.h"

#include <stack>

#pragma once

class ExpressionTreeNode {

};

class ExpressionTree {
private:
	_ExpressionNode mNode;
	ExpressionTreeNode* mLeft;
	ExpressionTreeNode* mRight;

public:
	ExpressionTree(const Expression& expression);

	~ExpressionTree();

public:

	_ExpressionNode& node();
	const _ExpressionNode& node() const;

	ExpressionTree fromLeft() const;
	ExpressionTree fromRight() const;
};