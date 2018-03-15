#pragma once
#include "TSTNode.hpp"
extern std::vector <std::string> globalVariables;

struct TSTReturnStatement : public TSTStatement{
public:
	~TSTReturnStatement(){}
	void print() const override {
		tabify();
		std::cout<< "return ";
		child->print();
		std::cout<<std::endl;
	}
	TSTReturnStatement(const TSTExpression* expressionIn) {
		child = expressionIn;
	}
	TSTReturnStatement(){
		child = NULL;
	}
private:
	const TSTExpression* child;
};

struct TSTAssignmentStatement : public TSTStatement{
public:
	~TSTAssignmentStatement(){}
	void print() const override {
		tabify();
		left->print();
		std::cout<<" = ";
		right->print();
		std::cout<<std::endl;
	}
	TSTAssignmentStatement(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn) {}
	TSTAssignmentStatement(){}
private:
	const TSTExpression* left;
	const TSTExpression* right;

};

struct TSTDeclarationStatement : public TSTStatement{
public:
	~TSTDeclarationStatement(){}
	void print() const override {
		tabify();
		//type->print();

		std::cout<<Name;
		if (child != NULL) {std::cout<<"="; child->print();}
		else std::cout<< " = 0";
		std::cout<<std::endl;
	}
	TSTDeclarationStatement(const TSTExpression* expressionIn, const TSTKeyword* typeIn, const std::string NameIn, const int flagIn): Name(NameIn), child(expressionIn), type(typeIn), globalFlag(flagIn){
		if (globalFlag == 1){
			globalVariables.push_back(Name);
		}
	}
	TSTDeclarationStatement(const TSTKeyword* typeIn, const std::string NameIn, const int flagIn): Name(NameIn), child(nullptr), type(typeIn), globalFlag(flagIn) {
		if (globalFlag == 1){
			globalVariables.push_back(Name);
		}
	}
	TSTDeclarationStatement(): globalFlag(0){
		TSTInteger* temp = new TSTInteger(0);
		child = temp;

	}
private:
	const TSTKeyword* type;
	const std::string Name;
	const TSTExpression* child;
	const int globalFlag;
};


struct TSTExpressionStatement: public TSTStatement {
public:
	~TSTExpressionStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	TSTExpressionStatement(const TSTExpression* statementIn) : left(statementIn), right(nullptr){}
	TSTExpressionStatement(const TSTExpression* statementInA, const TSTExpressionStatement* statementInB) : left(statementInA), right(statementInB){}
	TSTExpressionStatement(): left(nullptr), right(nullptr) {}
private:
	const TSTExpression* left;
	const TSTExpressionStatement* right;
};


struct TSTVariableStatement: public TSTStatement {
public:
	~TSTVariableStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	TSTVariableStatement(const TSTVariable* statementIn) : left(statementIn), right(nullptr){}
	TSTVariableStatement(const TSTVariable* statementInA, const TSTVariableStatement* statementInB) : left(statementInA), right(statementInB){}
	TSTVariableStatement(): left(nullptr), right(nullptr) {}
private:
	const TSTVariable* left;
	const TSTVariableStatement* right;
};


struct TSTFunctionCall : public TSTExpression {
public:
  ~TSTFunctionCall(){}
  void print() const override {
		std::cout<< FunctionName << "( ";
    if(Args != NULL) Args->print();
    std::cout << " )";
	}
  TSTFunctionCall() {}
  TSTFunctionCall(const std::string NameIn): FunctionName(NameIn), Args(NULL) {}
  TSTFunctionCall(const std::string NameIn, const TSTExpressionStatement* ArgsIn): FunctionName(NameIn), Args(ArgsIn) {}
private:
  const std::string FunctionName;
  const TSTExpressionStatement* Args;
};

struct TSTSelectionStatement: public TSTStatement {
public:
	~TSTSelectionStatement(){}
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
	TSTSelectionStatement(const TSTExpression* CondIn, const TSTStatement* statementIn) : Condition(CondIn), ifTrue(statementIn) {}
	TSTSelectionStatement(const TSTExpression* CondIn, const TSTStatement* statementInA, const TSTStatement* statementInB): Condition(CondIn), ifTrue(statementInA), ifFalse(statementInB){}
	TSTSelectionStatement(): Condition(nullptr), ifTrue(nullptr), ifFalse(nullptr) {}
private:
	const TSTExpression* Condition;
	const TSTStatement* ifTrue;
	const TSTStatement* ifFalse;
};

struct TSTIteratorStatementWhile: public TSTStatement {
public:
	~TSTIteratorStatementWhile(){}
	void print() const override{
		tabify();
		std::cout<<"while (";
		if (Cond !=NULL) Cond->print();
		std::cout<<" ): "<<std::endl;
		tabspace++;
		if (Block !=NULL) Block->print();
		tabspace--;
	}
	TSTIteratorStatementWhile(const TSTExpression* CondIn) : Cond(CondIn), Block(nullptr){}
	TSTIteratorStatementWhile(const TSTExpression* CondIn, const TSTStatement* BlockIn) : Cond(CondIn), Block(BlockIn){}
	TSTIteratorStatementWhile(): Cond(nullptr), Block(nullptr) {}
private:
	const TSTExpression* Cond;
	const TSTStatement* Block;
};
