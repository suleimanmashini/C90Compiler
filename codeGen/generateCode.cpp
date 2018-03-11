#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
std::vector<variable> allVariables;

int main() {
				const ASTFunctionDefinition *ASTRoot = parseAST();
				ASTRoot->codeGen();

	return 0;
}
