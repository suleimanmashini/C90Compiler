#pragma once

#include "TSTExpression.hpp"

enum types {integer = 0, character = 1, vOID = 2, key = 3};


struct TSTPrimitive : public TSTExpression {
public:
	TSTPrimitive() {}
protected:
	int typeValue;
};

struct TSTInteger : public TSTPrimitive {
public:
	~TSTInteger(){}
	void print() const override {
		std::cout<< "(" << Value << ")";
	}
	 TSTInteger(int IntegerIn){
		Value = IntegerIn;
		typeValue = integer;
	}
protected:
	int Value;
};

struct TSTVoid : public TSTPrimitive {
public:
	~TSTVoid(){}
	void print() const override {
		std::cout<< "Void!" << std::endl;
	}
	 TSTVoid(){
		typeValue = vOID;
	}
};

struct TSTKeyword : public TSTPrimitive {
public:
	~TSTKeyword(){}
	TSTKeyword() {}
	TSTKeyword(std::string Name): keyword(Name) {}
	void print() const override {
		std::cout<< keyword;
	}
private:
	const std::string keyword;
};


struct TSTVariable : public TSTPrimitive {
public:
	~TSTVariable(){}
	TSTVariable() {}
	TSTVariable(const TSTKeyword* Name, std::string IDIn): keyword(Name), ID(IDIn) {}
	void print() const override {
		keyword->print();
		std::cout<< " " << ID ;

	}
private:
	const TSTKeyword* keyword;
	const std::string ID;
};

struct TSTVariableName : public TSTPrimitive {
public:
	~TSTVariableName(){}
	TSTVariableName() {}
	TSTVariableName(std::string IDIn): ID(IDIn) {}
	void print() const override {
		std::cout<< ID ;

	}
private:
	const std::string ID;
};
