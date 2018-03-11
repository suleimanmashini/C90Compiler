#pragma once


struct ASTStatement: public ASTNode {
public:
	virtual ~ASTStatement() {};
	virtual void codeGen() const = 0;
};


struct ASTReturnStatement: public ASTStatement {
ASTReturnStatement(const ASTExpression* _returnExpression) : returnExpression(_returnExpression) {}
void codeGen() const override {
	if(returnExpression == NULL) {
  std::cout<< "\tnop \n";
} else {
	returnExpression->codeGen(V0);
	std::cout<< "\tnop \n";
}
}
private:
  const ASTExpression* returnExpression;
};


struct ASTStatementList: public ASTStatement {

ASTStatementList(const ASTStatementList* _Child, const ASTReturnStatement* _Statement): Child(_Child), Statement(_Statement) {}
void codeGen() const override {
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
  const ASTStatementList* Child;
  const ASTReturnStatement* Statement;

};
struct ASTDeclarationList;
struct ASTCompoundStatement: public ASTStatement {
public:
~ASTCompoundStatement() {}
ASTCompoundStatement(const ASTStatementList* _StateList, const ASTDeclarationList* _DeclList): StatList(_StateList), DeclList(_DeclList) {}
void codeGen() const override {
	if (StatList == NULL && DeclList == NULL) {
		std::cout <<"\tnop\n";
		return;
	}
  StatList->codeGen();
  //TODO: INCLUDE DECLlIST HERE!!
}
private:
  const ASTStatementList* StatList;
  const ASTDeclarationList* DeclList;
};

struct ASTExpression;
