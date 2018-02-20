#pragma once
#include "ASTNode.hpp"

struct ASTreturnStatement : public ASTNode{
public:
	ASTreturnStatement(ASTExpression* expressionIn): left(expressionIn);
private:
};
