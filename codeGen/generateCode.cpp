#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

//this UNION WAS TAKEN FROM:
//*****************************************
// https://gist.github.com/khajavi/5667960
//*****************************************
union FloatingPointIEEE754 {
	struct {
		unsigned int mantissa : 23;
		unsigned int exponent : 8;
		unsigned int sign : 1;
	} raw;
	float f;
} Fnum;

std::vector<int> floatValues;
std::vector<variable> allVariables;
std::vector<std::string> regList = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5",
																		"$t6", "$t7", "$t8", "$t9", "$s0", "$s1", "$s2", "$s3",
																		"$s4", "$s5", "$s6", "$s7"};

std::vector<std::string> regListFloat = {"$f4",  "$f6",   "$f8", "$f10"
																				 "$f16", "$f18", "$f20", "$f22",
																			 	 "$f24", "$f26", "$f28", "f30"};

std::vector<std::string> listOfFunctions;
int uniqueID = 2;
int uniqueIDFloat = 0;
int initialVSize;
int workingWithFloat = 0;
int NumberofVaraibles;
int isAccessible = 1;
int currentScope;
int isGlobal = 1;
int globalsFound;
int maxArgs;
std::string currentFunction;
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
		if (vIn[i].getName() == variableName && vIn[i].getScope() == currentScope && vIn[i].getisAccess() == 1) {
			return i;
		}
	}
	for (int i = vIn.size() - 1; i >= 0; i--) {
		if (vIn[i].getName() == variableName && vIn[i].getScope() <= currentScope && vIn[i].getisAccess() == 1) {
			return i;
		}
	}
	return -1;
}

void setAccessible(int indexStart, int numofSets) {
	for (int i = indexStart; i < numofSets + indexStart; i++) {
		allVariables[i].setAccess();
	}
}

void unsetAccessible(int indexStart, int numofSets) {
	for (int i = indexStart; i < numofSets + indexStart; i++) {
		allVariables[i].unsetAccess();
	}
}

std::string uniqueIdGen() {

	std::string temp = "$L" + std::to_string(uniqueID);
	uniqueID++;
	return temp;

}

std::string uniqueIdGenFloat() {

	std::string temp = "$LC" + std::to_string(uniqueIDFloat);
	uniqueIDFloat++;
	return temp;

}
