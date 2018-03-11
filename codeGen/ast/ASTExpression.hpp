#pragma once
struct ASTVariableDeclaration;

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
	ASTExpression() {}
	ASTExpression(const ASTIntegerConst* _left, const ASTExpression* _right, const std::string _vleft): right(_right), left(_left), vleft(_vleft) {}
	void codeGen() const override {
		//unsure of the purpose of this now
	}

	void codeGen(int destReg) const {
		if (left != NULL && vleft == "") {
				std::cout << "\t li\t$" << destReg << ", " << left->getValue() << std::endl;
		}
		if (right != NULL) {right->codeGen(destReg);}
	}
private:
	const ASTExpression* right;
	const ASTIntegerConst* left;
	const std::string vleft;
};


struct ASTMultiplicativeExpression: public ASTExpression {
public:
	~ASTMultiplicativeExpression() {}
	ASTMultiplicativeExpression(const ASTMultiplicativeExpression* _right, const ASTExpression* _left, const int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	void codeGen() const override {}
	void codeGen(int destReg) const{
		if (operationFlag == 1) {

			//TODO:ADD SUPPORT FOR ALL THESE THINGS BUT DO IT LATER!!
		}
	}
private:
	const ASTMultiplicativeExpression* right;
	const ASTExpression* left;
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
			left->codeGen(destReg);
			//std::cout << "\t add\t$" << destReg << ", " << left->getValue() << std::endl;
		}
	}
private:
	const ASTAdditiveExpression* right;
	const ASTMultiplicativeExpression* left;
	const int operationFlag;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};
