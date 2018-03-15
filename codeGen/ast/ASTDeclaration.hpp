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
		int initialVSize = allVariables.size();
		Declarator->codeGen();
		Block->pushVariables();
    int Framesize = ((initialVSize - allVariables.size()) * 8);
    //TODO: FIX THIS NOW!!!
    //im going to assume that all functions are leaves.
    //FOR NOW!
    //this is where i push the stack and print shit but for now itll be simple
    std::cout<<"generating block" << std::endl;
    Block->codeGen();

    std::cout << "nop\n";
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
	void updateVariables() {
		variable TEMP(typeNumber, Variable->getName());
		allVariables.push_back(TEMP);
	}
private:
	 int typeNumber;
	 ASTDirectDeclarator* Variable;
};
