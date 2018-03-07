#pragma once

struct ASTExpression: public ASTNode {
public:
	virtual ~ASTExpression() {};
	virtual void codeGen() const = 0;
};
