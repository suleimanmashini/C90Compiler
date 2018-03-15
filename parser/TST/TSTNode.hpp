#pragma once

#include <iostream>
#include <string>
#include <vector>


extern int tabspace;

inline void tabify () {
	for (int i = 0; i < tabspace; i++){
		std::cout<<" ";
	}
}

struct TSTNode {
public:
	virtual ~TSTNode() {};
	//virtual void codeGen() = 0;
	virtual void print() const = 0;

};

struct TSTStatement: public TSTNode {
public:
	~TSTStatement(){}
	void print() const override{
		if (left !=NULL) left->print();
		if (right !=NULL) right->print();
	}
	TSTStatement(const TSTStatement* statementIn) : left(statementIn), right(nullptr){}
	TSTStatement(const TSTStatement* statementInA, const TSTStatement* statementInB) : left(statementInA), right(statementInB){}
	TSTStatement(): left(nullptr), right(nullptr) {}
private:
	const TSTStatement* left;
	const TSTStatement* right;
};
