#include "Expression.h"

Expression::Expression(const char* cstr, size_t len) {
	const void* foundPtr;

	for (size_t i = 0; i < len; i++) {
		const char c = cstr[i];

		if (BinaryOperationInfo::findOperation(&c, &foundPtr)) {
			this->mExpressionNodes.push_back(
				ExpressionNode(ExpressionNodeType::BIN_OP, ((BinaryOperationInfo*)foundPtr)->opId())
			);
			i += ((BinaryOperationInfo*)foundPtr)->tokenLen() - 1;
		}
		else if (UnaryOperationInfo::findOperation(&c, &foundPtr)) {
			this->mExpressionNodes.push_back(
				ExpressionNode(ExpressionNodeType::UN_OP, ((UnaryOperationInfo*)foundPtr)->opId())
			);
			i += ((UnaryOperationInfo*)foundPtr)->tokenLen() - 1;
		}
	}

}

Expression::Expression(const std::string& str) : Expression(str.c_str(), str.length()) {

}

