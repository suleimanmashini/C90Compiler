#pragma once
#include "ASTNode.hpp"

struct ASTStatement: public ASTNode {
};

struct ASTReturnStatement : public ASTStatement{
public:
	~ASTReturnStatement(){}
	void print() const override {
		std::cout<< "RETURN!" << std::endl;
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
