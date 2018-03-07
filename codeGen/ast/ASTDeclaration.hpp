#pragma once
#include "ASTNode.hpp"

struct ASTDeclaration: public ASTNode {

};

struct ASTFunctionDefinition: public ASTNode {
public:
  ASTFunctionDefinition(const ASTDeclaration* _typeSpecifier, const int _returnType, const std::string _functionName, const ASTStatement* _block): returnType(_returnType), typeSpecifier(_typeSpecifier), functionName(_functionName), block(_block) {}
  int getReturnType const () {return functionType->getType();}
  std::string getFunctionName const() {Declarator->getName();}
  void codeGen const override();
private:
  const ASTTypeSpecifier* functionType;
  const ASTDirectDeclarator* Declarator;
  const ASTCompoundStatement* Block;
};

struct ASTDirectDeclarator: public Declaration {
public:
  ASTDirectDeclarator(const std::string _Identifier): Identifier(_Identifier) {}
  std::string getName () {return Identifier;}
private:
  //TODO: add a parameters PARAMETEr list
  const std::string Identifier;
};

struct ASTTypeSpecifier: public ASTDeclaration {
public:
  ASTTypeSpecifier(const int _typeNumber): typeNumber(_typeNumber) {}
  int getType() { return typeNumber;}
private:
  const int typeNumber;
  //lets assume for now 0 is void and 1 is int
};

struct ASTDeclarationList: public ASTDeclaration {};
