#include "Expression.h"

#include <stack>

#pragma once

class ExpressionTreeNode {
private:
	ExpressionNode node;
	ExpressionTreeNode* left;
	ExpressionTreeNode* right;

public:
	ExpressionTreeNode(const ExpressionNode& node, ExpressionTreeNode* left, ExpressionTreeNode* right);
};

class ExpressionTree {
private:
	ExpressionTreeNode* root;

public:
	ExpressionTree(const Expression& expr);
};