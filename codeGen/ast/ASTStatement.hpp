#pragma once

struct ASTStatement: public ASTExpression {
public:
	virtual ~ASTStatement() {};
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
private:
   ASTStatementList* Child;
   ASTStatement* Statement;

};

struct ASTCompoundStatement: public ASTStatement {
public:
~ASTCompoundStatement() {}
ASTCompoundStatement( ASTStatementList* _StateList,  ASTDeclarationList* _DeclList): StatList(_StateList), DeclList(_DeclList) {}
void codeGen()  override {
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
  //TODO: INCLUDE DECLlIST HERE!!
}
void pushVariables()  {
	if (DeclList != NULL) {
		DeclList->pushVariables();
		//TODO: ADD SUPPORT TO OTHER STATEMENTS THAT HAVE BLOCKS
	}
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
		Exp->codeGen();
	}
private:
 ASTExpression* Exp;
};
