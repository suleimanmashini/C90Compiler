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
		this->codeGen(regList);
	}
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

struct ASTIntegerConst: public ASTExpression {
public:
	~ASTIntegerConst() {}
	ASTIntegerConst( int _numValue): numValue(_numValue) {}
	void updateRegisterNeeds() override {
	}
	void codeGen (std::vector<std::string> regIn)   override{
		std::string r1 = head(regIn);
		std::cout<< "\tli " << r1 << ", " << numValue << std::endl;
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
		//ok this part is wrong cause i then need to check for all scope;
		std::string r1 = head(regIn);
		if (findVariableIndex(allVariables, variableName) == -1) {
			//save $2 in the stack below
			//this is me winging it it probably doesnt work but why not;
			std::cout<<"\tsw $2,-4(fp)" << std::endl;
			std::cout<<"\tlw $2,%got("<<variableName<<")($28)" << std::endl;
			if(r1 == "$v0") {
			std::cout<<"\tlw " << r1 << ",0($2)" << std::endl;
		} else {
			std::cout<<"\tlw " << r1 << ",0($2)" << std::endl;
			std::cout<<"\tlw $2,-4(fp)" << std::endl;
		}

		} else {
			int index = ((NumberofVaraibles)-(findVariableIndex(allVariables, variableName)-initialVSize)) * 4;
			//TODO:MAKENSURE THAT THIS EQUATION WORKS PROPERLY
			if (index > NumberofVaraibles * 4) {
				int Framesize;
					NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
					if (NumberofVaraibles != 0) {
			    Framesize = ((NumberofVaraibles + 9 + maxArgs) * 4) + ((NumberofVaraibles + maxArgs + 9) * 4) % 8;
			  } else {
			    //used to be 8 now i changed it to fit the new registers
			    Framesize = ((9 + maxArgs) * 4) + (((maxArgs + 9) * 4)%8);
			  }
					std::cout<< "\tlw " << r1 << ", " << index + Framesize - 8 <<"($fp)" << std::endl;
			} else {
			std::cout<< "\tlw " << r1 << ", " << index <<"($fp)" << std::endl;
		}
		}
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
		switch(assignmentOp){
			case 0:
				break;
			case 1:
				//*=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tmul $t0,$v0"<<std::endl;
				std::cout<<"\tmov $v0,$LO"<<std::endl;
				break;
			case 2:
				///=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tdiv $t0,$v0"<<std::endl;
				std::cout<<"\tmov $v0,$LO"<<std::endl;
				break;
			case 3:
				//%=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tdiv $t0,$v0"<<std::endl;
				std::cout<<"\tmov $v0,$HI"<<std::endl;
				break;
			case 4:
				//+=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tadd $v0,$t0,$v0"<<std::endl;
				break;
			case 5:
				//-=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tsub $v0,$t0,$v0"<<std::endl;
				break;
			case 6:
				//>>=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tsrlv $v0,$t0,$v0"<<std::endl;
				break;
			case 7:
				//<<=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tsllv $v0,$t0,$v0"<<std::endl;
				break;
			case 8:
				//&=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tand $v0,$t0,$v0"<<std::endl;
				break;
			case 9:
				//^=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\txor $v0,$t0,$v0"<<std::endl;
				break;
			case 10:
				//|=
				std::cout<<"\tlw $t0," << variable->returnIndex() << "($fp)" << std::endl;
				std::cout<<"\tor $v0,$t0,$v0"<<std::endl;
				break;
		}
		if (variable->returnIndex() > NumberofVaraibles * 4) {
			int Framesize;
				NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
				if (NumberofVaraibles != 0) {
				Framesize = ((NumberofVaraibles + 9 + maxArgs) * 4) + ((NumberofVaraibles + maxArgs + 9) * 4) % 8;
			} else {
				//used to be 8 now i changed it to fit the new registers
				Framesize = ((9 + maxArgs) * 4) + (((maxArgs + 9) * 4)%8);
			}
				std::cout<< "\tsw $v0," << variable->returnIndex() + Framesize - 8 <<"($fp)" << std::endl;
		} else {
			std::cout<<"\tsw $v0," << variable->returnIndex() << "($fp)" << std::endl;
	}
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
			std::cout <<"\taddiu $sp, $sp, -" << 84 << std::endl;
			std::cout <<"\tsw $30," << 78 << "($sp)" << std::endl;
			std::cout <<"\tsw $23," << 74 << "($sp)" << std::endl;
			std::cout <<"\tsw $22," << 72 << "($sp)" << std::endl;
			std::cout <<"\tsw $21," << 68 << "($sp)" << std::endl;
			std::cout <<"\tsw $20," << 64 << "($sp)" << std::endl;
			std::cout <<"\tsw $19," << 60 << "($sp)" << std::endl;
			std::cout <<"\tsw $18," << 56 << "($sp)" << std::endl;
			std::cout <<"\tsw $17," << 52 << "($sp)" << std::endl;
			std::cout <<"\tsw $16," << 48 << "($sp)" << std::endl;
			std::cout <<"\tsw $25," << 44 << "($sp)" << std::endl;
			std::cout <<"\tsw $24," << 40 << "($sp)" << std::endl;
			std::cout <<"\tsw $15," << 36 << "($sp)" << std::endl;
			std::cout <<"\tsw $14," << 32 << "($sp)" << std::endl;
			std::cout <<"\tsw $13," << 28 << "($sp)" << std::endl;
			std::cout <<"\tsw $12," << 24 << "($sp)" << std::endl;
			std::cout <<"\tsw $11," << 20 << "($sp)" << std::endl;
			std::cout <<"\tsw $10," << 16 << "($sp)" << std::endl;
			std::cout <<"\tsw $9," << 12 << "($sp)" << std::endl;
			std::cout <<"\tsw $8," << 8 << "($sp)" << std::endl;
			std::cout <<"\tsw $v0," << 4 << "($sp)" << std::endl;
			std::cout <<"\tsw $v1," << 0 << "($sp)" << std::endl;
			Argument->codeGen();
			//now the result will be in v0 so we need to move it to its appropriate registers
			switch(argNumber){
				case 0:
				std::cout << "\tmove $a0, $v0" << std::endl;
				break;
				case 1:
				std::cout << "\tmove $a1, $v0" << std::endl;
				break;
				case 2:
				std::cout << "\tmove $a2, $v0" << std::endl;
				break;
				case 3:
				std::cout << "\tmove $a3, $v0" << std::endl;
				break;
				default:
				std::cout << "\tsw $v0,"<< argNumber * 4 <<"($fp)" << std::endl;
			}
			//OR we can pop it to the frame
			//POPBACK THE REGISTERS AFTER EVALUATION
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
			std::cout<<"\tlui	$28,%hi(__gnu_local_gp)"<<std::endl;
			std::cout<<"\taddiu	$28,$28,%lo(__gnu_local_gp)"<<std::endl;
			std::cout<<"\t.cprestore	16"<<std::endl;
			//TODO:load the arguments now
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
			std::cout <<"\tlw $30," << 78 << "($sp)" << std::endl;
			std::cout <<"\tlw $23," << 74 << "($sp)" << std::endl;
			std::cout <<"\tlw $22," << 72 << "($sp)" << std::endl;
			std::cout <<"\tlw $21," << 68 << "($sp)" << std::endl;
			std::cout <<"\tlw $20," << 64 << "($sp)" << std::endl;
			std::cout <<"\tlw $19," << 60 << "($sp)" << std::endl;
			std::cout <<"\tlw $18," << 56 << "($sp)" << std::endl;
			std::cout <<"\tlw $17," << 52 << "($sp)" << std::endl;
			std::cout <<"\tlw $16," << 48 << "($sp)" << std::endl;
			std::cout <<"\tlw $25," << 44 << "($sp)" << std::endl;
			std::cout <<"\tlw $24," << 40 << "($sp)" << std::endl;
			std::cout <<"\tlw $15," << 36 << "($sp)" << std::endl;
			std::cout <<"\tlw $14," << 32 << "($sp)" << std::endl;
			std::cout <<"\tlw $13," << 28 << "($sp)" << std::endl;
			std::cout <<"\tlw $12," << 24 << "($sp)" << std::endl;
			std::cout <<"\tlw $11," << 20 << "($sp)" << std::endl;
			std::cout <<"\tlw $10," << 16 << "($sp)" << std::endl;
			std::cout <<"\tlw $9," << 12 << "($sp)" << std::endl;
			std::cout <<"\tlw $8," << 8 << "($sp)" << std::endl;
			std::cout <<"\tlw $v0," << 4 << "($sp)" << std::endl;
			std::cout <<"\tlw $v1," << 0 << "($sp)" << std::endl;
			std::cout <<"\taddiu $sp, $sp, " << 84 << std::endl;
		}
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
