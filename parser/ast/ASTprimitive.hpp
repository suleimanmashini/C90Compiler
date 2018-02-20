#pragma once
#include "AST.hpp"

struct ASTInteger : public ASTExpression {
public:
	ASTInteger(int IntegerIn) : Value(IntegerIn) {};
private:
	int Value;
};


struct ASTPrimitive : public ASTExpression {
public:
private:
	int Value;
};
