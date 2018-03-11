#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

int main() {
				const ASTFunctionDefinition *ASTRoot = parseAST();
				ASTRoot->codeGen();

	return 0;
}
