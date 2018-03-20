#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
std::vector<variable> allVariables;
std::vector<std::string> regList = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5",
																		"$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3",
																		"$s4", "$s5", "$s6", "$s7"};
std::vector<std::string> listOfFunctions;

int initialVSize;
int NumberofVaraibles;
int currentScope;
int maxArgs;
void fullCompiler() {

 	ASTTranslationUnit *ASTRoot = parseAST();
	std::cout<<"\t.nan  legacy\n";
	std::cout<<"\t.module fp=xx\n";
	std::cout<<"\t.module nooddspreg\n";
	std::cout<<"\t.abicalls\n";
	ASTRoot->codeGen();
}

std::vector<std::string> tail(std::vector<std::string> vIn){
	vIn.erase(vIn.begin());
	return vIn;
}

std::string head(std::vector<std::string> vIn) {
	return vIn[0];
}

//THIS FUNCTION NOW FINDS SHADOWING FUNCTIONS
int findVariableIndex (std::vector<variable> vIn, std::string variableName){
	for (int i = vIn.size() - 1; i >= 0; i--) {
		if (vIn[i].getName() == variableName && vIn[i].getScope() == currentScope) {
			return i;
		}
	}
	for (int i = vIn.size() - 1; i >= 0; i--) {
		if (vIn[i].getName() == variableName) {
			return i;
		}
	}
	return -1;
}
