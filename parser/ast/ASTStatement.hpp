#pragma once
#include "ASTNode.hpp"

struct ASTReturnStatement : public ASTStatement{
public:
	~ASTReturnStatement(){}
	void print() const override {
		tabify();
		std::cout<< "return ";
		child->print();
		std::cout<<std::endl;
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
		tabify();
		type->print();
		std::cout<<" "<<Name;
		if (child != NULL) {std::cout<<"="; child->print();}
		std::cout<<std::endl;
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


struct ASTArgumentStatement: public ASTStatement {
public:
	~ASTArgumentStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	ASTArgumentStatement(const ASTStatement* statementIn) : left(statementIn), right(nullptr){}
	ASTArgumentStatement(const ASTStatement* statementInA, const ASTStatement* statementInB) : left(statementInA), right(statementInB){}
	ASTArgumentStatement(): left(nullptr), right(nullptr) {}
private:
	const ASTStatement* left;
	const ASTStatement* right;
};
