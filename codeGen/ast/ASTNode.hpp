#pragma once

#include <iostream>
#include <string>
#include <vector>


struct ASTNode {
public:
	virtual ~ASTNode() {};
	virtual void codeGen() const = 0;
};

struct variable {
public:
variable(int _type, std::string _name): type(_type), name(_name) {}
private:
int type;
std::string name;
};

std::vector<variable> allVariables;

enum {
	RZERO, //HARD WIRES TO 0
	AT,  //RESERVED FOR PSEUDO-INSTRUCTIONS
	V0, V1, //RETURN VALUES FROM FUNCTIONS
	A0, A1, A2, A3, //ARGUMENTS TO FUNCTIONS (NOT PRESERVED BY SUBPROGRAMS)
	T0, T1, T2, T3, T4, T5, T6, T7, //TEMPORARY DATA (NOT PRESERVED BY SUBPROGRAMS)
	S0, S1, S2, S3, S4, S5, S6, S7, //SAVED REGISTERS (PRESERVED BY SUPROGRAMS)
	T8, T9, //MORE TEMP REGISTERS (NOT PRESEVED BY SUBPROGRAMS)
	K0, K1, //KERNEL
	GP, //GLOBAL AREA POINTED
	SP, //STACK POINTER
	FP, //FRAME POINTER
	RA, //RETURN ADDRESS
	HI, LO,
	PC
};
