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

struct ASTArgumentStatement: public ASTStatement {
public:
	~ASTArgumentStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	ASTArgumentStatement(const ASTExpression* statementIn) : left(statementIn), right(nullptr){}
	ASTArgumentStatement(const ASTExpression* statementInA, const ASTArgumentStatement* statementInB) : left(statementInA), right(statementInB){}
	ASTArgumentStatement(): left(nullptr), right(nullptr) {}
private:
	const ASTExpression* left;
	const ASTArgumentStatement* right;
};
