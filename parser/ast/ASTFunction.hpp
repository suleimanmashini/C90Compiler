#pragma once
#include "AST.hpp"

struct ASTfunction : public ASTNode {
public:
  ASTfunction(ASTPrimitive* returnType, string functionName, ASTNode* parameters, ASTReturnStatement* returnNumber ) {
    //TO BE CLEANED UP LATER THIS IS TO JUST GET THE FUNCTIONALITY GOING!!!!
    this->returnType = returnType;
    this->functionName = functionName;
    this->parameters = parameters;
    this->returnNumber = returnNumber;
  }
  void print() const override {
    std::cout << "FUNCTION" << std::endl;
  };
private:
  ASTprimitive* returnType
  string functionName;
  ASTNode* parameters;
  ASTReturnStatement returnNumber;
};
