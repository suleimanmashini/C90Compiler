#pragma once

#include "ASTexpression.hpp"
#include "ASTFunction.hpp"
#include "ASTprimitive.hpp"
#include "ASTStatement.hpp"

#include <iostream>

struct ASTNode{
public:
	ASTNode(ASTNode* child) { left = child; };
	virtual ~ASTNode() = 0;
	virtual void codeGen() = 0;
	virtual void print() const = 0;
private:
	ASTNode* left = nullptr;;

};

extern const ASTNode *parseAST();

