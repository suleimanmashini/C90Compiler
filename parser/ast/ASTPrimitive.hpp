#pragma once

#include "ASTExpression.hpp"

enum types {integer = 0, character = 1, vOID = 2, key = 3};


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
		std::cout<< Value;
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
		typeValue = vOID;
	}
};

struct ASTKeyword : public ASTPrimitive {
public:
	~ASTKeyword(){}
	ASTKeyword() {}
	ASTKeyword(std::string Name): keyword(Name) {}
	void print() const override {
		std::cout<< keyword;
	}
private:
	const std::string keyword;
};


struct ASTVariable : public ASTPrimitive {
public:
	~ASTVariable(){}
	ASTVariable() {}
	ASTVariable(const ASTKeyword* Name, std::string IDIn): keyword(Name), ID(IDIn) {}
	void print() const override {
		keyword->print();
		std::cout<< " " << ID ;

	}
private:
	const ASTKeyword* keyword;
	const std::string ID;
};
