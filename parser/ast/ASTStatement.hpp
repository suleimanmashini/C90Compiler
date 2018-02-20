#pragma once
#include "AST.hpp"

struct ASTreturnStatement : public ASTNode{
public:
	ASTreturnStatement(ASTExpression* expressionIn): left(expressionIn);
private:
};
