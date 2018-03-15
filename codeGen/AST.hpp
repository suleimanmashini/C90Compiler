#pragma once

#include "ast/ASTNode.hpp"
#include "ast/ASTExpression.hpp"
#include "ast/ASTStatement.hpp"
#include "ast/ASTDeclaration.hpp"


struct ASTTranslationUnit : public ASTNode {
public:
  ~ASTTranslationUnit() {}
  ASTTranslationUnit( ASTFunctionDefinition* _Function,  ASTDeclaration* _Dec,  ASTTranslationUnit* _Next): Next(_Next), Dec(_Dec), Function(_Function) {}
  void codeGen()  {
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
   ASTFunctionDefinition* Function;
   ASTDeclaration* Dec;
   ASTTranslationUnit* Next;
};

extern  ASTTranslationUnit *parseAST();
