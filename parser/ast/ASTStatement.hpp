#pragma once
#include "AST.hpp"

struct ASTreturnStatement : public ASTNode{
public:
	ASTreturnStatement() {};
	ASTreturnStatement(ASTExpression* expressionIn) {
		returnExp = expressionIn;
	};
private:
	ASTExpression* returnExp = nullptr;
};

