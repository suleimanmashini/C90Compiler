#pragma once

#include "ast/ASTNode.hpp"
#include "ast/ASTExpression.hpp"
#include "ast/ASTStatement.hpp"
#include "ast/ASTDeclaration.hpp"


struct ASTTranslationUnit : public ASTNode {
public:
  ~ASTTranslationUnit() {}
  ASTTranslationUnit(const ASTFunctionDefinition* _Function, const ASTDeclaration* _Dec, const ASTTranslationUnit* _Next): Next(_Next), Dec(_Dec), Function(_Function) {}
  void codeGen() const {
    if (Function != NULL) {
      Function->codeGen();
    }
    if (Dec != NULL){
      Dec->codeGen();
    }
    if (Next != NULL) {
      Next->codeGen();
    }
  }
private:
  const ASTFunctionDefinition* Function;
  const ASTDeclaration* Dec;
  const ASTTranslationUnit* Next;
};

extern const ASTTranslationUnit *parseAST();
