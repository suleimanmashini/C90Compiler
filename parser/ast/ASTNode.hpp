#pragma once

#include <iostream>

struct ASTNode {
public:
	virtual ~ASTNode() = 0;
	virtual void codeGen() = 0;
	virtual void print() const = 0;


};
