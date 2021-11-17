#include "Expression.h"

Expression::Expression(const char* cstr, size_t len) {

}

Expression::Expression(const std::string& str) : Expression(str.c_str(), str.length()) {

}

size_t Expression::nodeCount() const {
	return this->mExpressionNodes.size();
}

std::vector<_ExpressionNode>::const_iterator Expression::begin() const {
	return this->mExpressionNodes.begin();
}

std::vector<_ExpressionNode>::const_iterator Expression::end() const {
	return this->mExpressionNodes.end();
}

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
	for (const auto& node : expr.mExpressionNodes) {
		//out << node;
	}
	return out;
}

