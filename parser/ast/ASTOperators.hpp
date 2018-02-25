struct ASTPlus : public ASTExpression {
public:
  ~ASTPlus(){}
  void print() const override {
    std::cout<< "PLUS" << std::endl;
    left->print();
    right->print();
  }
  ASTPlus(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};

struct ASTMinus : public ASTExpression {
public:
  ~ASTMinus(){}
  void print() const override {
    std::cout<< "PLUS" << std::endl;
    left->print();
    right->print();
  }
  ASTMinus(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};

struct ASTDivide : public ASTExpression {
public:
  ~ASTDivide(){}
  void print() const override {
    std::cout<< "PLUS" << std::endl;
    left->print();
    right->print();
  }
  ASTDivide(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};

struct ASTMultiply : public ASTExpression {
public:
  ~ASTMultiply(){}
  void print() const override {
    std::cout<< "PLUS" << std::endl;
    left->print();
    right->print();
  }
  ASTMultiply(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};
