#pragma once
#include "ASTNode.hpp"

struct ASTStatement: public ASTNode {

};

struct ASTCompoundStatement: public ASTStatement {
public:

private:
  const ASTStatementList* StatList;
  const ASTDeclarationList* DeclList;
};

struct ASTStatementList: public ASTStatement {

};

struct ASTReturnStatement: public ASTStatement {

};
