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

struct ASTNode {
public:
	virtual ~ASTNode() {};
	//virtual void codeGen() = 0;
	virtual void print() const = 0;

};

struct ASTStatement: public ASTNode {
public:
	~ASTStatement(){}
	void print() const override{
		if (left !=NULL) left->print();
		if (right !=NULL) right->print();
	}
	ASTStatement(const ASTStatement* statementIn) : left(statementIn), right(nullptr){}
	ASTStatement(const ASTStatement* statementInA, const ASTStatement* statementInB) : left(statementInA), right(statementInB){}
	ASTStatement(): left(nullptr), right(nullptr) {}
private:
	const ASTStatement* left;
	const ASTStatement* right;
};
