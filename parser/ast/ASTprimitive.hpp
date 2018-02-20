#pragma once
#include "AST.hpp"

struct ASTInteger : public ASTExpression {
public:
	ASTInteger(int IntegerIn) : Value(IntegerIn) {};
private:
	int Value;
};