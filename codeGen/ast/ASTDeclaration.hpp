#pragma once

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
	void codeGen() override {}
	void pushVariables() override {
		variable TEMP = variable(typeNumber, Variable->getName(), currentScope);
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
    //reset maxArgs to find the needed stack space for all the arguments.
    maxArgs = 0;

    //push arguments if there are any, should not be counted in the size;
    currentScope++;
    Declarator->pushArguments();
    currentScope--;
		initialVSize = allVariables.size();
    if (initialVSize == -1) initialVSize = 0;
    std::cout<<"\t.text\n";
    std::cout<<"\t.align  2\n";
    std::cout<<"\t.global "; Declarator->codeGen(); std::cout<<std::endl;
    std::cout<<"\t.ent  "; Declarator->codeGen(); std::cout << std::endl;
    std::cout<<"\t.type  "; Declarator->codeGen(); std::cout <<", @function" <<  std::endl;
		Block->pushVariables();
    Declarator->codeGen();
    std::cout<< ":\n";;
    int newsize = allVariables.size();
    NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
    int Framesize;
    if (NumberofVaraibles != 0) {
    Framesize = ((NumberofVaraibles + 9 + maxArgs) * 4) + ((NumberofVaraibles + maxArgs + 9) * 4) % 8;
  } else {
    //used to be 8 now i changed it to fit the new registers
    Framesize = ((9 + maxArgs) * 4) + (((maxArgs + 9) * 4)%8);
  }
    std::cout << "\n\t.frame $fp," << Framesize <<",$31"<<std::endl;
    std::cout << "\t.mask 0x40000000,-4" << std::endl;
    std::cout << "\t.fmask	0x00000000,0" << std::endl;
    std::cout << "\t.set	noreorder" << std::endl;
    std::cout << "\t.set nomacro" << std::endl;
    std::cout <<"\taddiu $sp, $sp, -" << Framesize << std::endl;
    //THIS PART IS NEW IM GONNA START SAVING REGISTERS
    std::cout <<"\tsw $31," << Framesize - 4 << "($sp)" << std::endl;
    std::cout <<"\tsw $23," << Framesize - 8 << "($sp)" << std::endl;
    std::cout <<"\tsw $22," << Framesize - 12 << "($sp)" << std::endl;
    std::cout <<"\tsw $21," << Framesize - 16 << "($sp)" << std::endl;
    std::cout <<"\tsw $20," << Framesize - 20 << "($sp)" << std::endl;
    std::cout <<"\tsw $19," << Framesize - 24 << "($sp)" << std::endl;
    std::cout <<"\tsw $18," << Framesize - 28 << "($sp)" << std::endl;
    std::cout <<"\tsw $17," << Framesize - 32 << "($sp)" << std::endl;
    std::cout <<"\tsw $16," << Framesize - 36 << "($sp)" << std::endl;
    std::cout << "\tsw $fp," << Framesize - 40 << "($sp)" << std::endl;
    std::cout << "\tmove $fp, $sp" << std::endl;
    // push the arguments in the registers to the stack
    if (Declarator->countArgs() != 0){
      switch (Declarator->countArgs()){
          case 1:
          std::cout <<"\tlw $a0," << Framesize << "($sp)" << std::endl;
          break;
          case 2:
          std::cout <<"\tlw $a0," << Framesize << "($sp)" << std::endl;
          std::cout <<"\tlw $a1," << Framesize + 4 << "($sp)" << std::endl;
          break;
          case 3:
          std::cout <<"\tlw $a0," << Framesize << "($sp)" << std::endl;
          std::cout <<"\tlw $a1," << Framesize + 4 << "($sp)" << std::endl;
          std::cout <<"\tlw $a2," << Framesize + 8 << "($sp)" << std::endl;
          break;
          default:
          std::cout <<"\tlw $a0," << Framesize << "($sp)" << std::endl;
          std::cout <<"\tlw $a1," << Framesize + 4 << "($sp)" << std::endl;
          std::cout <<"\tlw $a2," << Framesize + 8 << "($sp)" << std::endl;
          std::cout <<"\tlw $a3," << Framesize + 12 << "($sp)" << std::endl;
      }
    }
    Block->codeGen();
    std::cout << "\tnop\n";
    std::cout << "\tmove $sp, $fp" << std::endl;
    std::cout <<"\tlw $31," << Framesize - 4 << "($sp)" << std::endl;
    std::cout <<"\tlw $23," << Framesize - 8 << "($sp)" << std::endl;
    std::cout <<"\tlw $22," << Framesize - 12 << "($sp)" << std::endl;
    std::cout <<"\tlw $21," << Framesize - 16 << "($sp)" << std::endl;
    std::cout <<"\tlw $20," << Framesize - 20 << "($sp)" << std::endl;
    std::cout <<"\tlw $19," << Framesize - 24 << "($sp)" << std::endl;
    std::cout <<"\tlw $18," << Framesize - 28 << "($sp)" << std::endl;
    std::cout <<"\tlw $17," << Framesize - 32 << "($sp)" << std::endl;
    std::cout <<"\tlw $16," << Framesize - 36 << "($sp)" << std::endl;
    std::cout << "\tlw $fp," << Framesize - 40 << "($sp)" << std::endl;
    std::cout << "\taddiu $sp, $sp," << Framesize << std::endl;
    std::cout << "\tjr $31" << std::endl;
    std::cout << "\tnop\n";
    std::cout << "\t.set  macro" << std::endl;
    std::cout << "\t.set  reorder";
    std::cout << "\n\t.end  ";
    Declarator->codeGen();
    std::cout<<"\n\t.size "; Declarator->codeGen();
    std::cout<<", .-"; Declarator->codeGen(); std::cout<<std::endl;
    std::cout << std::endl;
    //END OF A FUNCTION TIME TO POP THE VARIABLES
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
