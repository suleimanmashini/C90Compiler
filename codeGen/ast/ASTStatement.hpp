#pragma once

struct ASTStatement: public ASTExpression {
public:
	virtual ~ASTStatement() {};
	virtual void pushVariables() {}
	virtual void codeGen()  = 0;
};


struct ASTReturnStatement: public ASTStatement {
ASTReturnStatement( ASTExpression* _returnExpression) : returnExpression(_returnExpression) {}
void codeGen()  override {
	if(returnExpression == NULL) {
  std::cout<< "\tnop \n";
} else {
	returnExpression->codeGen();
	std::cout<< "\tnop \n";
}
}
private:
   ASTExpression* returnExpression;
};


struct ASTStatementList: public ASTStatement {

ASTStatementList( ASTStatementList* _Child,  ASTStatement* _Statement): Child(_Child), Statement(_Statement) {}
void codeGen()  override {
  if (Child == NULL && Statement == NULL) {
		std::cout <<"\tnop\n";
		return;
	}
	if (Statement != NULL) {
		Statement->codeGen();
	}
	if (Child != NULL) {
		Child->codeGen();
	}
}
void pushVariables() override{
	if(Statement!=NULL) {
		Statement->pushVariables();
	}
	if (Child != NULL) {
		Child->pushVariables();
	}
}
private:
   ASTStatementList* Child;
   ASTStatement* Statement;

};

struct ASTCompoundStatement: public ASTStatement {
public:
~ASTCompoundStatement() {}
ASTCompoundStatement( ASTStatementList* _StateList,  ASTDeclarationList* _DeclList): StatList(_StateList), DeclList(_DeclList) {}
void codeGen()  override {
	currentScope++;
	if (StatList == NULL && DeclList == NULL) {
		std::cout <<"\tnop\n";
		return;
	}
	if(DeclList != NULL) {
		DeclList->codeGen();
	}
	if(StatList != NULL){
  StatList->codeGen();
}
	currentScope--;
  //TODO: INCLUDE DECLlIST HERE!!
}
void pushVariables()  override {
	currentScope++;
	if (DeclList != NULL) {
		DeclList->pushVariables();
		//TODO: ADD SUPPORT TO OTHER STATEMENTS THAT HAVE BLOCKS
	}
	if (StatList != NULL) {
		StatList->pushVariables();
	}
	currentScope--;
}
private:
   ASTStatementList* StatList;
   ASTDeclarationList* DeclList;
};

struct ASTExpression;
struct ASTExpressionStatement: ASTStatement {
	~ASTExpressionStatement() {}
	ASTExpressionStatement( ASTExpression* _Exp): Exp(_Exp) {}
	void codeGen()  {
		if (Exp != NULL) {
		Exp->codeGen();
	} else {

		std::cout<<"\tnop"<< std::endl;
	}
	}
	void pushVariables() override {
		Exp->pushVariables();
	}
private:
 ASTExpression* Exp;
};
