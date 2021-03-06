#pragma once
struct ASTVariableDeclaration;



struct ASTExpression: public ASTNode {
public:
	~ASTExpression() {}
	ASTExpression() {}
	ASTExpression( ASTExpression* _left,  ASTExpression* _right,  std::string _vleft): right(_right), left(_left), vleft(_vleft) {}
	virtual void codeGen(std::vector<std::string> regIn) {}
	virtual int returnIndex () {}
	virtual void codeGen()  {
		if(isFloat == 1 || isDouble == 1) {
			this->codeGen(regListFloat);
			return;
		}
		this->codeGen(regList);
	}
	virtual std::string nameretrieval() {return "virutal";}
	virtual int countArgs() {}
	virtual int getregs() {}
	virtual void updateRegisterNeeds() {}
	virtual void pushVariables() {}
	virtual void updateArgNumber(int index) {}
private:
	 ASTExpression* right;
	 ASTExpression* left;
	 std::string vleft;
};

struct ASTNumConst: public ASTExpression {
public:
	~ASTNumConst() {}
	ASTNumConst( int _numValue): numValue(_numValue) {}
	void updateRegisterNeeds() override {
	}
	void codeGen (std::vector<std::string> regIn)   override{
		std::string r1 = head(regIn);
		if (isFloat == 1) {
			unsigned int ui;
			memcpy(&ui, &numValue, sizeof(ui));
			floatValues.push_back(ui);
			std::string newAddress = uniqueIdGenFloat();
			std::cout<<"\tlui " << "$t0,%hi(" << newAddress << ")" << std::endl;
			std::cout<<"\tlwc1 " << r1 << ",%lo(" << newAddress << ")($t0)" << std::endl;
			return;
		} else if (isDouble == 1){
			unsigned int ui;
			memcpy(&ui, &numValue, sizeof(ui));
			floatValues.push_back(ui);
			std::string newAddress = uniqueIdGenFloat();
			std::cout<<"\tlui " << "$t0,%hi(" << newAddress << ")" << std::endl;
			std::cout<<"\tldc1 " << r1 << ",%lo(" << newAddress << ")($t0)" << std::endl;
			return;
		}
		std::cout<< "\tli " << r1 << ", " << numValue << std::endl;
	}
	int getregs()  override {return registerNeeds;}
	int getValue()  {return numValue;}
private:
	 float numValue;
	int registerNeeds = 1;
};


