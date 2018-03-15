#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
std::vector<variable> allVariables;
std::vector<std::string> regList = {"$v0", "$v1", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"};

int main() {
				 ASTTranslationUnit *ASTRoot = parseAST();
				ASTRoot->codeGen();

	return 0;
}

std::vector<std::string> tail(std::vector<std::string> vIn){
	vIn.erase(vIn.begin());
	return vIn;
}

std::string head(std::vector<std::string> vIn) {
	return vIn[0];
}
