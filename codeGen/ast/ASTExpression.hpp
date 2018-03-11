#pragma once
#include "ASTDeclaration.hpp"
struct ASTExpression;


struct ASTIntegerConst: public ASTNode {
public:
	~ASTIntegerConst() {}
	ASTIntegerConst(const int _numValue): numValue(_numValue) {}
	void codeGen () const {}
	int getValue() const {return numValue;}
private:
	const int numValue;
};


struct ASTExpression: public ASTNode {
public:
	~ASTExpression() {}
	ASTExpression(const ASTIntegerConst* _left, const ASTExpression* _right): right(_right), left(_left) {
		vleft = NULL;
	}
	void codeGen() const override {
		//unsure of the purpose of this now
	}

	void codeGen(int destReg) const {
		if (left != NULL && aleft == NULL) {
				std::cout << "\t li\t$" << destReg << ", " << left->getValue() << std::endl;
		}
		if (right != NULL) {right->codeGen(destReg);}
	}
private:
	const ASTExpression* right;
	const ASTIntegerConst* left;
	const ASTVariableDeclaration* vleft;
};

struct ASTCastExpression: public ASTExpression {
public:
	ASTCastExpression(const std::string _Identifier): Identifier(_Identifier) {
		Cosntant = NULL;
		type = 0;
	}
	ASTCastExpression(const ASTIntegerConst* _Constant):Constant(_Constant){
		Identifier = "";
		type = 1;
	}
private:
	const std::string Identifier;
	const ASTIntegerConst* Constant;
	const int type;
};

struct ASTMultiplicativeExpression: public ASTExpression {
public:
	~ASTMultiplicativeExpression() {}
	ASTMultiplicativeExpression(const ASTAdditiveExpression* _right, const ASTMultiplicativeExpression* _left, const int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	void codeGen() const override {}
	void codeGen(int destReg) {
		if (operationFlag == 1) {
			ASTMultiplicativeExpression->codeGen(destReg);
			std::cout << "\t add\t$" << destReg << ", " << left->getValue() << std::endl;


		}
	}
private:
	const ASTMultiplicativeExpression* right;
	const ASTCastExpression* left;
	const int operationFlag;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;};
};

struct ASTAdditiveExpression: public ASTExpression {
public:
	~ASTAdditiveExpression() {}
	ASTAdditiveExpression(const ASTAdditiveExpression* _right, const ASTMultiplicativeExpression* _left, const int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	void codeGen() const override {}
	void codeGen(int destReg) {
		if (operationFlag == 1) {
			ASTMultiplicativeExpression->codeGen(destReg);
			std::cout << "\t add\t$" << destReg << ", " << left->getValue() << std::endl;


		}
	}
private:
	const ASTAdditiveExpression* right;
	const ASTMultiplicativeExpression* left;
	const int operationFlag;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};
