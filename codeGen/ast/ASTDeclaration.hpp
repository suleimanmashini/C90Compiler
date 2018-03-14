#pragma once



struct ASTDirectDeclarator: public ASTDeclaration {
public:
  ASTDirectDeclarator(const std::string _Identifier): Identifier(_Identifier) {}
  void codeGen() const override{
    std::cout <<  this->getName() << ":\t";
  }
  std::string getName() const {return Identifier;}
private:
  //TODO: add a parameters PARAMETEr list
  const std::string Identifier;
};

struct ASTFunctionDefinition: public ASTNode {
public:
	ASTFunctionDefinition(const int _functionType, const ASTDirectDeclarator* _Declarator, const ASTCompoundStatement* _Block): functionType(_functionType), Declarator(_Declarator), Block(_Block) {}
  int getReturnType() const {return functionType;}
  std::string getFunctionName() const {return Declarator->getName();}
  void codeGen() const override {
		int initialVSize = allVariables.size();
		Declarator->codeGen();
		Block->pushVariables();
    int Framesize = ((initialVSize - allVariables.size()) * 8)
    //TODO: FIX THIS NOW!!!
    //im going to assume that all functions are leaves.
    //FOR NOW!
    //this is where i push the stack and print shit but for now itll be simple
    std::cout << "  addiu $sp, $sp, -" << Framesize << std::endl;
    std::cout << "  sw    $fp, " << (Framesize-4) << "($sp)\n";
    std::cout << "  move  $fp, $sp\n";
    Block->codeGen();
    std::cout << "  move  $sp, $fp\n";
    std::cout << "  lw    $fp, 4($sp)\n";
    std::cout << "  addiu $sp, $sp, 8\n";
    std::cout << "  jr    $31\n";
    std::cout << "nop\n";
  }
private:
	const int functionType;
  const ASTDirectDeclarator* Declarator;
  const ASTCompoundStatement* Block;
};



struct ASTTypeSpecifier: public ASTDeclaration {
public:
  ASTTypeSpecifier(const int _typeNumber): typeNumber(_typeNumber) {}
	void codeGen() const override  {}
  int getType() { return typeNumber;}
private:
  const int typeNumber;
  //lets assume for now 0 is void and 1 is int
};




struct ASTVariableDeclaration: public ASTDeclaration {
public:
	~ASTVariableDeclaration() {}
	ASTVariableDeclaration(const int _typeNumber, const ASTDirectDeclarator* _Variable): typeNumber(_typeNumber), Variable(_Variable) {}
	void codeGen() const {}
	void updateVariables() {
		variable TEMP(typeNumber, Variable->getName());
		allVariables.push_back(TEMP);
	}
private:
	const int typeNumber;
	const ASTDirectDeclarator* Variable;
};
