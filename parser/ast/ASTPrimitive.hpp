#pragma once
#include "AST.hpp"

enum types {integer = 0, character = 1}
struct ASTInteger : public ASTPrimitive {
public:
	ASTInteger(int IntegerIn) : Value(IntegerIn): typeValue(integer);
private:
};


struct ASTPrimitive : public ASTExpression {
public:
private:
	int typeValue
};
