#pragma once
struct ASTVariableDeclaration;



struct ASTExpression: public ASTNode {
public:
	~ASTExpression() {}
	ASTExpression() {}
	ASTExpression( ASTExpression* _left,  ASTExpression* _right,  std::string _vleft): right(_right), left(_left), vleft(_vleft) {}
	virtual int returnIndex () {}
	void codeGen()  {
		//unsure of the purpose of this now
	}
	virtual void codeGen(std::vector<std::string> regIn) {}
	virtual int getregs() {}
	virtual void updateRegisterNeeds() {}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	 std::string vleft;
};

struct ASTIntegerConst: public ASTExpression {
public:
	~ASTIntegerConst() {}
	ASTIntegerConst( int _numValue): numValue(_numValue) {}
	void updateRegisterNeeds() override {
	}
	void codeGen (std::vector<std::string> regIn)   override{
		std::string r1 = head(regIn);
		std::cout<< "\tli, " << r1 << ", " << numValue << std::endl;
	}
	int getregs()  override {return registerNeeds;}
	int getValue()  {return numValue;}
private:
	 int numValue;
	int registerNeeds = 1;
};

struct ASTVariableExp: public ASTExpression {
public:
	~ASTVariableExp() {}
	ASTVariableExp( std::string _variableName): variableName(_variableName) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds() override {
	}
	int returnIndex () override{
		return ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
	}
	void codeGen (std::vector<std::string> regIn) override {
		std::string r1 = head(regIn);
		int index = ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
		//TODO:MAKENSURE THAT THIS EQUATION WORKS PROPERLY
		std::cout<< "\tlw, " << r1 << ", " << index <<"($fp)" << std::endl;
	}
private:
	 std::string variableName;
	int registerNeeds = 1;
};

struct ASTMultiplicativeExpression: public ASTExpression {
public:
	~ASTMultiplicativeExpression() {}
	ASTMultiplicativeExpression( ASTExpression* _right,  ASTExpression* _left,  int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds() {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn)  override {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul " << r1 << ", " << r2 << std::endl;
					std::cout << "\tmov "<< r1 << ", $LO" << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv, " << r1 << ", " << r2 << std::endl;
					std::cout << "\tmov "<< r1 << ", $LO" << std::endl;
				} else {
					//MOD
					std::cout << "\tdiv, " << r1 << ", " << r2 << std::endl;
					std::cout << "mov "<< r1 << ", $HI" << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul " << r2 << ", " << r1 << std::endl;
					std::cout << "\tmov "<< r2 << ", $LO" << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv, " << r2 << ", " << r1 << std::endl;
					std::cout << "\tmov "<< r2 << ", $LO" << std::endl;
				} else {
					//MOD
					std::cout << "\tdiv, " << r2 << ", " << r1 << std::endl;
					std::cout << "\tmov "<< r2 << ", $HI" << std::endl;
				}
			}
		}
	}
private:
	 ASTExpression* right;
	 ASTExpression* left;
	 int operationFlag;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;};
};

struct ASTShiftExpression: public ASTExpression {
public:
	~ASTShiftExpression() {}
	ASTShiftExpression( ASTExpression* _right,  ASTExpression* _left,  int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tsllv " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsra " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tsllv " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsra " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	 int operationFlag;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};

struct ASTAdditiveExpression: public ASTExpression {
public:
	~ASTAdditiveExpression() {}
	ASTAdditiveExpression( ASTExpression* _right,  ASTExpression* _left,  int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsub " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsub " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	 int operationFlag;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};

struct ASTEqualityExpression: public ASTExpression {
public:
	~ASTEqualityExpression() {}
	ASTEqualityExpression( ASTExpression* _right,  ASTExpression* _left,  int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\txori" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu" << r1 << ", " << r1 << ", " << "1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else {
					std::cout << "\txor" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu" << r1 << ", " << "0" << ", " << r1 << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\txori" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu" << r1 << ", " << r1 << ", " << "1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else {
					std::cout << "\txori" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu" << r1 << ", " << r1 << ", " << "1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	 int operationFlag;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};

struct ASTRelationalExpression: public ASTExpression {
public:
	~ASTRelationalExpression() {}
	ASTRelationalExpression( ASTExpression* _right,  ASTExpression* _left,  int _operationFlag): right(_right), left(_left), operationFlag(_operationFlag) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					//less than
					std::cout << "\tslt" << r1 << ", " << r2 << ", " << r1 << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else if(operationFlag == 2){
					//more than
					std::cout << "\tslt" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout << "\tslt" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\txori" << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else {
					//more than or equal to
					std::cout << "\tslt" << r1 << ", " << r2 << ", " << r1 << std::endl;
					std::cout << "\txori" << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					//less than
					std::cout << "\tslt" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else if(operationFlag == 2){
					//more than
					std::cout << "\tslt" << r1 << ", " << r2 << ", " << r1 << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout << "\tslt" << r1 << ", " << r2 << ", " << r1 << std::endl;
					std::cout << "\txori" << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else {
					//more than or equal to
					std::cout << "\tslt" << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\txori" << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi" << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	 int operationFlag;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};


struct ASTInclusiveOr: public ASTExpression {
public:
	~ASTInclusiveOr() {}
	ASTInclusiveOr( ASTExpression* _right,  ASTExpression* _left): right(_right), left(_left){}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
					std::cout << "\tor " << r1 << ", " << r1 << ", " << r2 << std::endl;
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
					std::cout << "\tor " << r1 << ", " << r2 << ", " << r1 << std::endl;

			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};

struct ASTExclusiveOr: public ASTExpression {
public:
	~ASTExclusiveOr() {}
	ASTExclusiveOr( ASTExpression* _right,  ASTExpression* _left): right(_right), left(_left) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
					std::cout << "\txor " << r1 << ", " << r1 << ", " << r2 << std::endl;
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
					std::cout << "\txor " << r1 << ", " << r2 << ", " << r1 << std::endl;

			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};

struct ASTBitwiseAnd: public ASTExpression {
public:
	~ASTBitwiseAnd() {}
	ASTBitwiseAnd( ASTExpression* _right,  ASTExpression* _left): right(_right), left(_left) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds()  {
		left->updateRegisterNeeds();
		right->updateRegisterNeeds();
		if (left->getregs() == right->getregs()){
			registerNeeds = right->getregs() + 1;
		} else {
			registerNeeds = std::max(right->getregs(), left->getregs());
		}
	}
	void codeGen(std::vector<std::string> regIn) override  {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));

					std::cout << "\txor " << r1 << ", " << r1 << ", " << r2 << std::endl;

			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
					std::cout << "\txor " << r1 << ", " << r2 << ", " << r1 << std::endl;

			}
		}
	}

private:
	 ASTExpression* right;
	 ASTExpression* left;
	int registerNeeds = 1;
	//TODO: SUPPORT GOING THRU AND FINDING THE STUFF;
};




struct ASTAssignmentExpression: public ASTExpression {
public:
	~ASTAssignmentExpression() {}
	ASTAssignmentExpression( ASTExpression* _variable,  ASTExpression* _EquivalentExp,  int _assignmentOp): variable(_variable), EquivalentExp(_EquivalentExp), assignmentOp(_assignmentOp) {}
	void codeGen()  {
		EquivalentExp->updateRegisterNeeds();
		EquivalentExp->codeGen(regList);
		//now youll store v0 to the variable;
		std::cout<<"\tsw $v0," << variable->returnIndex() << "($fp)" << std::endl;
	}
private:
	 ASTExpression* variable;
	 int assignmentOp;
	 ASTExpression* EquivalentExp;
	//TODO: change the additive expression to the other thing
};
