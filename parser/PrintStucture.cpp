#include "ast/AST.hpp"

int main() {

	const ASTNode *ASTRoot = parseAST();
	ASTRoot->print();
	return 0;
}