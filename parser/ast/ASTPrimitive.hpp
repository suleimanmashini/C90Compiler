#pragma once

#include "ASTExpression.hpp"

enum types {integer = 0, character = 1, vOID = 2};


struct ASTPrimitive : public ASTExpression {
public:
	ASTPrimitive() {}
protected:
	int typeValue;
};

struct ASTInteger : public ASTPrimitive {
public:
	~ASTInteger(){}
	void print() const override {
		std::cout<< Value << std::endl;
	}
	 ASTInteger(int IntegerIn){
		Value = IntegerIn;
		typeValue = integer;
	}
protected:
	int Value;
};

struct ASTVoid : public ASTPrimitive {
public:
	~ASTVoid(){}
	void print() const override {
		std::cout<< "Void!" << std::endl;
	}
	 ASTVoid(){
		typeValue = 2;
	}
};
