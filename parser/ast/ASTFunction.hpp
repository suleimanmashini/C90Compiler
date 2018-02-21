#pragma once
#include "ASTNode.hpp"
#include "ASTPrimitive.hpp"
#include "ASTStatement.hpp"

struct ASTFunction : public ASTNode {
public:
  ~ASTFunction() {}
  ASTFunction(const ASTPrimitive* returnTypeIn, std::string functionNameIn, const ASTNode* parametersIn, const ASTReturnStatement* returnNumberIn ) {
    //TO BE CLEANED UP LATER THIS IS TO JUST GET THE FUNCTIONALITY GOING!!!!
    returnType = returnTypeIn;
    functionName = functionNameIn;
    parameters = parametersIn;
    returnNumber = returnNumberIn;
  }
  void print() const override {
    std::cout << "FUNCTION" << std::endl;
  }
protected:
  const ASTPrimitive* returnType;
  std::string functionName;
  const ASTNode* parameters;
  const ASTReturnStatement* returnNumber;
};
