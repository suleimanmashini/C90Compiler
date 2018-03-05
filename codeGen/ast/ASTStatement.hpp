#pragma once
#include "ASTNode.hpp"

struct ASTFunction: public ASTNode {
public:
private:
  const int returnType;
  const std::string functionName;
  const ASTStatement* block;
  const AST
};
