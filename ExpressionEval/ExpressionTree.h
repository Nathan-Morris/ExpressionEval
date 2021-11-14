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

	ExpressionTreeNode* copy() const;

	void print(std::ostream& out, unsigned short depth = 0);
};

class ExpressionTree {
private:
	ExpressionTreeNode* root;

	float solve(ExpressionTreeNode* root);

public:
	ExpressionTree(const Expression& expr);

	float solve();

public:
	friend std::ostream& operator<<(std::ostream& out, const ExpressionTree& tree);
};