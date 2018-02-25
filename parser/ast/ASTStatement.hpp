#pragma once
#include "ASTNode.hpp"

struct ASTStatement: public ASTNode {
public:
	~ASTStatement(){}
	void print() const override{
		std::cout<<"Statement"<< std::endl;
		child->print();
	}
	ASTStatement(const ASTExpression* expressionIn) : child(expressionIn){}
	ASTStatement() {}
private:
	const ASTExpression* child;
};

struct ASTReturnStatement : public ASTStatement{
public:
	~ASTReturnStatement(){}
	void print() const override {
		std::cout<< "return ";
		child->print();
	}
	ASTReturnStatement(const ASTExpression* expressionIn) {
		child = expressionIn;
	}
	ASTReturnStatement(){
		child = NULL;
	}
private:
	const ASTExpression* child;
};
