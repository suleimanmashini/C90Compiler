#pragma once

struct ASTStatement: public ASTExpression {
public:
	virtual ~ASTStatement() {};
	virtual void pushVariables() {}
	virtual void codeGen()  = 0;
};


struct ASTReturnStatement: public ASTStatement {
ASTReturnStatement( ASTExpression* _returnExpression) : returnExpression(_returnExpression) {}
void codeGen()  override {
	if (isTheFunctionFloat == 0) {
	if(returnExpression == NULL) {
  std::cout<< "\tnop \n";
} else {
	returnExpression->codeGen(regList);
	std::cout<< "\tmove $v0, $t0"<< std::endl;
	std::cout<< "\tb $" << currentFunction << std::endl;
	std::cout<< "\tnop \n";
}
} else {
	if(returnExpression == NULL) {
  std::cout<< "\tnop \n";
} else {
	isFloat = 1;
	returnExpression->codeGen(regListFloat);
	std::cout<< "\tmov.s $f0, $f4"<< std::endl;
	std::cout<< "\tb $" << currentFunction << std::endl;
	std::cout<< "\tnop \n";
	isFloat = 0;
}
}
}
void pushVariables() {
	returnExpression->pushVariables();
}
private:
   ASTExpression* returnExpression;
};


struct ASTStatementList: public ASTStatement {

ASTStatementList( ASTStatementList* _Child,  ASTStatement* _Statement): Child(_Child), Statement(_Statement) {}
void codeGen()  override {
  if (Child == NULL && Statement == NULL) {
		std::cout <<"\tnop\n";
		return;
	}
	if (Statement != NULL) {
		Statement->codeGen();
	}
	if (Child != NULL) {
		Child->codeGen();
	}
}
void pushVariables() override{
	if(Statement!=NULL) {
		Statement->pushVariables();
	}
	if (Child != NULL) {
		Child->pushVariables();
	}
}
private:
   ASTStatementList* Child;
   ASTStatement* Statement;

};

struct ASTCompoundStatement: public ASTStatement {
public:
~ASTCompoundStatement() {}
ASTCompoundStatement( ASTStatementList* _StateList,  ASTDeclarationList* _DeclList): StatList(_StateList), DeclList(_DeclList) {}
void codeGen()  override {
	setAccessible(startIndex, numberofVinScope);
	currentScope++;
	if (StatList == NULL && DeclList == NULL) {
		std::cout <<"\tnop\n";
		return;
	}
	if(DeclList != NULL) {
		DeclList->codeGen();
	}
	if(StatList != NULL){
  StatList->codeGen();
}
	currentScope--;
	setAccessible(startIndex, numberofVinScope);
}
void pushVariables()  override {
	currentScope++;
	startIndex = allVariables.size();
	if (DeclList != NULL) {
		DeclList->pushVariables();
		//TODO: ADD SUPPORT TO OTHER STATEMENTS THAT HAVE BLOCKS
	}
	int temp = allVariables.size();
	numberofVinScope = temp - startIndex;
	if (StatList != NULL) {
		StatList->pushVariables();
	}
	currentScope--;
}
private:
   ASTStatementList* StatList;
   ASTDeclarationList* DeclList;
	 int numberofVinScope;
	 int startIndex;
};

