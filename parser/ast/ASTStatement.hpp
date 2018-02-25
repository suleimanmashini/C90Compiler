#pragma once
#include "ASTNode.hpp"

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

struct ASTDeclarationStatement : public ASTStatement{
public:
	~ASTDeclarationStatement(){}
	void print() const override {
		type->print();
		std::cout<<" "<<Name<<"=";
		child->print();
		std::cout<<'\n';
	}
	ASTDeclarationStatement(const ASTExpression* expressionIn, const ASTKeyword* typeIn, const std::string NameIn): Name(NameIn), child(expressionIn), type(typeIn) {}
	ASTDeclarationStatement(const ASTKeyword* typeIn, const std::string NameIn): Name(NameIn), child(nullptr), type(typeIn) {}
	ASTDeclarationStatement(){
		ASTInteger* temp = new ASTInteger(0);
		child = temp;
	}
private:
	const ASTKeyword* type;
	const std::string Name;
	const ASTExpression* child;
};