struct ASTVariableExp: public ASTExpression {
public:
	~ASTVariableExp() {}
	std::string nameretrieval() override {
		return variableName;
	}
	ASTVariableExp( std::string _variableName): variableName(_variableName) {}
	int getregs()  override {return registerNeeds;}
	void updateRegisterNeeds() override {
	}
	int returnIndex () override{
		if (findVariableIndex(allVariables, variableName) == -1) {
			return -1;
		}
		return ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
	}
	void codeGen (std::vector<std::string> regIn) override {
		//ok this part is wrong cause i then need to check for all scope;
		std::string r1 = head(regIn);
		if (isFloat == 0 && isDouble == 0) {
		if (findVariableIndex(allVariables, variableName) == -1) {
			//save $2 in the stack below
			//this is me winging it it probably doesnt work but why not;
			std::cout<<"\tlw " << r1 <<",%got("<<variableName<<")($28)" << std::endl;
			std::cout<<"\tlw " << r1 << ",                              0(" << r1 << ")" << std::endl;
		} else {
			int index = ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
			//TODO:MAKENSURE THAT THIS EQUATION WORKS PROPERLY
			if (index > NumberofVaraibles * 4) {
				int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
			    Framesize = ((NumberofVaraibles + 12 + 20 + maxArgs + 1) * 4) + ((NumberofVaraibles + maxArgs + 12 + 20 + 1) * 4) % 8;
			  } else {
			    //used to be 8 now i changed it to fit the new registers
			    Framesize = ((20 + 12 +  maxArgs + 1) * 4) + (((maxArgs + 12 + 20 + 1) * 4)%8);
			  }
					std::cout<< "\tlw " << r1 << ", " << index + Framesize  - 4 <<"($fp)" << std::endl;
			} else {
			std::cout<< "\tlw " << r1 << ", " << (index + (maxArgs * 4))  <<"($fp)" << std::endl;
		}
		}
	} else if (isFloat == 1) {
		if (findVariableIndex(allVariables, variableName) == -1) {
			//save $2 in the stack below
			//this is me winging it it probably doesnt work but why not;
			std::cout<<"\tlwc1 " << r1 <<",%got("<<variableName<<")($28)" << std::endl;
			std::cout<<"\tlwc1 " << r1 << ",                              0(" << r1 << ")" << std::endl;
		} else {
			int index = ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
			//TODO:MAKENSURE THAT THIS EQUATION WORKS PROPERLY
			if (index > NumberofVaraibles * 4) {
				int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 12 + 20 + maxArgs + 1) * 4) + ((NumberofVaraibles + 12 + maxArgs + 20 + 1) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + 12 + maxArgs + 1) * 4) + (((maxArgs + 12 + 20 + 1) * 4)%8);
				}
					std::cout<< "\tlwc1 " << r1 << ", " << index + Framesize  - 4 <<"($fp)" << std::endl;
			} else {
			std::cout<< "\tlwc1 " << r1 << ", " << (index + (maxArgs * 4))  <<"($fp)" << std::endl;
		}
		}
	} else {
		if (findVariableIndex(allVariables, variableName) == -1) {
			//save $2 in the stack below
			//this is me winging it it probably doesnt work but why not;
			std::cout<<"\tldc1 " << r1 <<",%got("<<variableName<<")($28)" << std::endl;
			std::cout<<"\tldc1 " << r1 << ",                              0(" << r1 << ")" << std::endl;
		} else {
			int index = ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
			//TODO:MAKENSURE THAT THIS EQUATION WORKS PROPERLY
			if (index > NumberofVaraibles * 4) {
				int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 12 + 20 + maxArgs + 1) * 4) + ((NumberofVaraibles + 12 + maxArgs + 20 + 1) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + 12 + maxArgs + 1) * 4) + (((maxArgs + 12 + 20 + 1) * 4)%8);
				}
					std::cout<< "\tldc1 " << r1 << ", " << index + Framesize  - 4 <<"($fp)" << std::endl;
			} else {
			std::cout<< "\tldc1 " << r1 << ", " << (index + (maxArgs * 4))  <<"($fp)" << std::endl;
		}
		}
	}
	}
	void codeGen() override {
		codeGen(regList);
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
		if (isFloat == 0 && isDouble == 0) {
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul " << r2 << ", " << r1 << std::endl;
					std::cout << "\tmflo "<< r1 << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv " << r2 << ", " << r1 << std::endl;
					std::cout << "\tmflo "<< r1 << std::endl;
				} else {
					//MOD
					std::cout << "\tdiv " << r2 << ", " << r1 << std::endl;
					std::cout << "\tmfhi "<< r1 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul " << r1 << ", " << r2 << std::endl;
					std::cout << "\tmflo "<< r2 << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv " << r1 << ", " << r2 << std::endl;
					std::cout << "\tmflo "<< r2 << std::endl;
				} else {
					//MOD
					std::cout << "\tdiv " << r1 << ", " << r2 << std::endl;
					std::cout << "\tmfhi "<< r2 << std::endl;
				}
			}
		}
	} else if (isFloat == 1){
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul.s " << r1 << ", " << r2 << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv.s " << r1 << ", " << r2 << std::endl;
				} else {

				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul.s " << r2 << ", " << r1 << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv.s " << r2 << ", " << r1 << std::endl;
				} else {

				}
			}
		}
	} else {
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul.s " << r1 << ", " << r2 << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv.s " << r1 << ", " << r2 << std::endl;
				} else {

				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tmul.s " << r2 << ", " << r1 << std::endl;
				} else if (operationFlag == 2){
					std::cout << "\tdiv.s " << r2 << ", " << r1 << std::endl;
				} else {

				}
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
		if (isFloat == 0 && isDouble == 0 && isDouble != 1) {

		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\taddu " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsubu " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\taddu " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsubu " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
			}
		}
	} else {
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else if (isDouble == 1) {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd.d " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsub.d " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd.d " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsub.d " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
			}
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd.s " << r1 << ", " << r1 << ", " << r2 << std::endl;
				} else {
					std::cout << "\tsub.s " << r1 << ", " << r1 << ", " << r2 << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tadd.s " << r1 << ", " << r2 << ", " << r1 << std::endl;
				} else {
					std::cout << "\tsub.s " << r1 << ", " << r2 << ", " << r1 << std::endl;
				}
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
		if (isFloat == 0 && isDouble == 0) {
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\txor " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu " << r1 << ", " << r1 << ", " << "1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else {
					std::cout << "\txor " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu " << r1 << ", " << "$0" << ", " << r1 << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\txor " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu " << r1 << ", " << r1 << ", " << "1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else {
					std::cout << "\txor " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tsltu " << r1 << ", " << r1 << ", " << "1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			}
		}
	} else {
		std::string firstconditionAddress = uniqueIdGen();
		std::string secondConditionAddress = uniqueIdGen();
		std::string tempfloat= uniqueIdGenFloat();
		floatValues.push_back(1);
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));

				if (operationFlag == 1){
					std::cout << "\tc.eq.s $fcc0," << r1 << ", " << r2 << std::endl;
					std::cout << "\tbc1f $fcc0," << firstconditionAddress << std::endl;
					std::cout <<"\tnop\n" << std::endl;
					std::cout << "\tlui $2,%hi(" << tempfloat << ")" << std::endl;
					std::cout << "\tlwc1 " << r1 << ",%lo(" << tempfloat << ")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " << secondConditionAddress << std::endl;
					std::cout << "\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout << firstconditionAddress << ":" << std::endl;
					std::cout << "\tmtc1 $0," << r1 << std::endl;
					std::cout << secondConditionAddress << ":" << std::endl;
				} else {
					std::cout << "\tc.eq.s $fcc0," << r1 << ", " << r2 << std::endl;
					std::cout << "\tbc1t $fcc0," << firstconditionAddress << std::endl;
					std::cout <<"\tnop\n" << std::endl;
					std::cout << "\tlui $2,%hi(" << tempfloat << ")" << std::endl;
					std::cout << "\tlwc1 " << r1 << ",%lo(" << tempfloat << ")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " << secondConditionAddress << std::endl;
					std::cout << "\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout << firstconditionAddress << ":" << std::endl;
					std::cout << "\tmtc1 $0," << r1 << std::endl;
					std::cout << secondConditionAddress << ":" << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					std::cout << "\tc.eq.s $fcc0," << r1 << ", " << r2 << std::endl;
					std::cout << "\tbc1f $fcc0," << firstconditionAddress << std::endl;
					std::cout <<"\tnop\n" << std::endl;
					std::cout << "\tlui $2,%hi(" << tempfloat << ")" << std::endl;
					std::cout << "\tlwc1 " << r2 << ",%lo(" << tempfloat << ")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " << secondConditionAddress << std::endl;
					std::cout << "\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout << firstconditionAddress << ":" << std::endl;
					std::cout << "\tmtc1 $0," << r2 << std::endl;
					std::cout << secondConditionAddress << ":" << std::endl;

				} else {
					std::cout << "\tc.eq.s $fcc0," << r1 << ", " << r2 << std::endl;
					std::cout << "\tbc1t $fcc0," << firstconditionAddress << std::endl;
					std::cout <<"\tnop\n" << std::endl;
					std::cout << "\tlui $2,%hi(" << tempfloat << ")" << std::endl;
					std::cout << "\tlwc1 " << r2 << ",%lo(" << tempfloat << ")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " << secondConditionAddress << std::endl;
					std::cout << "\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout << firstconditionAddress << ":" << std::endl;
					std::cout << "\tmtc1 $0," << r2 << std::endl;
					std::cout << secondConditionAddress << ":" << std::endl;
				}
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
		if (isFloat == 0 && isDouble == 0) {
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				if (operationFlag == 1){
					//less than
					std::cout << "\tslt " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else if(operationFlag == 2){
					//more than
					std::cout << "\tslt " << r1 << ", " <<  r2<< ", " <<  r1<< std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout << "\tslt " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\txori " << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else {
					//more than or equal to
					std::cout << "\tslt " << r1 << ", " << r2 << ", " << r1 << std::endl;
					std::cout << "\txori " << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			} else {
				right->codeGen(regIn);
				left->codeGen(tail(regIn));
				if (operationFlag == 1){
					//less than
					std::cout << "\tslt " << r1 << ", " << r2 << ", " <<  r1 << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;

				} else if(operationFlag == 2){
					//more than
					std::cout << "\tslt " << r1 << ", " << r1 << ", " << r2<< std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout << "\tslt " << r1 << ", " << r2 << ", " << r1 << std::endl;
					std::cout << "\txori " << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				} else {
					//more than or equal to
					std::cout << "\tslt " << r1 << ", " << r1 << ", " << r2 << std::endl;
					std::cout << "\txori " << r1 << ", " << r1 << ", " << "0x1" << std::endl;
					std::cout << "\tandi " << r1 << ", " << r1 << ", " << "0x00ff" << std::endl;
				}
			}
		}
	} else if (isFloat == 1){
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				std::string firstconditionAddress = uniqueIdGen();
				std::string secondConditionAddress = uniqueIdGen();
				std::string floattemp = uniqueIdGenFloat();
				floatValues.push_back(1);
				if (operationFlag == 1){
					//less than
					std::cout<<"\tc.lt.s $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r1 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if(operationFlag == 2){
					//more than
						std::cout<<"\tc.lt.s $fcc0," << r2 << "," << r1 << std::endl;
						std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
						std::cout<<"\tnop\n"<<std::endl;
						std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
						std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
						std::cout << "\t.option pic0" << std::endl;
						std::cout << "\tb " <<  firstconditionAddress << std::endl;
						std::cout<<"\tnop\n" << std::endl;
						std::cout << "\t.option pic2" << std::endl;
						std::cout<<secondConditionAddress<<":"<<std::endl;
						std::cout<<"\tmtc1 $0," << r1 << std::endl;
						std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout<<"\tc.le.s $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r1 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else {
					//more than or equal to
					std::cout<<"\tc.le.s $fcc0," << r2 << "," << r1 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r1 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				}
			} else {
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				std::string firstconditionAddress = uniqueIdGen();
				std::string secondConditionAddress = uniqueIdGen();
				std::string floattemp = uniqueIdGenFloat();
				if (operationFlag == 1){
					//less than
					std::cout<<"\tc.lt.s $fcc0," << r2 << "," << r1 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if(operationFlag == 2){
					//more than
					std::cout<<"\tc.lt.s $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout<<"\tc.le.s $fcc0," << r2 << "," << r1 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
				} else {
					//more than or equal to
					std::cout<<"\tc.le.s $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				}
			}
		}
	} else {
		if(left->getregs() >= regIn.size() && right->getregs() >= regIn.size()){
			//THIS PART HANDLES REGISTER SPILLIGBUT ILL DO IT LATER
		} else {
			if(left->getregs() >= right->getregs()){
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				std::string firstconditionAddress = uniqueIdGen();
				std::string secondConditionAddress = uniqueIdGen();
				std::string floattemp = uniqueIdGenFloat();
				floatValues.push_back(1);
				if (operationFlag == 1){
					//less than
					std::cout<<"\tc.lt.s $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r1 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if(operationFlag == 2){
					//more than
						std::cout<<"\tc.lt.s $fcc0," << r2 << "," << r1 << std::endl;
						std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
						std::cout<<"\tnop\n"<<std::endl;
						std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
						std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
						std::cout << "\t.option pic0" << std::endl;
						std::cout << "\tb " <<  firstconditionAddress << std::endl;
						std::cout<<"\tnop\n" << std::endl;
						std::cout << "\t.option pic2" << std::endl;
						std::cout<<secondConditionAddress<<":"<<std::endl;
						std::cout<<"\tmtc1 $0," << r1 << std::endl;
						std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout<<"\tc.le.s $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r1 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else {
					//more than or equal to
					std::cout<<"\tc.le.s $fcc0," << r2 << "," << r1 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tlwc1 " <<  r1 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r1 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				}
			} else {
				left->codeGen(regIn);
				right->codeGen(tail(regIn));
				std::string firstconditionAddress = uniqueIdGen();
				std::string secondConditionAddress = uniqueIdGen();
				std::string floattemp = uniqueIdGenFloat();
				if (operationFlag == 1){
					//less than
					std::cout<<"\tc.lt.d $fcc0," << r2 << "," << r1 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tldc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if(operationFlag == 2){
					//more than
					std::cout<<"\tc.lt.d $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tldc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				} else if (operationFlag == 3){
					//less than or equal to
					std::cout<<"\tc.le.d $fcc0," << r2 << "," << r1 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tldc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
				} else {
					//more than or equal to
					std::cout<<"\tc.le.d $fcc0," << r1 << "," << r2 << std::endl;
					std::cout<<"\tbc1f $fcc0," << secondConditionAddress << std::endl;
					std::cout<<"\tnop\n"<<std::endl;
					std::cout<<"\tlui $2,%hi("<<floattemp<<")"<<std::endl;
					std::cout<<"\tldc1 " <<  r2 <<",%lo("<<floattemp<<")($2)" << std::endl;
					std::cout << "\t.option pic0" << std::endl;
					std::cout << "\tb " <<  firstconditionAddress << std::endl;
					std::cout<<"\tnop\n" << std::endl;
					std::cout << "\t.option pic2" << std::endl;
					std::cout<<secondConditionAddress<<":"<<std::endl;
					std::cout<<"\tmtc1 $0," << r2 << std::endl;
					std::cout<<firstconditionAddress<<":"<<std::endl;
				}
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
		if (allVariables[findVariableIndex(allVariables, variable->nameretrieval())].getType() == 5) {
			isFloat = 1;
			EquivalentExp->codeGen(regListFloat);

		} else if (allVariables[findVariableIndex(allVariables, variable->nameretrieval())].getType() == 6) {
			isDouble = 1;
			EquivalentExp->codeGen(regListFloat);
		} else {
		EquivalentExp->codeGen(regList);
	}
		//now youll store v0 to the variable;
		if (isFloat == 0 && isDouble == 0) {
		switch(assignmentOp){
			case 0:
				break;
			case 1:
				//*=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tmul $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$LO"<<std::endl;
					} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
						NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
						if (NumberofVaraibles != 0) {
						Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
					} else {
						//used to be 8 now i changed it to fit the new registers
						Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
					}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tmul $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$LO"<<std::endl;
					} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tmul $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$LO"<<std::endl;
				}

				break;
			case 2:
				///=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tdiv $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$LO"<<std::endl;
					} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tdiv $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$LO"<<std::endl;
					} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tdiv $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$LO"<<std::endl;
				}

				break;
			case 3:
				//%=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tdiv $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$HI"<<std::endl;
					} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tdiv $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$HI"<<std::endl;
					} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tdiv $t9,$t0"<<std::endl;
					std::cout<<"\tmov $t0,$HI"<<std::endl;
				}
				break;
			case 4:
				//+=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tadd $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tadd $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tadd $t0,$t9,$t0"<<std::endl;
				}
				break;
			case 5:
				//-=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tsub $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tsub $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tsub $t0,$t9,$t0"<<std::endl;
				}
				break;
			case 6:
				//>>=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tsrlv $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tsrlv $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tsrlv $t0,$t9,$t0"<<std::endl;
				}
				break;
			case 7:
				//<<=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tsllv $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tsllv $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tsllv $t0,$t9,$t0"<<std::endl;
				}
				break;
			case 8:
				//&=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tand $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tand $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tand $t0,$t9,$t0"<<std::endl;
				}
				break;
			case 9:
				//^=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\txor $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\txor $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\txor $t0,$t9,$t0"<<std::endl;
				}
				break;
			case 10:
				//|=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $t9,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tor $t0,$t9,$t0"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlw $t9," << variable->returnIndex() + Framesize - 4  << "($fp)" << std::endl;
					std::cout<<"\tor $t0,$t9,$t0"<<std::endl;
				} else {
					std::cout<<"\tlw $t9," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tor $t0,$t9,$t0"<<std::endl;
				}
				break;
		}
	} else {
		switch(assignmentOp){
			case 0:
				break;
			case 1:
				//*=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $f18,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tmul.s $f4,$f4,$f18"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + Framesize - 4 << "($fp)" << std::endl;
					std::cout<<"\tmul.s $f4,$f4,$f18"<<std::endl;
				} else {
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tmul.s $f4,$f4,$f18"<<std::endl;
				}
				break;
			case 2:
				///=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $f18,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tdiv.s $f4,$f4,$f18"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + Framesize - 4 << "($fp)" << std::endl;
					std::cout<<"\tdiv.s $f4,$f4,$f18"<<std::endl;
				} else {
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tdiv.s $f4,$f4,$f18"<<std::endl;
				}
				break;
			case 4:
				//+=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $f18,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tadd.s $f4,$f4,$f18"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + Framesize - 4 << "($fp)" << std::endl;
					std::cout<<"\tadd.s $f4,$f4,$f18"<<std::endl;
				} else {
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tadd.s $f4,$f4,$f18"<<std::endl;
				}
				break;
			case 5:
				//-=
				if (variable->returnIndex() == -1) {
					std::cout<<"\tlw $f18,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
					std::cout<<"\tsub.s $f4,$f4,$f18"<<std::endl;
				} else if (variable->returnIndex() > NumberofVaraibles * 4) {
					int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
					Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
				} else {
					//used to be 8 now i changed it to fit the new registers
					Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
				}
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + Framesize - 4 << "($fp)" << std::endl;
					std::cout<<"\tsub.s $f4,$f4,$f18"<<std::endl;
				} else {
					std::cout<<"\tlwc1 $f18," << variable->returnIndex() + (maxArgs * 4) << "($fp)" << std::endl;
					std::cout<<"\tsub.s $f4,$f4,$f18"<<std::endl;
				}
				break;
			}
	}
		if (isFloat == 0 && isDouble == 0) {
		if (variable->returnIndex() == -1) {
			std::cout<<"\tlw $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tsw $t0,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
			NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
			if (NumberofVaraibles != 0) {
			Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
		} else {
			//used to be 8 now i changed it to fit the new registers
			Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
		}
				std::cout<< "\tsw $t0," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tsw $t0," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	} else {
		if (variable->returnIndex() == -1) {
			std::cout<<"\tlw $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tswc1 $f4,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
			NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
			if (NumberofVaraibles != 0) {
			Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
		} else {
			//used to be 8 now i changed it to fit the new registers
			Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
		}
				std::cout<< "\tswc1 $f4," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tswc1 $f4," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	}
	isFloat = 0;
	isDouble = 0;
}

	void codeGen(std::vector<std::string> regList) override {
		codeGen();
	}
private:
	 ASTExpression* variable;
	 int assignmentOp;
	 ASTExpression* EquivalentExp;
	//TODO: change the additive expression to the other thing
};

struct ASTArgumentList: public ASTExpression {
public:
  ASTArgumentList(ASTExpression* _Argument, ASTExpression* _NextArgument): Argument(_Argument), NextArgument(_NextArgument) {}
	void codeGen() override {
		 	//TODO: SAVE THE REGISTERS BEFORE YOU GO INTO AN EVALUATION
			//I NEED TO SAVE ALL THE REGISTERS JUST INCASE THERE USED IN EVALUATION;
			Argument->codeGen();
			//now the result will be in v0 so we need to move it to its appropriate registers
			switch(argNumber){
				case 0:
				std::cout << "\tmove $a0, $t0" << std::endl;
				break;
				case 1:
				std::cout << "\tmove $a1, $t0" << std::endl;
				break;
				case 2:
				std::cout << "\tmove $a2, $t0" << std::endl;
				break;
				case 3:
				std::cout << "\tmove $a3, $t0" << std::endl;
				break;
				default:
				std::cout << "\tsw $t0,"<< argNumber * 4 <<"($fp)" << std::endl;
			}
			//OR we can pop it to the frame
			//POPBACK THE REGISTERS AFTER EVALUATION
			if (NextArgument != NULL ) {
				NextArgument->codeGen();
			}
	}
	int countArgs() override{
		if(NextArgument != NULL){
			return NextArgument->countArgs() + 1;
		} else {
			return 1;
		}
	}
	void updateArgNumber(int index) {
		argNumber = index;
		if (NextArgument != NULL){
			NextArgument->updateArgNumber(index + 1);
		}
	}
private:
  ASTExpression* Argument;
  ASTExpression* NextArgument;
	int argNumber;
};

struct ASTFunctionCall: public ASTExpression {
public:
	ASTFunctionCall(std::string _FunctionName, ASTExpression* _ArgsList): FunctionName(_FunctionName), ArgsList(_ArgsList) {
		isPrototype = std::find(listOfFunctions.begin(), listOfFunctions.end(), FunctionName) == listOfFunctions.end();
	}
	void codeGen() override {
		if (isPrototype) {

			//this is the part where we do that cool handle;
			std::cout<<"\tlui $28,%hi(__gnu_local_gp)"<<std::endl;
			std::cout<<"\taddiu	$28,$28,%lo(__gnu_local_gp)"<<std::endl;
			if (ArgsList->countArgs() <= 4) {
				std::cout<<"\t.cprestore	16" <<std::endl;

			} else {
			std::cout<<"\t.cprestore	"<< ArgsList->countArgs() <<std::endl;

			}
			//TODO:load the arguments now
			ArgsList->updateArgNumber(0);
			ArgsList->codeGen();
			std::cout<<"\tlw	$2,%call16("<<FunctionName<<")($28)"<<std::endl;
			std::cout<<"\tmove $25,$2"<<std::endl;
			std::cout<<"\t.reloc	1f,R_MIPS_JALR,"<<FunctionName<<std::endl;
			std::cout<<"1:	jalr $25" << std::endl << "\tnop" << std::endl;
		} else {
			//codeGen the arguments to v0 then place them to where they need to be;
			ArgsList->updateArgNumber(0);
			ArgsList->codeGen();
			std::cout<<"\t.option	pic0"<<std::endl;
			std::cout<<"\tjal	"<< FunctionName << std::endl;
			std::cout<<"\tnop	\n" << std::endl;
			std::cout<<"\t.option pic2" << std::endl << std::endl;
			//I HAVE NOTHING TO MOVE THE RESULT BACK TO WHERE IT NEEDS TO BE!!!!
		}
	}
	void codeGen(std::vector<std::string> regIn) {
		std::string r1 = head(regIn);
		//i need to move the result to where it needs to be!
		this->codeGen();
		std::cout<<"\tnop"<< std::endl;
		std::cout<<"\tmove " << r1 << ",$v0"<< std::endl;

	}


	void pushVariables() override {
		//we will use this function to update the global variable of MAXargs
		if (ArgsList == NULL){
			maxArgs = std::max(0, maxArgs);
		} else {
			maxArgs = std::max(ArgsList->countArgs(), maxArgs);
		}
	}
private:
	std::string FunctionName;
	bool isPrototype;
	ASTExpression* ArgsList;
};

std::string uniqueIdGen();
struct ASTLogicANDExpression: public ASTExpression {
	~ASTLogicANDExpression() {}
	ASTLogicANDExpression(ASTExpression* _left, ASTExpression* _right): left(_left), right(_right) {}
	void codeGen(std::vector<std::string> regIn) {
		left->codeGen(regIn);
		std::string r1 = head(regIn);
		std::string firstconditionAddress = uniqueIdGen();
		std::string secondConditionAddress = uniqueIdGen();
		if(isFloat == 1) {
		std::cout<<"\tmtc1 $0,$f2"<<std::endl;
		std::cout<<"\tc.eq.s	$fcc0," << r1<<",$f2"<<std::endl;
		std::cout<<"\tbc1t $fcc0," << firstconditionAddress << std::endl;
		std::cout<<"\tnop\n"<<std::endl;
		right->codeGen(regIn);
		std::cout<<"\tmtc1 $0,$f2"<<std::endl;
		std::cout<<"\tc.eq.s	$fcc0," << r1<<",$f2"<<std::endl;
		std::cout<<"\tbc1t $fcc0," << firstconditionAddress << std::endl;
		std::cout<<"\tli	$2,1"<<std::endl;
		std::cout<<"\t.option	pic0"<<std::endl;
		std::cout<<"\tb "<< secondConditionAddress <<std::endl;
		std::cout<<"\tnop\n"<<std::endl;
		std::cout<<"\t.option	pic2"<<std::endl;
		std::cout<<firstconditionAddress<<":"<<std::endl;
		std::cout<<"\tmove $2,$0" << std::endl;
		std::cout<<secondConditionAddress<<":"<<std::endl;
		std::cout<<"\tmtc1 $2,"<< r1 << std::endl;
		std::cout<<"\tcvt.s.w "<< r1 << ","<< r1 << std::endl;
		} else {

		std::cout<<"\t beq " << r1 << ",$0," << firstconditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		right->codeGen(regIn);
		std::cout<<"\tbeq " << r1 << ",$0," << firstconditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<"\tli " << r1 << ",1" << std::endl;
		std::cout<<"\t.option pic0" << std::endl;
		std::cout << "\tb " << secondConditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<< firstconditionAddress << ":" << std::endl;
		std::cout << "\tmove " << r1 << ",$0" << std::endl;
		std::cout << secondConditionAddress << ":" << std::endl;
	}
	}
private:
	ASTExpression* left;
	ASTExpression* right;
};

struct ASTLogicORExpression: public ASTExpression {
	~ASTLogicORExpression() {}
	ASTLogicORExpression(ASTExpression* _left, ASTExpression* _right): left(_left), right(_right) {}
	void codeGen(std::vector<std::string> regIn) {

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		std::string firstconditionAddress = uniqueIdGen();
		std::string secondConditionAddress = uniqueIdGen();
		std::string thirdConditionAddress = uniqueIdGen();
		if (isFloat == 0 && isDouble == 0) {
		left->codeGen(regIn);
		std::cout<<"\tbne " << r1 << ",$0," << firstconditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		right->codeGen(regIn);
		std::cout<<"\tbeq " << r1 << ",$0," << secondConditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout << firstconditionAddress << ":" << std::endl;
		std::cout<<"\tli " << r1 << ",1" << std::endl;
		std::cout<<"\t.option pic0" << std::endl;
		std::cout << "\tb " << thirdConditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<< secondConditionAddress << ":" << std::endl;
		std::cout << "\tmove " << r1 << ",$0" << std::endl;
		std::cout << thirdConditionAddress << ":" << std::endl;
	} else {
		left->codeGen(regIn);
		std::cout<<"mtc1 $0," << r2 << std::endl;
		std::cout<<"\tc.eq.s $fcc0," << r1 << "," << r2 << std::endl;
		std::cout<<"\tbc1f $fcc0," << firstconditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		right->codeGen(regIn);
		std::cout<<"mtc1 $0," << r2 << std::endl;
		std::cout<<"\tc.eq.s $fcc0," << r1 << "," << r2 << std::endl;
		std::cout<<"\tbc1t $fcc0," << secondConditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<firstconditionAddress<<":"<<std::endl;
		std::cout<<"\tli $2,1" << std::endl;
		std::cout<<"\t.option pic0" << std::endl;
		std::cout<<"\tb " << thirdConditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<<"mtc1 $0," << r1 << std::endl;
		std::cout<<secondConditionAddress<<":"<<std::endl;
		std::cout<<"\tmove $2,$0" << std::endl;
		std::cout<<thirdConditionAddress <<":"<<std::endl;
		std::cout<<"\tmtc1 $2,"<<r1<<std::endl;
		std::cout<<"\tcvt.s.w "<<r1<<","<<r1<<std::endl;
			}
	}
private:
	ASTExpression* left;
	ASTExpression* right;
};

struct ASTLogicNotExpression: public ASTExpression {
~ASTLogicNotExpression() {}
ASTLogicNotExpression(ASTExpression* _exp): exp(_exp) {}
void codeGen(std::vector<std::string> regIn) {
	std::string r1 = head(regIn);
	if (isFloat == 1) {
		std::string temp = uniqueIdGen();
		std::string temp1 = uniqueIdGen();
		std::string tempFloat = uniqueIdGenFloat();
		floatValues.push_back(1);
		exp->codeGen(regIn);
		std::cout<<"\tmtc1 $0,$f2" << std::endl;
		std::cout<<"\tc.eq.s $fcc0," << r1 << ",$f2" << std::endl;
		std::cout<<"\tbc1f $fcc0, " << temp << std::endl;
		std::cout<< "\tnop\n" << std::endl;
		std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
		std::cout<<"\tlwc1 " << r1 << ",%lo(" << tempFloat <<")($2)" << std::endl;
		std::cout<<"\t.option pic0" << std::endl;
		std::cout<<"\tb " << temp1 << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<<temp <<":" << std::endl;
		std::cout<<"\tmtc1 $0," << r1 << std::endl;
		std::cout<<temp1<<":" << std::endl;
	} else if (isDouble == 1){
		std::string temp = uniqueIdGen();
		std::string temp1 = uniqueIdGen();
		std::string tempFloat = uniqueIdGenFloat();
		floatValues.push_back(1);
		exp->codeGen(regIn);
		std::cout<<"\tmtc1 $0,$f2" << std::endl;
		std::cout<<"\tc.eq.d $fcc0," << r1 << ",$f2" << std::endl;
		std::cout<<"\tbc1f $fcc0, " << temp << std::endl;
		std::cout<< "\tnop\n" << std::endl;
		std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
		std::cout<<"\tldc1 " << r1 << ",%lo(" << tempFloat <<")($2)" << std::endl;
		std::cout<<"\t.option pic0" << std::endl;
		std::cout<<"\tb " << temp1 << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<<temp <<":" << std::endl;
		std::cout<<"\tmtc1 $0," << r1 << std::endl;
		std::cout<<temp1<<":" << std::endl;
} else {
	exp->codeGen(regIn);
	std::cout<<"\tsltu " << r1 << "," << r1 << ",1"<<std::endl;
	std::cout<<"\tandi " << r1 << "," << r1 << ",0x00ff" << std::endl;
}
}
private:
	ASTExpression* exp;
};

struct ASTbitwiseNotExpression: public ASTExpression {
~ASTbitwiseNotExpression() {}
ASTbitwiseNotExpression(ASTExpression* _exp): exp(_exp) {}
void codeGen(std::vector<std::string> regIn) {
	std::string r1 = head(regIn);
	exp->codeGen(regIn);
	std::cout<<"\tnor " << r1 << "," << r1 << ",$0" << std::endl;
}
private:
	ASTExpression* exp;
};

struct ASTNegativeOperator: public ASTExpression {
~ASTNegativeOperator() {}
ASTNegativeOperator(ASTExpression* _exp): exp(_exp) {}
void codeGen(std::vector<std::string> regIn) {
	std::string r1 = head(regIn);
	exp->codeGen(regIn);
	std::cout<<"\tsubu " << r1 << ",$0," << r1  << std::endl;
}
private:
	ASTExpression* exp;
};


struct ASTConditionalExpression: public ASTExpression {
public:
~ASTConditionalExpression() {}
ASTConditionalExpression(ASTExpression* _condition, ASTExpression* _left, ASTExpression* _right): condition(_condition), left(_left), right(_right) {}
std::string firstconditionAddress = uniqueIdGen();
std::string secondConditionAddress = uniqueIdGen();
void codeGen(std::vector<std::string> regIn) {
	if(isFloat == 1) {
		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		condition->codeGen(regIn);
		std::cout<<"\tmtc1 $0," << r2 << std::endl;
		std::cout<<"\tc.eq.s $fcc0," << r1 << "," << r2 << std::endl;
	 	std::cout<< "\tbc1t $fcc0,"<< firstconditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		left->codeGen(regIn);
		std::cout << "\tb " << secondConditionAddress << std::endl;
		std::cout << "\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout << firstconditionAddress << ":" << std::endl;
		right->codeGen(regIn);
		std::cout << secondConditionAddress << ":" << std::endl;
	} else if (isDouble == 1){

		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		condition->codeGen(regIn);
		std::cout<<"\tmtc1 $0," << r2 << std::endl;
		std::cout<<"\tc.eq.d $fcc0," << r1 << "," << r2 << std::endl;
		std::cout<< "\tbc1t $fcc0,"<< firstconditionAddress << std::endl;
		std::cout<<"\tnop\n" << std::endl;
		left->codeGen(regIn);
		std::cout << "\tb " << secondConditionAddress << std::endl;
		std::cout << "\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout << firstconditionAddress << ":" << std::endl;
		right->codeGen(regIn);
		std::cout << secondConditionAddress << ":" << std::endl;

} else {
	std::string r1 = head(regIn);
	condition->codeGen(regIn);
	std::cout<<"\tbeq " << r1 << ",$0," << firstconditionAddress << std::endl;
	std::cout<<"\tnop\n" << std::endl;
	left->codeGen(regIn);
	std::cout << "\tb " << secondConditionAddress << std::endl;
	std::cout << "\tnop\n" << std::endl;
	std::cout<<"\t.option pic2" << std::endl;
	std::cout << firstconditionAddress << ":" << std::endl;
	right->codeGen(regIn);
	std::cout << secondConditionAddress << ":" << std::endl;
}
}
private:
	ASTExpression* condition;
	ASTExpression* left;
	ASTExpression* right;
};

struct ASTPrefixIncrement: public ASTExpression {
public:
	~ASTPrefixIncrement() {}
	ASTPrefixIncrement(ASTExpression* _exp, int _flag): variable(_exp), flag(_flag) {}
	void codeGen(std::vector<std::string> regIn) override {
		std::string r1 = head(regIn);
		variable->codeGen(regIn);
		if (isFloat == 0 && isDouble == 0) {
		if (flag == 0) {
		std::cout<<"\taddiu " << r1 << "," << r1 << ", 1" << std::endl;
	} else {
		std::cout<<"\tsubiu " << r1 << "," << r1 << ", 1" << std::endl;
	}
		if (variable->returnIndex() == -1) {
			std::cout<<"\tlw $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tsw $t0,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
			NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
			if (NumberofVaraibles != 0) {
			Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
		} else {
			//used to be 8 now i changed it to fit the new registers
			Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
		}
				std::cout<< "\tsw " << r1 << "," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tsw " << r1 << "," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	} else if(isDouble == 1) {
		std::string tempFloat = uniqueIdGenFloat();
		floatValues.push_back(1);
		std::string r2 = head(tail(regIn));
		if (flag == 0) {
			std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
			std::cout<<"\tldc1 "<< r2 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
		std::cout<<"\tadd.s " << r1 << "," << r1 << "," << r2  << std::endl;
	} else {
		std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
		std::cout<<"\tldc1 "<< r2 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
		std::cout<<"\tsub.s " << r1 << "," << r1 << "," << r2  << std::endl;
		}
		if (variable->returnIndex() == -1) {
			std::cout<<"\tldc1 $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tsdc1 $t0,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
				NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
				NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
				if (NumberofVaraibles != 0) {
				Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
			} else {
				//used to be 8 now i changed it to fit the new registers
				Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
			}
				std::cout<< "\tsdc1 " << r1 << "," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tsdc1 " << r1 << "," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	} else {
		std::string tempFloat = uniqueIdGenFloat();
		floatValues.push_back(1);
		std::string r2 = head(tail(regIn));
		if (flag == 0) {
			std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
			std::cout<<"\tlwc1 "<< r2 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
		std::cout<<"\tadd.s " << r1 << "," << r1 << "," << r2  << std::endl;
	} else {
		std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
		std::cout<<"\tlwc1 "<< r2 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
		std::cout<<"\tsub.s " << r1 << "," << r1 << "," << r2  << std::endl;
		}
		if (variable->returnIndex() == -1) {
			std::cout<<"\tlwc1 $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tswc1 $t0,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
				NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
				NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
				if (NumberofVaraibles != 0) {
				Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
			} else {
				//used to be 8 now i changed it to fit the new registers
				Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
			}
				std::cout<< "\tswc1 " << r1 << "," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tswc1 " << r1 << "," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	}
	}
	void codeGen() override {
		this->codeGen(regList);
	}
private:
	ASTExpression* variable;
	int flag;
};

struct ASTPostfixIncrement: public ASTExpression {
public:
	~ASTPostfixIncrement() {}
	ASTPostfixIncrement(ASTExpression* _exp, int _flag): variable(_exp), flag(_flag) {}
	void codeGen(std::vector<std::string> regIn) override {
		std::string r1 = head(regIn);
		std::string r2 = head(tail(regIn));
		std::string r3 = head(tail(tail(regIn)));
		variable->codeGen(regIn);
		if (isFloat == 1){
			std::string tempFloat = uniqueIdGenFloat();
			floatValues.push_back(1);
		if (flag == 0) {
			std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
			std::cout<<"\tlwc1 "<< r3 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
			std::cout<<"\tadd.s " << r2 << "," << r1 << "," << r3 << std::endl;
	} else {
		std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
		std::cout<<"\tlwc1 "<< r3 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
		std::cout<<"\tsub.s " << r2 << "," << r1 << "," << r3 << std::endl;	}
		if (variable->returnIndex() == -1) {
			std::cout<<"\tlwc1 $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tswc1 $t0,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
			NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
			if (NumberofVaraibles != 0) {
			Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
		} else {
			//used to be 8 now i changed it to fit the new registers
			Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
		}
				std::cout<< "\tswc1 " << r2 << "," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tswc1 " << r2 << "," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	} else if (isDouble == 1) {
		std::string tempFloat = uniqueIdGenFloat();
		floatValues.push_back(1);
	if (flag == 0) {
		std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
		std::cout<<"\tldc1 "<< r3 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
		std::cout<<"\tadd.d " << r2 << "," << r1 << "," << r3 << std::endl;
} else {
	std::cout<<"\tlui $2,%hi(" << tempFloat << ")" << std::endl;
	std::cout<<"\tldc1 "<< r3 << ",%lo("<<tempFloat<<")($2)"<<std::endl;
	std::cout<<"\tsub.d " << r2 << "," << r1 << "," << r3 << std::endl;	}
	if (variable->returnIndex() == -1) {
		std::cout<<"\tldc1 $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
		std::cout<<"\tsdc1 $t0,0($t1)" << std::endl;
	} else if (variable->returnIndex() > NumberofVaraibles * 4) {
		int Framesize;
		NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
		if (NumberofVaraibles != 0) {
		Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
	} else {
		//used to be 8 now i changed it to fit the new registers
		Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
	}
			std::cout<< "\tsdc1 " << r2 << "," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
	} else {
		std::cout<<"\tsdc1 " << r2 << "," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
	}
	} else {
		if (flag == 0) {
		std::cout<<"\taddiu " << r2 << "," << r1 << ", 1" << std::endl;
	} else {
		std::cout<<"\tsubiu " << r2 << "," << r1 << ", 1" << std::endl;
	}
		if (variable->returnIndex() == -1) {
			std::cout<<"\tlw $t1,%got("<<variable->nameretrieval()<<")($28)" << std::endl;
			std::cout<<"\tsw $t0,0($t1)" << std::endl;
		} else if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
			NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
			if (NumberofVaraibles != 0) {
			Framesize = ((NumberofVaraibles + 20 + 12 + maxArgs ) * 4) + ((NumberofVaraibles + maxArgs + 20  + 12) * 4) % 8;
		} else {
			//used to be 8 now i changed it to fit the new registers
			Framesize = ((20 + maxArgs  + 12) * 4) + (((maxArgs + 20  + 12) * 4)%8);
		}
				std::cout<< "\tsw " << r2 << "," << variable->returnIndex() + Framesize - 4 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tsw " << r2 << "," << variable->returnIndex() + (maxArgs * 4)  << "($fp)" << std::endl;
		}
	}
	}
	void codeGen() override {
		this->codeGen(regList);
	}
private:
	ASTExpression* variable;
	int flag;
};

struct ASTSizeOfExpression: public ASTExpression {
public:
	~ASTSizeOfExpression() {}
	ASTSizeOfExpression(int size) {
		sizeNumber = new ASTNumConst(size);
	}
	void codeGen(std::vector<std::string> regIn) override {
		sizeNumber->codeGen(regIn);
	}

	void codeGen() override {
		sizeNumber->codeGen(regList);
	}
private:
	ASTNumConst* sizeNumber;
};
