#pragma once
struct ASTVariableDeclaration;



struct ASTExpression: public ASTNode {
public:
	~ASTExpression() {}
	ASTExpression() {}
	ASTExpression(const ASTExpression* _left, const ASTExpression* _right, const std::string _vleft): right(_right), left(_left), vleft(_vleft) {}
	void codeGen() const {
		//unsure of the purpose of this now
	}
	void codeGen(std::vector<std::string> regIn) const {}
	int getregs() const{}
	void updateRegisterNeeds() const{
	}

private:
	const ASTExpression* right;
	const ASTExpression* left;
	const std::string vleft;
};

struct ASTIntegerConst: public ASTExpression {
public:
	~ASTIntegerConst() {}
	ASTIntegerConst(const int _numValue): numValue(_numValue) {}
	void codeGen (std::vector<std::string> regIn) const {
		std::string r1 = head(regIn);
		std::cout<< "\t li, " << r1 << ", " << numValue << std::endl;
	}	int getregs() const {return registerNeeds;}
	int getValue() const {return numValue;}
private:
	const int numValue;
	int registerNeeds = 1;
};

struct ASTVariableExp: public ASTExpression {
public:
	~ASTVariableExp() {}
	ASTVariableExp(const std::string _variableName): variableName(_variableName) {}
	int getregs() const {return registerNeeds;}
	void codeGen (std::vector<std::string> regIn) const {
		std::string r1 = head(regIn);
		std::cout<< "\t lw, " << r1 << ", " << "FIND LOCATION OF VARIABLE" << std::endl;
	}
private:
	const std::string variableName;
	int registerNeeds = 1;
};

struct ASTMultiplicativeExpression: public ASTExpression {
public:
	~ASTMultiplicativeExpression() {}
	ASTMultiplicativeExpression(const ASTExpression* _right, const ASTExpression* _left, const int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs() const {return registerNeeds;}
	void updateRegisterNeeds() const {
		left->getregs();
		right->getregs();
	}
	void codeGen(std::vector<std::string> regIn)  {
		this->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd, " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsub, " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd, " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsub, " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
			}
		}
	}
private:
	const ASTExpression* right;
	const ASTExpression* left;
	const int operationFlag;
	int registerNeeds;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;};
};

struct ASTAdditiveExpression: public ASTExpression {
public:
	~ASTAdditiveExpression() {}
	ASTAdditiveExpression(const ASTExpression* _right, const ASTExpression* _left, const int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs() const {return registerNeeds;}
	void updateRegisterNeeds() const {
		left->getregs();
		right->getregs();
	}
	void codeGen(std::vector<std::string> regIn)  {
		std::cout << "getting info" << std::endl;
		this->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd, " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsub, " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd, " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsub, " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
			}
		}
	}

private:
	const ASTExpression* right;
	const ASTExpression* left;
	const int operationFlag;
	int registerNeeds;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};

struct ASTAssignmentExpression: public ASTExpression {
public:
	~ASTAssignmentExpression() {}
	ASTAssignmentExpression(const ASTExpression* _variable, const ASTExpression* _EquivalentExp, const int _assignmentOp): variable(_variable), EquivalentExp(_EquivalentExp), assignmentOp(_assignmentOp) {}
	void codeGen() const {
		std::cout<<"im in here!"<<std::endl;
		EquivalentExp->codeGen(regList);
	}
private:
	const ASTExpression* variable;
	const int assignmentOp;
	const ASTExpression* EquivalentExp;
	//TODO: change the additive expression to the other thing
};
