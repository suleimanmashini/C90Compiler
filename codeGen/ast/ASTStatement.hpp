#pragma once
struct ASTDeclarationList;

struct ASTStatement: public ASTNode {
public:
	virtual ~ASTStatement() {};
	virtual void codeGen() const = 0;
};

struct ASTStatementList: public ASTStatement {
ASTStatementList(const ASTStatementList* _Child, const ASTStatement* _Statement): Child(_Child), Statement(_Statement) {}
void codeGen() const override {
  Statement->codeGen();
  //TODO: SUPPORT MULTIPLE STATEMENTS
}
private:
  const ASTStatementList* Child;
  const ASTStatement* Statement;

};

struct ASTCompoundStatement: public ASTStatement {
public:
~ASTCompoundStatement() {}
ASTCompoundStatement(const ASTStatementList* _StateList, const ASTDeclarationList* _DeclList): StatList(_StateList), DeclList(_DeclList) {}
void codeGen() const override {
  StatList->codeGen();
  //TODO: INCLUDE DECLlIST HERE!!
}
private:
  const ASTStatementList* StatList;
  const ASTDeclarationList* DeclList;
};

struct ASTExpression;

struct ASTReturnStatement: public ASTStatement {
ASTReturnStatement(const ASTExpression* _returnExpression): returnExpression(_returnExpression) {}
void codeGen() const override {
  if(returnExpression == NULL) std::cout<< "\tnop \n";
}
private:
  const ASTExpression* returnExpression;
};
