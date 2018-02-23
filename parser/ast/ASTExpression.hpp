#pragma once

#include "ASTNode.hpp"

struct ASTExpression : public ASTNode {
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
  ASTNode* left = nullptr;
};
