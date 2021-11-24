#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

#include "ExpressionNode.h"

#pragma once

// shitty hack
#if defined(_WIN64) || defined(__x84_64__) || defined(__ppc64__)
#	define EXPR_BRANCH_TEMPLATE_HOLDER_LEFT			((ExpressionBranch*)(0xFFFFFFFFFFFFFFFF))
#	define EXPR_BRANCH_TEMPLATE_HOLDER_RIGHT		((ExpressionBranch*)(0xFFFFFFFFFFFFFFFF - 1))
#	define EXPR_BRANCH_TEMPLATE_HOLDER_DER_LEFT		((ExpressionBranch*)(0xFFFFFFFFFFFFFFFF - 2))
#	define EXPR_BRANCH_TEMPLATE_HOLDER_DER_RIGHT	((ExpressionBranch*)(0xFFFFFFFFFFFFFFFF - 3))
#else
#	define EXPR_BRANCH_TEMPLATE_HOLDER_LEFT			((ExpressionBranch*)(0xFFFFFFFF))
#	define EXPR_BRANCH_TEMPLATE_HOLDER_RIGHT		((ExpressionBranch*)(0xFFFFFFFF - 1))
#	define EXPR_BRANCH_TEMPLATE_HOLDER_DER_LEFT		((ExpressionBranch*)(0xFFFFFFFF - 2))
#	define EXPR_BRANCH_TEMPLATE_HOLDER_DER_RIGHT	((ExpressionBranch*)(0xFFFFFFFF - 3))
#endif

class ExpressionBranch {
private:
	static std::map<unsigned int, ExpressionBranch*> EXPRESSION_COMMON_DERIVATIVES;

private:
	ExpressionNode mNode;
	ExpressionBranch* mLeft = NULL;
	ExpressionBranch* mRight = NULL;

public:
	void print(std::ostream& out, unsigned int depth = 0) const;
	FloatType solve(const std::map<char, FloatType>& variableMap) const;

public:
	ExpressionBranch(const ExpressionNode& node, ExpressionBranch* left, ExpressionBranch* right);
	ExpressionBranch(const ExpressionBranch& exprBranch);
	~ExpressionBranch();

	ExpressionNode& node();
	const ExpressionNode& node() const;

	ExpressionBranch* left() const;
	ExpressionBranch* right() const;

	ExpressionBranch* derivative() const; 

public:
	ExpressionBranch& operator=(const ExpressionBranch& branch);

public:
	friend std::ostream& operator<<(std::ostream& out, const ExpressionBranch& branch);
};

class Expression
{
private:
	ExpressionBranch* mRoot = NULL;

public:
	Expression();
	Expression(const Expression& expr);
	Expression(const ExpressionBranch* branch);
	Expression(const char* cstr, size_t len);
	Expression(const std::string& str);
	~Expression();

	Expression& parse(const char* cstr, size_t len);
	Expression& parse(const std::string& str);

	FloatType solve(const std::map<char, FloatType>& variableMap) const;
	FloatType solve() const;
	Expression derivative() const;

public:
	Expression& operator=(const Expression& e);

public:
	friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

 