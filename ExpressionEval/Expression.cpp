#include "Expression.h"

ExpressionBranch::ExpressionBranch(const ExpressionNode& node, ExpressionBranch* left, ExpressionBranch* right)
	: mNode(node), mLeft(left), mRight(right) { }

ExpressionBranch::ExpressionBranch(const ExpressionBranch& exprBranch) {
	this->mNode = exprBranch.mNode;

	if (exprBranch.mLeft) {
		this->mLeft = new ExpressionBranch(*exprBranch.mLeft);
	}

	if (exprBranch.mRight) {
		this->mRight = new ExpressionBranch(*exprBranch.mRight);
	}
}

ExpressionBranch::~ExpressionBranch() {
	if (this->mLeft) {
		this->mLeft->~ExpressionBranch();
		delete this->mLeft;
		this->mLeft = NULL;
	}

	if (this->mRight) {
		this->mRight->~ExpressionBranch();
		delete this->mRight;
		this->mRight = NULL;
	}
}

ExpressionNode& ExpressionBranch::node() {
	return this->mNode;
}

const ExpressionNode& ExpressionBranch::node() const {
	return this->mNode;
}

ExpressionBranch* ExpressionBranch::left() const {
	return this->mLeft;
}

ExpressionBranch* ExpressionBranch::right() const {
	return this->mRight;
};

ExpressionBranch& ExpressionBranch::operator=(const ExpressionBranch& exprBranch) {
	this->mNode = exprBranch.mNode;
	if (exprBranch.mLeft) {
		this->mLeft = new ExpressionBranch(*exprBranch.mLeft);
	}
	if (exprBranch.mRight) {
		this->mRight = new ExpressionBranch(*exprBranch.mRight);
	}
	return *this;
}


void ExpressionBranch::print(std::ostream& out, unsigned int depth) const {
	for (unsigned int i = 0; i != depth; i++) {
		out << '\t';
	}

	out << this->mNode << std::endl;

	if (this->mLeft) {
		this->mLeft->print(out, depth + 1);
	}

	if (this->mRight) {
		this->mRight->print(out, depth + 1);
	}
}

FloatType ExpressionBranch::solve(const std::map<char, FloatType>& variableMap) const {
	if (this->mNode.type() == EXPR_NODE_OPERAND) {
		if (this->mNode.subType() == OPERAND_NODE_VALUE) {
			return this->mNode.value();
		}

		auto iter = variableMap.find(this->mNode.variable());

		if (iter == variableMap.end()) {
			throw std::runtime_error("Unable To Solve For Variable");
		}

		return iter->second;
	}

	if (this->mNode.type() == EXPR_NODE_OPERATION) {
		if (this->mNode.subType() == OPERATION_NODE_BINARY) {
			return this->mNode.operation().binary()(
				this->mLeft->solve(variableMap),
				this->mRight->solve(variableMap)
			);
		}
		else if (this->mNode.subType() == OPERATION_NODE_UNARY) {
			return this->mNode.operation().unary()(
				this->mLeft->solve(variableMap)
			);
		}
	}
}

std::ostream& operator<<(std::ostream& out, const ExpressionBranch& branch) {
	branch.print(out);
	return out;
}

//
// ==============================================================
//

Expression::Expression() {

}

Expression::Expression(const char* cstr, size_t len) {
	this->parse(cstr, len);
}

Expression::Expression(const std::string& str) : Expression(str.c_str(), str.length()) { }

Expression::~Expression() { 
	if (this->mRoot) {
		delete this->mRoot; // calls de-constructor
	}

	this->mRoot = NULL;
}

