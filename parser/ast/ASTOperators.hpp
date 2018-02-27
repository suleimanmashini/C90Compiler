struct ASTPlus : public ASTExpression {
public:
  ~ASTPlus(){}
  void print() const override {
    left->print();
    std::cout<< "+";
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
    left->print();
    std::cout<< "-";
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
    left->print();
    std::cout<< "/";
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
    left->print();
    std::cout<< "*";
    right->print();
  }
  ASTMultiply(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};

struct ASTEquality : public ASTExpression {
public:
  ~ASTEquality(){}
  void print() const override {
    left->print();
    std::cout<< " == ";
    right->print();
  }
  ASTEquality(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};

struct ASTLessThan : public ASTExpression {
public:
  ~ASTLessThan(){}
  void print() const override {
    left->print();
    std::cout<< " < ";
    right->print();
  }
  ASTLessThan(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};

struct ASTMoreThan : public ASTExpression {
public:
  ~ASTMoreThan(){}
  void print() const override {
    left->print();
    std::cout<< " > ";
    right->print();
  }
  ASTMoreThan(const ASTExpression* leftIn, const ASTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const ASTExpression* left;
  const ASTExpression* right;
};