struct ASTExpression;
struct ASTExpressionStatement: public ASTStatement {
	~ASTExpressionStatement() {}
	ASTExpressionStatement( ASTExpression* _Exp): Exp(_Exp) {}
	void codeGen()  {
		if (Exp != NULL) {
		Exp->codeGen(regList);
	} else {

		std::cout<<"\tnop"<< std::endl;
	}
	}
	void pushVariables() override {
		Exp->pushVariables();
	}
private:
 ASTExpression* Exp;
};
extern int uniqueID;
std::string uniqueIdGen();
struct ASTIfStatement: public ASTStatement {
public:
	~ASTIfStatement() {}
	ASTIfStatement(ASTExpression* _Condition, ASTStatement* _then, ASTStatement* _ifelse): Condition(_Condition), then(_then), ifelse(_ifelse) {}
	void pushVariables() override {
		then->pushVariables();
		if (ifelse != NULL) {
		ifelse->pushVariables();
	}
	}
	void codeGen() {
		Condition->codeGen();
		//this is where we do the jump for the condition
		//We need a jump generators
		std::string elseaddress = uniqueIdGen();
		std::string exitaddress = uniqueIdGen();

		std::cout<<"\tbeq $0, $t0," <<  elseaddress << std::endl;
		std::cout<<"\tnop" << std::endl;
		then->codeGen();
		std::cout<<"\tb " << exitaddress << std::endl;
		std::cout<<"\tnop" << std::endl;
		std::cout<< elseaddress << ":" << std::endl;
		if (ifelse != NULL) {
		ifelse->codeGen();
	}
		std::cout<< exitaddress << ":" << std::endl;
	}
private:
	ASTExpression* Condition;
	ASTStatement* then;
	ASTStatement* ifelse;
};

struct ASTWhileStatement: public ASTStatement {
public:
	~ASTWhileStatement() {}
	void pushVariables() override {
		block->pushVariables();
	}
	ASTWhileStatement(ASTExpression* _condition, ASTStatement* _block): condition(_condition), block(_block) {}
	void codeGen() {
		std::string address1 = uniqueIdGen();
		std::string address2 = uniqueIdGen();
		std::cout<<"\t.option pic0" << std::endl;
		std::cout << "\tb " << address1 << std::endl;
		std::cout << "\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<<address2<< ":" << std::endl;
		block->codeGen();
		std::cout<<address1<<":" << std::endl;
		condition->codeGen();
		std::cout<<"\tbne " << "$t0,$0," << address2 << std::endl;
		std::cout<<"\tnop\n" << std::endl;
	}
	void codeGen(ASTExpression* expression){
		std::string address1 = uniqueIdGen();
		std::string address2 = uniqueIdGen();
		std::cout<<"\t.option pic0" << std::endl;
		std::cout << "\tb " << address1 << std::endl;
		std::cout << "\tnop\n" << std::endl;
		std::cout<<"\t.option pic2" << std::endl;
		std::cout<<address2<< ":" << std::endl;
		block->codeGen();
		if(expression != NULL) {
		expression->codeGen();
	}
		std::cout<<address1<<":" << std::endl;
		condition->codeGen();
		std::cout<<"\tbne " << "$t0,$0," << address2 << std::endl;
		std::cout<<"\tnop\n" << std::endl;
	}
private:
	ASTExpression* condition;
	ASTStatement* block;
};


struct ASTDoWhileSatement: public ASTStatement {
public:
	~ASTDoWhileSatement() {}
	void pushVariables() override {
		block->pushVariables();
	}
	ASTDoWhileSatement(ASTExpression* _condition, ASTStatement* _block): condition(_condition), block(_block) {}
	void codeGen() {
		std::string address1 = uniqueIdGen();
		std::cout<<address1<<":" << std::endl;
		block->codeGen();
		condition->codeGen();
		std::cout<<"bne " << "$t0,$0," << address1 << std::endl;
		std::cout<<"\tnop\n" << std::endl;
	}
private:
	ASTExpression* condition;
	ASTStatement* block;
};

struct ASTForLoopStatement: public ASTStatement {
public:
	~ASTForLoopStatement() {}
	ASTForLoopStatement(ASTExpression* _exp1, ASTExpression* _exp2, ASTExpression* _exp3, ASTStatement* _block): exp1(_exp1), exp3(_exp3) {
		if (exp1 == NULL && exp3 == NULL) {
			ASTNumConst* temp = new ASTNumConst(1);
			blockWhile = new ASTWhileStatement(temp, _block);
		} else {
		blockWhile = new ASTWhileStatement(_exp2, _block);
	}
	}
	void codeGen() {
		if(exp1 != NULL) {
		exp1->codeGen();
	}
	if (blockWhile != NULL) {
		blockWhile->codeGen(exp3);
	}
	}
	void pushVariables() override {
		blockWhile->pushVariables();
	}
private:
ASTExpression* exp1;
ASTWhileStatement* blockWhile;
ASTExpression* exp3;

};
