#pragma once

#include "ASTNode.hpp"
#include "ASTPrimitive.hpp"

struct ASTExpression : public ASTNode {
public:
  ASTExpression(ASTPrimitive* returnType) : left(returnType)
private:
  ASTNode* left = nullptr;
};
