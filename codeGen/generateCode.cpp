#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

std::vector <std::string> globalVariables;
int globalFlag = 0;

int tabspace = 0;

int main(int argc, char* argv[]) {
				const ASTNode *ASTRoot = parseAST();
				ASTRoot->codeGen();

	return 0;
}
