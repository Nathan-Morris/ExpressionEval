#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

#include "ExpressionNode.h"

#pragma once

class ExpressionBranch {
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
	Expression(const char* cstr, size_t len);
	Expression(const std::string& str);
	~Expression();

	Expression& parse(const char* cstr, size_t len);
	Expression& parse(const std::string& str);

	FloatType solve(const std::map<char, FloatType>& variableMap) const;
	FloatType solve() const;

public:
	Expression& operator=(const Expression& e);

public:
	friend std::ostream& operator<<(std::ostream& out, const Expression& expr);
};

 