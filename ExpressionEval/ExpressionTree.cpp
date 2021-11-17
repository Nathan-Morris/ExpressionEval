#include "ExpressionTree.h"

ExpressionTreeNode::ExpressionTreeNode(
	const ExpressionNode& node, 
	ExpressionTreeNode* left, 
	ExpressionTreeNode* right
) : mNode(node), mLeft(left), mRight(right) { }

const ExpressionNode& ExpressionTreeNode::node() const {
	return this->mNode;
}

ExpressionNode& ExpressionTreeNode::node() {
	return this->mNode;
}

ExpressionTreeNode* ExpressionTreeNode::left() const {
	return this->mLeft;
}

ExpressionTreeNode* ExpressionTreeNode::right() const {
	return this->mRight;
}

ExpressionTreeNode* ExpressionTreeNode::copy() const {
	return new ExpressionTreeNode(
		this->mNode, 
		this->mLeft ? this->mLeft->copy() : NULL, 
		this->mRight ? this->mRight->copy() : NULL
	);
}

void ExpressionTreeNode::print(std::ostream& out, unsigned short depth) {
	for (unsigned short i = 0; i != depth; i++) {
		out << '\t';
	}
	out << this->mNode;
	out << std::endl;

	if (this->mLeft) {
		mLeft->print(out, depth + 1);
	}
	if (this->mRight) {
		mRight->print(out, depth + 1);
	}
}



//
//
//

ExpressionTree::ExpressionTree(const Expression& expr) {
	std::vector<ExpressionNode> postNodeList;
	std::stack<ExpressionNode> postNodeStack;
	std::stack<ExpressionTreeNode*> treeNodeStack;
	ExpressionTreeNode* leafL, * leafR;
	bool unaryPopped = false;

	for (const ExpressionNode& node : expr) {
		switch (node.type())
		{
		case ExpressionNodeType::VALUE:
		case ExpressionNodeType::VARIABLE:
			postNodeList.push_back(node);
			break;

		case ExpressionNodeType::SCOPE_INC:	
			postNodeStack.push(node);
			break;

		case ExpressionNodeType::SCOPE_DEC:
			while (!postNodeStack.empty() && postNodeStack.top().type() != ExpressionNodeType::SCOPE_INC) {
				postNodeList.push_back(postNodeStack.top());
				postNodeStack.pop();
			}
			postNodeStack.pop();
			break;
				
		case ExpressionNodeType::OPERATION:
			if (node.opId().type() == OperationIdOpType::UNARY) {
				if (!postNodeStack.empty() && postNodeStack.top().opId().type() == OperationIdOpType::UNARY) {
					postNodeStack.pop();
				}
				postNodeStack.push(node);
			}
			// binary
			else {
				unaryPopped = false;
				while (
					!postNodeStack.empty()
					&& postNodeStack.top().type() != ExpressionNodeType::SCOPE_INC
					&& postNodeStack.top().opId().precedent() >= node.opId().precedent()
				) {
					if (postNodeStack.top().opId().type() == OperationIdOpType::UNARY) {
						if (unaryPopped) {
							break;
						}
						unaryPopped = true;
					}

					postNodeList.push_back(postNodeStack.top());
					postNodeStack.pop();
				}

				postNodeStack.push(node);
			}
			break;
		}
	}

	while (!postNodeStack.empty()) {
		postNodeList.push_back(postNodeStack.top());
		postNodeStack.pop();
	}

	for (const ExpressionNode& node : postNodeList) {
		switch (node.type())
		{
		case ExpressionNodeType::VALUE:
		case ExpressionNodeType::VARIABLE:
			treeNodeStack.push(new ExpressionTreeNode(node, NULL, NULL));
			break;

		case ExpressionNodeType::OPERATION:
			leafL = treeNodeStack.top();
			treeNodeStack.pop();

			if (node.opId().type() == OperationIdOpType::UNARY) {
				treeNodeStack.push(new ExpressionTreeNode(node, leafL, NULL));
			}
			else {
				leafR = treeNodeStack.top();
				treeNodeStack.pop();

				treeNodeStack.push(new ExpressionTreeNode(node, leafR, leafL));
			}
			break;
		}
	}

	this->root = treeNodeStack.top();
}

float ExpressionTree::solve(ExpressionTreeNode* root) {
	const ExpressionOperationInfo* opInfoPtr;
	
	switch (root->node().type())
	{
	case ExpressionNodeType::VALUE:
		return root->node().value();
	
	case ExpressionNodeType::OPERATION:
		if (ExpressionOperationInfo::findOperation(root->node().opId(), &opInfoPtr)) {
			switch (opInfoPtr->opId().type())
			{
			case OperationIdOpType::BINARY:
				opInfoPtr->binaryCallback()(this->solve(root->left()), this->solve(root->right()));
				break;

			case OperationIdOpType::UNARY:
				opInfoPtr->unaryCallback()(this->solve(root->left()));
				break;
			}
		}
		else {
			return 0.f;
		}
	}
}

float ExpressionTree::solve() {
	ExpressionTreeNode* rootCopy = this->root->copy();
	return this->solve(rootCopy);
}

std::ostream& operator<<(std::ostream& out, const ExpressionTree& tree) {
	tree.root->print(out);
	return out;
}