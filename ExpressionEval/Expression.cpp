#include "Expression.h"

Expression::Expression(const char* cstr, size_t len) {
	const ExpressionOperationInfo* opPtr;
	char* endPtr;

	for (size_t i = 0; i < len; i++) {
		const char& c = cstr[i];

		// to dynamic scope token setting
		if (ExpressionOperationInfo::findOperation(&c, &opPtr)) {
			this->mExpressionNodes.push_back(
				ExpressionNode(ExpressionNodeType::OPERATION, opPtr->opId())
			);
		}
		else if (c == '(' || c == ')' || c == '[' || c == ']' || c == 'x') {
			this->mExpressionNodes.push_back(
				ExpressionNode(c)
			);
		}
		else if (isdigit(c) || c == '.') {
			this->mExpressionNodes.push_back(
				ExpressionNode(strtof(&c, &endPtr))
			);
			
			i += (size_t)(endPtr - &c) - 1;
		}
	}

}

Expression::Expression(const std::string& str) : Expression(str.c_str(), str.length()) {

}

size_t Expression::nodeCount() const {
	return this->mExpressionNodes.size();
}

std::ostream& operator<<(std::ostream& out, const Expression& expr) {
	for (const auto& node : expr.mExpressionNodes) {
		out << node;
	}
	return out;
}

