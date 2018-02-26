#pragma once
#include "ASTNode.hpp"
#include "ASTPrimitive.hpp"
#include "ASTStatement.hpp"

struct ASTFunction : public ASTStatement {
public:
  ~ASTFunction() {}
  ASTFunction(const ASTPrimitive* returnTypeIn, std::string functionNameIn, const ASTStatement* parametersIn, const ASTStatement* BlockIn )
  : returnType(returnTypeIn), functionName(functionNameIn), parameters(parametersIn), Block(BlockIn){}
  void print() const override {
    tabspace++;
    std::cout << "def " << functionName << "(";
    if(parameters != NULL) parameters->print();
    std::cout << "):" << std::endl;
    Block->print();
    std::cout<< std::endl;
    tabspace--;
  }
protected:
  const ASTPrimitive* returnType;
  const std::string functionName;
  const ASTStatement* parameters;
  const ASTStatement* Block;
};
