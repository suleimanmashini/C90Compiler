#pragma once
//TODO:ADD SUPPORT FOR VOID EXPRESSIONS
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <stdio.h>
#include <string.h>

extern int initialVSize;
extern int NumberofVaraibles;
extern int currentScope;
extern std::vector<std::string> regList;
extern std::vector<std::string> regListFloat;
extern int maxArgs;
extern int isFloat;
extern std::vector<int> floatValues;
std::string uniqueIdGenFloat();
extern std::string currentFunction;
void setAccessible(int indexStart, int numofSets);
void unsetAccessible(int indexStart, int numofSets);

struct variable {
public:
variable(int _type, std::string _name, int _foundInScope, int _isAccess): type(_type), name(_name), foundInScope(_foundInScope), isAccess(_isAccess) {}
std::string getName() {
	return name;
}
int getScope() {
	return foundInScope;
}
int getisAccess() {
	return isAccess;
}
void setAccess() {
	isAccess = 1;
}
void unsetAccess() {
	isAccess = 0;
}
private:
int type;
int foundInScope;
std::string name;
int isAccess;
};

extern std::vector<variable> allVariables;

std::vector<std::string> tail(std::vector<std::string> vIn);
std::string head(std::vector<std::string> vIn);
int findVariableIndex (std::vector<variable> vIn, std::string variableName);
//WE NEED THIS TO KEEP TRACK OF PROTOTYPING
extern std::vector<std::string> listOfFunctions;


struct ASTNode {
public:
	virtual ~ASTNode() {}
	virtual void codeGen()  {};
};

struct ASTDeclaration: public ASTNode {
public:
	~ASTDeclaration() {}
	virtual void codeGen()  {}
	virtual void pushVariables()  {}
	virtual std::string getName() {}
	virtual void pushArguments() {}
	virtual int countArgs() {}
	virtual void popVariables() {}
private:
};

struct ASTDeclarationList: public ASTDeclaration {
~ASTDeclarationList() {}
ASTDeclarationList( ASTDeclarationList* _Child,  ASTDeclaration* _Statement): Child(_Child), Declaration(_Statement) {}
void codeGen()  override {
  if (Child == NULL && Declaration == NULL) {
		std::cout <<"\tnop\n";
		return;
	}
	if (Declaration != NULL) {
		Declaration->codeGen();
	}
	if (Child != NULL) {
		Child->codeGen();
	}
}
void pushVariables()  {
	if (Declaration != NULL) {
		Declaration->pushVariables();
	}
	if (Child != NULL) {
		Child->pushVariables();
	}
}
private:
   ASTDeclarationList* Child;
   ASTDeclaration* Declaration;
};





enum {
	RZERO, //HARD WIRES TO 0
	AT,  //RESERVED FOR PSEUDO-INSTRUCTIONS
	V0, V1, //RETURN VALUES FROM FUNCTIONS
	A0, A1, A2, A3, //ARGUMENTS TO FUNCTIONS (NOT PRESERVED BY SUBPROGRAMS)
	T0, T1, T2, T3, T4, T5, T6, T7, //TEMPORARY DATA (NOT PRESERVED BY SUBPROGRAMS)
	S0, S1, S2, S3, S4, S5, S6, S7, //SAVED REGISTERS (PRESERVED BY SUPROGRAMS)
	T8, T9, //MORE TEMP REGISTERS (NOT PRESEVED BY SUBPROGRAMS)
	K0, K1, //KERNEL
	GP, //GLOBAL AREA POINTED
	SP, //STACK POINTER
	FP, //FRAME POINTER
	RA, //RETURN ADDRESS
	HI, LO,
	PC
};
