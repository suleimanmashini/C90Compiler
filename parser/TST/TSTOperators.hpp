struct TSTPlus : public TSTExpression {
public:
  ~TSTPlus(){}
  void print() const override {
    left->print();
    std::cout<< "+";
    right->print();
  }
  TSTPlus(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTMinus : public TSTExpression {
public:
  ~TSTMinus(){}
  void print() const override {
    left->print();
    std::cout<< "-";
    right->print();
  }
  TSTMinus(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTDivide : public TSTExpression {
public:
  ~TSTDivide(){}
  void print() const override {
    left->print();
    std::cout<< "/";
    right->print();
  }
  TSTDivide(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTMultiply : public TSTExpression {
public:
  ~TSTMultiply(){}
  void print() const override {
    std::cout << "( ";
    left->print();
    std::cout<< "*";
    right->print();
      std::cout << " )";
  }
  TSTMultiply(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTEquality : public TSTExpression {
public:
  ~TSTEquality(){}
  void print() const override {
    left->print();
    std::cout<< " == ";
    right->print();
  }
  TSTEquality(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTLessThan : public TSTExpression {
public:
  ~TSTLessThan(){}
  void print() const override {
    std::cout << "( ";
    left->print();
    std::cout<< " < ";
    right->print();
    std::cout << " )";
  }
  TSTLessThan(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTMoreThan : public TSTExpression {
public:
  ~TSTMoreThan(){}
  void print() const override {
    left->print();
    std::cout<< " > ";
    right->print();
  }
  TSTMoreThan(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};


struct TSTMoreThanOrEqual : public TSTExpression {
public:
  ~TSTMoreThanOrEqual(){}
  void print() const override {
      std::cout << "( ";
    left->print();
    std::cout<< " >= ";
    right->print();
      std::cout << " )";
  }
  TSTMoreThanOrEqual(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTLessThanOrEqual : public TSTExpression {
public:
  ~TSTLessThanOrEqual(){}
  void print() const override {
      std::cout << "( ";
    left->print();
    std::cout<< " <= ";
    right->print();
      std::cout << " )";
  }
  TSTLessThanOrEqual(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};

struct TSTNotEqualTo : public TSTExpression {
public:
  ~TSTNotEqualTo(){}
  void print() const override {
    std::cout << "( ";
    left->print();
    std::cout<< " != ";
    right->print();
    std::cout << " )";
  }
  TSTNotEqualTo(const TSTExpression* leftIn, const TSTExpression* rightIn): left(leftIn), right(rightIn){ }
private:
  const TSTExpression* left;
  const TSTExpression* right;
};
