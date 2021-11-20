#include "Expression.h"

ExpressionBranch::ExpressionBranch(const ExpressionNode& node, ExpressionBranch* left, ExpressionBranch* right)
	: mNode(node), mLeft(left), mRight(right) { }

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

//
// ==============================================================
//

Expression::Expression(const char* cstr, size_t len) {
	ExpressionNode nodeFound;

	for (size_t i = 0; i != len; i++) {
		const char& c = cstr[i];
		size_t tokenInc = ExpressionNode::parseAt(&c, nodeFound);
		if (tokenInc) {
			this->mExpressionNodes.push_back(nodeFound);
			i += tokenInc - 1;
		}
	}
}

Expression::Expression(const std::string& str) : Expression(str.c_str(), str.length()) {

}

size_t Expression::nodeCount() const {
	return this->mExpressionNodes.size();
}

std::vector<ExpressionNode>::const_iterator Expression::begin() const {
	return this->mExpressionNodes.begin();
}

std::vector<ExpressionNode>::const_iterator Expression::end() const {
	return this->mExpressionNodes.end();
}

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
	for (const auto& node : expr.mExpressionNodes) {
		out << node;
	}
	return out;
}

