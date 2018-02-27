#include "AST.hpp"
#include <iostream>
#include <stdio.h>

int tabspace = 0;

int main(int argc, char* argv[]) {
	const ASTNode *ASTRoot = parseAST();
	//if (argv[1] == "--translate") {
	ASTRoot->print();
//}

	return 0;
}
