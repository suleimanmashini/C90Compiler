#include "AST.hpp"
#include <iostream>

int main() {
	std::cout<< "start" << std::endl;
	const ASTNode *ASTRoot = parseAST();
	ASTRoot->print();
	return 0;
}
