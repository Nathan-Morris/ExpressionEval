#include "ExpressionTree.h"

ExpressionTreeNode::ExpressionTreeNode(
	const ExpressionNode& node, 
	ExpressionTreeNode* left, 
	ExpressionTreeNode* right
) : node(node), left(left), right(right) { }

ExpressionTree::ExpressionTree(const Expression& expr) {
	std::vector<ExpressionNode> postNodeList;
	std::stack<ExpressionNode> postNodeStack;
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


	for (ExpressionNode& node : postNodeList) {
		std::cout << node;
	}
}