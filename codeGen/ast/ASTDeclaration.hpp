#pragma once
extern int isGlobal;
extern int globalsFound;
extern int isAccessible;

struct ASTParameterList;
struct ASTDirectDeclarator: public ASTDeclaration {
public:
  ASTDirectDeclarator( std::string _Identifier, ASTDeclaration* _Parameters): Identifier(_Identifier), Parameters(_Parameters) {}
  void codeGen()  override{
    std::cout <<  this->getName();
  }
  std::string getName()  {return Identifier;}
  void pushVariables()  override {}
  void pushArguments() override {
    if(Parameters != NULL ){
      Parameters->pushArguments();
    }
  }
  void popVariables() override {
    if (Parameters != NULL ){
        Parameters->popVariables();
  }
}
  int countArgs() override{
    if (Parameters != NULL) {
      return Parameters->countArgs();
    } else {
      return 0;
    }
  }
private:
  //TODO: add a parameters parameter list
  ASTDeclaration* Parameters;
  std::string Identifier;
};

struct ASTVariableDeclaration: public ASTDeclaration {
public:
	~ASTVariableDeclaration() {}
	ASTVariableDeclaration( int _typeNumber,  ASTDeclaration* _Variable): typeNumber(_typeNumber), Variable(_Variable) {}
	void codeGen() override {
    if (isGlobal) {
      globalsFound = 1;
      std::cout << "\t.comm " << Variable->getName() << ",4,4\n"<<std::endl;
    }
  }
	void pushVariables() override {
		variable TEMP = variable(typeNumber, Variable->getName(), currentScope, isAccessible);
		allVariables.push_back(TEMP);
	}
  void popVariables() override{
    allVariables.pop_back();
  }
  int countArgs() override {
    return 1;
  }
private:
	 int typeNumber;
	 ASTDeclaration* Variable;
};

struct ASTParameterList: public ASTDeclaration {
public:
  ASTParameterList(ASTDeclaration* _Parameter, ASTDeclaration* _NextParameter): Parameter(_Parameter), NextParameter(_NextParameter) {}
  void pushArguments() override {
    Parameter->pushVariables();
    if (NextParameter != NULL) {
      NextParameter->pushArguments();
    }
  }
  virtual void codeGen()  {}
  virtual void pushVariables() {}
  void popVariables() override {
    Parameter->popVariables();
    if (NextParameter != NULL) {
      NextParameter->popVariables();
    }
  }
  int countArgs() override {
    if (NextParameter != NULL) {
      return NextParameter->countArgs() + Parameter->countArgs();
    } else {
      return Parameter->countArgs();
    }
  }
private:
  ASTDeclaration* Parameter;
  ASTDeclaration* NextParameter;
};

