#pragma once

#include <iostream>
#include <string>

struct ASTNode {
public:
	virtual ~ASTNode() {};
	//virtual void codeGen() = 0;
	virtual void print() const = 0;

};
