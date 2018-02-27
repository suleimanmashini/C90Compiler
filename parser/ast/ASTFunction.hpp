#pragma once
#include "ASTNode.hpp"
#include "ASTPrimitive.hpp"
#include "ASTStatement.hpp"
extern std::vector <std::string> globalVariables;

struct ASTFunction : public ASTStatement {
public:
  ~ASTFunction() {}
  ASTFunction(const ASTPrimitive* returnTypeIn, std::string functionNameIn, const ASTStatement* parametersIn, const ASTStatement* BlockIn )
  : returnType(returnTypeIn), functionName(functionNameIn), parameters(parametersIn), Block(BlockIn){}
  void print() const override {
    tabspace += 4;
    std::cout << "def " << functionName << "(";
    if(parameters != NULL) parameters->print();
    std::cout << "):" << std::endl;
    if(globalVariables.size() != 0){
      for (int i = 0; i < globalVariables.size(); i++){
      tabify();
      std::cout<<"global "<< globalVariables[i] <<std::endl;
    }
  }
    Block->print();
    std::cout<< std::endl;
    tabspace -= 4;
  }
protected:
  const ASTPrimitive* returnType;
  const std::string functionName;
  const ASTStatement* parameters;
  const ASTStatement* Block;
};
