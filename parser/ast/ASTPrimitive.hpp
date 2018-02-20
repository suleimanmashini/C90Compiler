#pragma once

#include "ASTExpression.hpp"

enum types {integer = 0, character = 1};


struct ASTPrimitive : public ASTExpression {
public:
protected:
	int typeValue;
};

struct ASTInteger : public ASTPrimitive {
public:
	ASTInteger(int IntegerIn) : Value(IntegerIn): typeValue(integer);
private:
};
