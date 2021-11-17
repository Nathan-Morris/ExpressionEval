#include "Expression.h"

#include <stack>

#pragma once

class ExpressionTreeNode {
private:
	ExpressionNode mNode;
	ExpressionTreeNode* mLeft;
	ExpressionTreeNode* mRight;

public:
	ExpressionTreeNode(const ExpressionNode& node, ExpressionTreeNode* left, ExpressionTreeNode* right);

	const ExpressionNode& node() const;
	ExpressionNode& node();

	ExpressionTreeNode* left() const;
	ExpressionTreeNode* right() const;

	void print(std::ostream& out, unsigned short depth = 0);
};

class ExpressionTree {
private:
	ExpressionTreeNode* root;

public:
	ExpressionTree(const Expression& expr);
	ExpressionTree(const ExpressionTree& exprTree);

public:
	friend std::ostream& operator<<(std::ostream& out, const ExpressionTree& tree);
};