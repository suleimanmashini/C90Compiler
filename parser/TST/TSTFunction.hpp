#pragma once
#include "TSTNode.hpp"
#include "TSTPrimitive.hpp"
#include "TSTStatement.hpp"
extern std::vector <std::string> globalVariables;

struct TSTFunction : public TSTStatement {
public:
  ~TSTFunction() {}
  TSTFunction(const TSTPrimitive* returnTypeIn, std::string functionNameIn, const TSTStatement* parametersIn, const TSTStatement* BlockIn )
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
  const TSTPrimitive* returnType;
  const std::string functionName;
  const TSTStatement* parameters;
  const TSTStatement* Block;
};
