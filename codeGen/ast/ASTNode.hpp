#pragma once

#include <iostream>
#include <string>
#include <vector>


struct ASTNode {
public:
	virtual ~ASTNode() {};
	virtual void codeGen() const = 0;
};
