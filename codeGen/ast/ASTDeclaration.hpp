#pragma once



struct ASTDirectDeclarator: public ASTDeclaration {
public:
  ASTDirectDeclarator( std::string _Identifier): Identifier(_Identifier) {}
  void codeGen()  override{
    std::cout <<  this->getName();
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
    std::cout<<"\t.text\n";
    std::cout<<"\t.align  2\n";
    std::cout<<"\t.global "; Declarator->codeGen(); std::cout<<std::endl;
    std::cout<<"\t.ent  "; Declarator->codeGen(); std::cout << std::endl;
    std::cout<<"\t.type  "; Declarator->codeGen(); std::cout <<", @function" <<  std::endl;

    Declarator->codeGen();
		Block->pushVariables();
    std::cout<< ":\n";;
    int newsize = allVariables.size();
    NumberofVaraibles = (((allVariables.size() + 1) ? allVariables.size() : 0) - initialVSize);
    int Framesize;
    if (NumberofVaraibles != 0) {
    Framesize = (((NumberofVaraibles + 9) * 4) + (((NumberofVaraibles + 9) * 4)%8));
  } else {
    //used to be 8 now i changed it to fit the new registers
    Framesize = (9 * 4) + 4;
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
		variable TEMP = variable(typeNumber, Variable->getName(), currentScope);
		allVariables.push_back(TEMP);
	}
private:
	 int typeNumber;
	 ASTDirectDeclarator* Variable;
};
