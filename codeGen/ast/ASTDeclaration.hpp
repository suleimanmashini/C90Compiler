#pragma once



struct ASTDirectDeclarator: public ASTDeclaration {
public:
  ASTDirectDeclarator( std::string _Identifier): Identifier(_Identifier) {}
  void codeGen()  override{
    std::cout <<  this->getName() << ":\t";
  }
  std::string getName()  {return Identifier;}
private:
  //TODO: add a parameters PARAMETEr list
   std::string Identifier;
};

struct ASTFunctionDefinition: public ASTNode {
public:
	ASTFunctionDefinition( int _functionType,  ASTDirectDeclarator* _Declarator,  ASTCompoundStatement* _Block): functionType(_functionType), Declarator(_Declarator), Block(_Block) {}
  int getReturnType()  {return functionType;}
  std::string getFunctionName()  {return Declarator->getName();}
  void codeGen()  override {
		initialVSize = allVariables.size();
    if (initialVSize == -1) initialVSize = 0;
		Declarator->codeGen();
		Block->pushVariables();
    int newsize = allVariables.size();
    NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
    int Framesize;
    if (NumberofVaraibles != 0) {
    Framesize = ((NumberofVaraibles * 4) + ((NumberofVaraibles * 4)%8));
  } else {
    Framesize = 8;
  }
    std::cout <<"\taddiu $sp, $sp, -" << Framesize << std::endl;
    std::cout << "\tsw $fp," << Framesize - 4 << "($sp)" << std::endl;
    std::cout << "\tmove $fp, $sp" << std::endl;
    Block->codeGen();
    std::cout << "\tnop\n";
    std::cout << "\tmove $sp, $fp" << std::endl;
    std::cout << "\tlw $fp," << Framesize - 4 << "($sp)" << std::endl;
    std::cout << "\taddiu $sp, $sp," << Framesize << std::endl;
    std::cout << "\tjr $31" << std::endl;
    std::cout << "\tnop\n";
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




struct ASTVariableDeclaration: public ASTDeclaration {
public:
	~ASTVariableDeclaration() {}
	ASTVariableDeclaration( int _typeNumber,  ASTDirectDeclarator* _Variable): typeNumber(_typeNumber), Variable(_Variable) {}
	void codeGen()  {}
	void pushVariables() override {
		variable TEMP = variable(typeNumber, Variable->getName());
		allVariables.push_back(TEMP);
	}
private:
	 int typeNumber;
	 ASTDirectDeclarator* Variable;
};