struct ASTFunctionDefinition: public ASTNode {
public:
	ASTFunctionDefinition( int _functionType,  ASTDirectDeclarator* _Declarator,  ASTCompoundStatement* _Block): functionType(_functionType), Declarator(_Declarator), Block(_Block) {
    listOfFunctions.push_back(Declarator->getName());
  }
  int getReturnType()  {return functionType;}
  std::string getFunctionName()  {return Declarator->getName();}
  void codeGen()  override {
    if(functionType == 5) isTheFunctionFloat = 1;
    if(functionType == 6) isTheFunctionDouble = 1;
    isGlobal = 0;
    //reset maxArgs to find the needed stack space for all the arguments.
    maxArgs = 0;
    currentFunction = this->getFunctionName();
    //push arguments if there are any, should not be counted in the size;
    currentScope++;
    Declarator->pushArguments();
    currentScope--;
    isAccessible = 0;
		initialVSize = allVariables.size();
    if (initialVSize == -1) initialVSize = 0;
    std::cout<<"\t.text\n";
    std::cout<<"\t.align  2\n";
    std::cout<<"\t.global "; Declarator->codeGen(); std::cout<<std::endl;
    std::cout<<"\t.ent  "; Declarator->codeGen(); std::cout << std::endl;
    std::cout<<"\t.type  "; Declarator->codeGen(); std::cout <<", @function" <<  std::endl;
		Block->pushVariables();
    Declarator->codeGen();
    std::cout<< ":\n";
    if (maxArgs <= 4) maxArgs = 4;
    int newsize = allVariables.size();
    NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
    int Framesize;
    if (NumberofVaraibles != 0) {
    Framesize = ((NumberofVaraibles + 20 + 12+ maxArgs) * 4 + 4) + ((NumberofVaraibles + maxArgs + 20+ 12) * 4 + 4) % 8;
  } else {
    //used to be 8 now i changed it to fit the new registers
    Framesize = ((20 + 12 + maxArgs) * 4 + 4 ) + (((maxArgs + 20) * 4 + 4)%8);
  }
    std::cout << "\n\t.frame $fp," << Framesize <<",$31"<<std::endl;
    if (isTheFunctionFloat == 0 ){
    std::cout << "\t.mask 0x40000000,-4" << std::endl;
  } else {
    std::cout << "\t.mask 0xc0000000,-4" << std::endl;

  }
    std::cout << "\t.fmask	0x00000000,0" << std::endl;
    std::cout << "\t.set	noreorder" << std::endl;
    std::cout << "\t.set nomacro" << std::endl;
    std::cout <<"\taddiu $sp, $sp, -" << Framesize << std::endl;
    //THIS PART IS NEW IM GONNA START SAVING REGISTERS
    std::cout <<"\tsw $31," << Framesize - 4 << "($sp)" << std::endl;
    std::cout <<"\tsw $25," << Framesize - 8 << "($sp)" << std::endl;
    std::cout <<"\tsw $24," << Framesize - 12 << "($sp)" << std::endl;
    std::cout <<"\tsw $23," << Framesize - 16 << "($sp)" << std::endl;
    std::cout <<"\tsw $22," << Framesize - 20 << "($sp)" << std::endl;
    std::cout <<"\tsw $21," << Framesize - 24 << "($sp)" << std::endl;
    std::cout <<"\tsw $20," << Framesize - 28 << "($sp)" << std::endl;
    std::cout <<"\tsw $19," << Framesize - 32 << "($sp)" << std::endl;
    std::cout <<"\tsw $18," << Framesize - 36 << "($sp)" << std::endl;
    std::cout <<"\tsw $17," << Framesize - 40 << "($sp)" << std::endl;
    std::cout <<"\tsw $16," << Framesize - 44 << "($sp)" << std::endl;
    std::cout <<"\tsw $15," << Framesize - 48 << "($sp)" << std::endl;
    std::cout <<"\tsw $14," << Framesize - 52 << "($sp)" << std::endl;
    std::cout <<"\tsw $13," << Framesize - 56 << "($sp)" << std::endl;
    std::cout <<"\tsw $12," << Framesize - 60 << "($sp)" << std::endl;
    std::cout <<"\tsw $11," << Framesize - 64 << "($sp)" << std::endl;
    std::cout <<"\tsw $10," << Framesize - 66 << "($sp)" << std::endl;
    std::cout <<"\tsw $9," << Framesize - 72 << "($sp)" << std::endl;
    std::cout <<"\tsw $8," << Framesize - 76 << "($sp)" << std::endl;
    std::cout << "\tsw $fp," << Framesize - 80 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f30," << Framesize - 84 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f28," << Framesize - 88 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f26," << Framesize - 92 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f24," << Framesize - 96 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f22," << Framesize - 100 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f20," << Framesize - 104 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f18," << Framesize - 108 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f16," << Framesize - 112 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f10," << Framesize - 116 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f8," << Framesize - 120 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f6," << Framesize - 124 << "($sp)" << std::endl;
    std::cout<<"\tswc1 $f4," << Framesize - 128 << "($sp)" << std::endl;

    std::cout << "\tmove $fp, $sp" << std::endl;
    if (globalsFound) {
      std::cout<<"\tlui	$28,%hi(__gnu_local_gp)" << std::endl;
      std::cout<<"\taddiu	$28,$28,%lo(__gnu_local_gp)" << std::endl;
    }

    // push the arguments in the registers to the stack
    if (Declarator->countArgs() != 0){
      switch (Declarator->countArgs()){
          case 1:
          std::cout <<"\tsw $a0," << Framesize << "($sp)" << std::endl;
          break;
          case 2:
          std::cout <<"\tsw $a0," << Framesize << "($sp)" << std::endl;
          std::cout <<"\tsw $a1," << Framesize + 4 << "($sp)" << std::endl;
          break;
          case 3:
          std::cout <<"\tsw $a0," << Framesize << "($sp)" << std::endl;
          std::cout <<"\tsw $a1," << Framesize + 4 << "($sp)" << std::endl;
          std::cout <<"\tsw $a2," << Framesize + 8 << "($sp)" << std::endl;
          break;
          default:
          std::cout <<"\tsw $a0," << Framesize << "($sp)" << std::endl;
          std::cout <<"\tsw $a1," << Framesize + 4 << "($sp)" << std::endl;
          std::cout <<"\tsw $a2," << Framesize + 8 << "($sp)" << std::endl;
          std::cout <<"\tsw $a3," << Framesize + 12 << "($sp)" << std::endl;
      }
    }
    Block->codeGen();
    std::cout << "\tnop\n";
    std::cout << "$" << this->getFunctionName() << ":" << std::endl;
    std::cout << "\tmove $sp, $fp" << std::endl;
    std::cout <<"\tlw $31," << Framesize - 4 << "($sp)" << std::endl;
    std::cout <<"\tlw $25," << Framesize - 8 << "($sp)" << std::endl;
    std::cout <<"\tlw $24," << Framesize - 12 << "($sp)" << std::endl;
    std::cout <<"\tlw $23," << Framesize - 16 << "($sp)" << std::endl;
    std::cout <<"\tlw $22," << Framesize - 20 << "($sp)" << std::endl;
    std::cout <<"\tlw $21," << Framesize - 24 << "($sp)" << std::endl;
    std::cout <<"\tlw $20," << Framesize - 28 << "($sp)" << std::endl;
    std::cout <<"\tlw $19," << Framesize - 32 << "($sp)" << std::endl;
    std::cout <<"\tlw $18," << Framesize - 36 << "($sp)" << std::endl;
    std::cout <<"\tlw $17," << Framesize - 40 << "($sp)" << std::endl;
    std::cout <<"\tlw $16," << Framesize - 44 << "($sp)" << std::endl;
    std::cout <<"\tlw $15," << Framesize - 48 << "($sp)" << std::endl;
    std::cout <<"\tlw $14," << Framesize - 52 << "($sp)" << std::endl;
    std::cout <<"\tlw $13," << Framesize - 56 << "($sp)" << std::endl;
    std::cout <<"\tlw $12," << Framesize - 60 << "($sp)" << std::endl;
    std::cout <<"\tlw $11," << Framesize - 64 << "($sp)" << std::endl;
    std::cout <<"\tlw $10," << Framesize - 66 << "($sp)" << std::endl;
    std::cout <<"\tlw $9," << Framesize - 72 << "($sp)" << std::endl;
    std::cout <<"\tlw $8," << Framesize - 76 << "($sp)" << std::endl;
    std::cout << "\tlw $fp," << Framesize - 80 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f30," << Framesize - 84 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f28," << Framesize - 88 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f26," << Framesize - 92 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f24," << Framesize - 96 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f22," << Framesize - 100 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f20," << Framesize - 104 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f18," << Framesize - 108 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f16," << Framesize - 112 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f10," << Framesize - 116 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f8," << Framesize - 120 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f6," << Framesize - 124 << "($sp)" << std::endl;
    std::cout<<"\tlwc1 $f4," << Framesize - 128 << "($sp)" << std::endl;
    std::cout << "\taddiu $sp, $sp," << Framesize << std::endl;
    std::cout << "\tjr $31" << std::endl;
    std::cout << "\tnop\n";
    std::cout << "\t.set  macro" << std::endl;
    std::cout << "\t.set  reorder";
    std::cout << "\n\t.end  ";
    Declarator->codeGen();
    std::cout<<"\n\t.size "; Declarator->codeGen();
    std::cout<<", .-"; Declarator->codeGen(); std::cout<<std::endl;
    if (isTheFunctionFloat == 1){
      std::cout<<"\t.rdata"<< std::endl;
    }
    std::cout << std::endl;
    //END OF A FUNCTION TIME TO POP THE VARIABLES
    isGlobal = 1;
    isTheFunctionDouble = 0;
    isTheFunctionFloat = 0;
  }
private:
	 int functionType;
   ASTDirectDeclarator* Declarator;
   ASTCompoundStatement* Block;
};



struct ASTTypeSpecifier: public ASTDeclaration {
public:
  ASTTypeSpecifier( int _typeNumber): typeNumber(_typeNumber) {}
	void codeGen()  override  {}
  int getType() { return typeNumber;}
private:
   int typeNumber;
  //lets assume for now 0 is void and 1 is int
};
