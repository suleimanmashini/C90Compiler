#include "AST.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

std::vector <std::string> globalVariables;
int globalFlag = 0;

int tabspace = 0;

int main(int argc, char* argv[]) {

	std::string inputFile = argv[2];
	std::string outputFile = argv[4];
	if ((std::string) argv[1] == "--translate") {

				freopen(argv[2], "r", stdin);
				const ASTNode *ASTRoot = parseAST();

		    std::ofstream out(outputFile);
		    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

				ASTRoot->print();
				std::cout<< std::endl;
				std::cout<<"# Boilerplat"<<std::endl;
				std::cout<<"if __name__ == \"__main__\":"<<std::endl;
				std::cout<<"    import sys"<<std::endl;
				std::cout<<"    ret=main()"<<std::endl;
				std::cout<<"    sys.exit(ret)"<<std::endl;


}

	return 0;
}
