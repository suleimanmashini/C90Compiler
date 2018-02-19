#pragma once

#include "ASTExpression.hpp"
#include "ASTStatement.hpp"

struct ASTNode{
public:
	ASTNode(ASTNode* child) { left = child; };
	virtual ~ASTNode() = 0;
	virtual void codeGen() = 0;
private:
	ASTNode* left = nullptr;;

};


extern ASTNode *ASTRoot;
