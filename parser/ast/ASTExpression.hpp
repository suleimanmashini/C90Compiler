#pragma once
#include "AST.hpp"

struct ASTExpression : public ASTNode{
public:
  ASTExpression(ASTPrimitive* returnType) : left(returnType)
private:
};
