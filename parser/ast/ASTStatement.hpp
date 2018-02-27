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
	ASTArgumentStatement(const ASTVariable* statementIn) : left(statementIn), right(nullptr){}
	ASTArgumentStatement(const ASTVariable* statementInA, const ASTArgumentStatement* statementInB) : left(statementInA), right(statementInB){}
	ASTArgumentStatement(): left(nullptr), right(nullptr) {}
private:
	const ASTVariable* left;
	const ASTArgumentStatement* right;
};


struct ASTSelectionStatement: public ASTStatement {
public:
	~ASTSelectionStatement(){}
	void print() const override{
		tabify();
		std::cout << "if ";
		if(Condition != NULL) Condition->print();
		std::cout<< ":" <<std::endl;
		tabspace++;
		if (ifTrue !=NULL) ifTrue->print();
		tabspace--;
		tabify();
		if (ifFalse !=NULL) std::cout<<"else: "<<std::endl;
		tabspace++;
		if (ifFalse !=NULL) ifFalse->print();
		tabspace--;
	}
	ASTSelectionStatement(const ASTExpression* CondIn, const ASTStatement* statementIn) : Condition(CondIn), ifTrue(statementIn) {}
	ASTSelectionStatement(const ASTExpression* CondIn, const ASTStatement* statementInA, const ASTStatement* statementInB): Condition(CondIn), ifTrue(statementInA), ifFalse(statementInB){}
	ASTSelectionStatement(): Condition(nullptr), ifTrue(nullptr), ifFalse(nullptr) {}
private:
	const ASTExpression* Condition;
	const ASTStatement* ifTrue;
	const ASTStatement* ifFalse;
};
