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

struct ASTAssignmentStatement : public ASTStatement{
public:
	~ASTAssignmentStatement(){}
	void print() const override {
		tabify();
		left->print();
		std::cout<<" = ";
		right->print();
		std::cout<<std::endl;
	}
	ASTAssignmentStatement(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn) {}
	ASTAssignmentStatement(){}
private:
	const ASTExpression* left;
	const ASTExpression* right;

};

struct ASTDeclarationStatement : public ASTStatement{
public:
	~ASTDeclarationStatement(){}
	void print() const override {
		tabify();
		//type->print();
		std::cout<<Name;
		if (child != NULL) {std::cout<<"="; child->print();}
		else std::cout<< " = 0";
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


struct ASTExpressionStatement: public ASTStatement {
public:
	~ASTExpressionStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	ASTExpressionStatement(const ASTExpression* statementIn) : left(statementIn), right(nullptr){}
	ASTExpressionStatement(const ASTExpression* statementInA, const ASTExpressionStatement* statementInB) : left(statementInA), right(statementInB){}
	ASTExpressionStatement(): left(nullptr), right(nullptr) {}
private:
	const ASTExpression* left;
	const ASTExpressionStatement* right;
};


struct ASTVariableStatement: public ASTStatement {
public:
	~ASTVariableStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	ASTVariableStatement(const ASTVariable* statementIn) : left(statementIn), right(nullptr){}
	ASTVariableStatement(const ASTVariable* statementInA, const ASTVariableStatement* statementInB) : left(statementInA), right(statementInB){}
	ASTVariableStatement(): left(nullptr), right(nullptr) {}
private:
	const ASTVariable* left;
	const ASTVariableStatement* right;
};


struct ASTFunctionCall : public ASTExpression {
public:
  ~ASTFunctionCall(){}
  void print() const override {
		std::cout<< FunctionName << "( ";
    if(Args != NULL) Args->print();
    std::cout << " )";
	}
  ASTFunctionCall() {}
  ASTFunctionCall(const std::string NameIn): FunctionName(NameIn), Args(NULL) {}
  ASTFunctionCall(const std::string NameIn, const ASTExpressionStatement* ArgsIn): FunctionName(NameIn), Args(ArgsIn) {}
private:
  const std::string FunctionName;
  const ASTExpressionStatement* Args;
};

struct ASTSelectionStatement: public ASTStatement {
public:
	~ASTSelectionStatement(){}
	void print() const override{
		tabify();
		std::cout << "if ";
		if(Condition != NULL) Condition->print();
		std::cout<< ":" <<std::endl;
		tabspace += 4;
		if (ifTrue !=NULL) ifTrue->print();
		tabspace -= 4;
		if (ifFalse !=NULL) {
			tabify();
			std::cout<<"else: "<<std::endl;
		}
		tabspace += 4;
		if (ifFalse !=NULL) ifFalse->print();
		tabspace -= 4;
	}
	ASTSelectionStatement(const ASTExpression* CondIn, const ASTStatement* statementIn) : Condition(CondIn), ifTrue(statementIn) {}
	ASTSelectionStatement(const ASTExpression* CondIn, const ASTStatement* statementInA, const ASTStatement* statementInB): Condition(CondIn), ifTrue(statementInA), ifFalse(statementInB){}
	ASTSelectionStatement(): Condition(nullptr), ifTrue(nullptr), ifFalse(nullptr) {}
private:
	const ASTExpression* Condition;
	const ASTStatement* ifTrue;
	const ASTStatement* ifFalse;
};
