#pragma once
#include "ASTNode.hpp"
#include "ASTPrimitive.hpp"
#include "ASTStatement.hpp"

struct ASTFunction : public ASTNode {
public:
  ~ASTFunction() {}
  ASTFunction(const ASTPrimitive* returnTypeIn, std::string functionNameIn, const ASTNode* parametersIn, const ASTStatement* BlockIn ) {
    //TO BE CLEANED UP LATER THIS IS TO JUST GET THE FUNCTIONALITY GOING!!!!
    returnType = returnTypeIn;
    functionName = functionNameIn;
    parameters = parametersIn;
    Block = BlockIn;
  }
  void print() const override {
    std::cout << "def " << functionName << " (): "<< std::endl;
    //returnType->print();
    std::cout << '\t';
    Block->print();
  }
protected:
  const ASTPrimitive* returnType;
  std::string functionName;
  const ASTNode* parameters;
  const ASTStatement* Block;
};