Expression& Expression::parse(const char* cstr, size_t len) {
	this->~Expression();

	// init
	std::vector<ExpressionNode> nodes;
	// post
	std::vector<ExpressionNode> postList;
	std::stack<ExpressionNode> operatorStack;
	bool unaryPopped;
	// tree
	std::stack<ExpressionBranch*> treeOperandStack;
	ExpressionBranch* left, * right;

	//
	// Initial Parsing
	//

	ExpressionNode nodeFound;

	for (size_t i = 0; i != len; i++) {
		const char& c = cstr[i];
		size_t tokenInc = ExpressionNode::parseAt(&c, nodeFound);

		if (tokenInc) {
			nodes.push_back(nodeFound);
			i += tokenInc - 1;
		}
	}

	//
	// Shunting Yard to post fix
	//

	postList.reserve(nodes.size());

	for (size_t i = 0; i != nodes.size(); i++) {
		ExpressionNode& node = nodes.at(i);

		switch (node.type())
		{
		case EXPR_NODE_OPERAND:
			postList.push_back(node);
			break;

		case EXPR_NODE_OPERATION:
			switch (node.subType())
			{
			case OPERATION_NODE_BINARY:
				unaryPopped = false;
				while (
					operatorStack.size()
					&& !(operatorStack.top().type() == EXPR_NODE_SCOPE && operatorStack.top().subType() == SCOPE_NODE_INCREMENT)
					&& (operatorStack.top().type() != EXPR_NODE_OPERATION || operatorStack.top().operation().precedent() >= node.operation().precedent())
					) {
					if (operatorStack.top().type() == EXPR_NODE_OPERATION && operatorStack.top().subType() == OPERATION_NODE_UNARY) {
						if (unaryPopped) {
							break;
						}

						unaryPopped = true;
					}

					postList.push_back(operatorStack.top());
					operatorStack.pop();
				}
				operatorStack.push(node);
				break;

			case OPERATION_NODE_UNARY:
				if (operatorStack.size() && (operatorStack.top().type() == EXPR_NODE_OPERATION && operatorStack.top().subType() == OPERATION_NODE_UNARY)) {
					operatorStack.pop();
				}
				operatorStack.push(node);
				break;

			case OPERATION_NODE_FUNCTION: // not yet implemented
				break;
			}

			break;

		case EXPR_NODE_SCOPE:
			switch (node.subType())
			{
			case SCOPE_NODE_INCREMENT:
				operatorStack.push(node);
				break;

			case SCOPE_NODE_DECREMENT:
				while (
					operatorStack.size()
					&& !(operatorStack.top().type() == EXPR_NODE_SCOPE && operatorStack.top().subType() == SCOPE_NODE_INCREMENT)
					) {
					postList.push_back(operatorStack.top());
					operatorStack.pop();
				}
				if (operatorStack.size()) {
					operatorStack.pop();
				}
				break;
			}
			break;
		}
	}

	while (operatorStack.size()) {
		postList.push_back(operatorStack.top());
		operatorStack.pop();
	}

	nodes = postList;

	//
	// Expression Tree Generation
	//

	for (ExpressionNode& node : nodes) {
		switch (node.type())
		{
		case EXPR_NODE_OPERAND:
			treeOperandStack.push(new ExpressionBranch(node, NULL, NULL));
			break;

		case EXPR_NODE_OPERATION:
			switch (node.subType())
			{
			case OPERATION_NODE_BINARY:
				right = treeOperandStack.top();
				treeOperandStack.pop();

				left = treeOperandStack.top();
				treeOperandStack.pop();

				treeOperandStack.push(new ExpressionBranch(node, left, right));
				break;

			case OPERATION_NODE_UNARY:
				left = treeOperandStack.top();
				treeOperandStack.pop();

				treeOperandStack.push(new ExpressionBranch(node, left, NULL));
				break;

			case OPERATION_NODE_FUNCTION: // not yet implemented
				break;
			}
			break;
		}
	}

	this->mRoot = treeOperandStack.top();

	return *this;
}

Expression& Expression::parse(const std::string& str) {
	return this->parse(str.c_str(), str.size());
}

FloatType Expression::solve(const std::map<char, FloatType>& variableMap) const {
	return this->mRoot->solve(variableMap);
}

FloatType Expression::solve() const {
	return this->mRoot->solve({});
}

Expression& Expression::operator=(const Expression& e) {
	this->~Expression();

	if (e.mRoot) {
		this->mRoot = new ExpressionBranch(*e.mRoot);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
	return out;
}

