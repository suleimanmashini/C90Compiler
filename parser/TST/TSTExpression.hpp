#pragma once

#include "TSTNode.hpp"

struct TSTExpression : public TSTStatement {
public:
  ~TSTExpression(){}
  void print() const override {
		std::cout<< "Expression" << std::endl;
    left->print();
	}
  TSTExpression() {}
  TSTExpression(TSTExpression* returnType){
    left = returnType;
  }
private:
  TSTExpression* left = nullptr;
};

struct TSTArgumentStatement: public TSTStatement {
public:
	~TSTArgumentStatement(){}
	void print() const override{
		if (left !=NULL) left->print();

		if (right !=NULL) {std::cout<< " , "; right->print();}
	}
	TSTArgumentStatement(const TSTExpression* statementIn) : left(statementIn), right(nullptr){}
	TSTArgumentStatement(const TSTExpression* statementInA, const TSTArgumentStatement* statementInB) : left(statementInA), right(statementInB){}
	TSTArgumentStatement(): left(nullptr), right(nullptr) {}
private:
	const TSTExpression* left;
	const TSTArgumentStatement* right;
};
