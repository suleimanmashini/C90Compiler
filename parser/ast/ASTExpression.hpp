#pragma once

#include "ASTNode.hpp"

struct ASTExpression : public ASTNode {
public:
  ASTExpression(ASTExpression* returnType){
    left = returnType;
  }
protected:
  ASTNode* left = nullptr;
};
