#pragma once

#include "ASTNode.hpp"

struct ASTExpression : public ASTStatement {
public:
  ~ASTExpression(){}
  void print() const override {
		std::cout<< "Expression" << std::endl;
    left->print();
	}
  ASTExpression() {}
  ASTExpression(ASTExpression* returnType){
    left = returnType;
  }
private:
  ASTExpression* left = nullptr;
};
