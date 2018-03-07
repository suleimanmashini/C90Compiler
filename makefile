CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

translator : bin/c_translator

parser/CParser.tab.cpp parser/CParser.tab.hpp : parser/CParser.y
	bison -v -d parser/CParser.y -o parser/CParser.tab.cpp

parser/Lexer/lex.yy.cpp : parser/Lexer/c90.flex parser/CParser.tab.hpp
	flex -o parser/Lexer/lex.yy.cpp  parser/Lexer/c90.flex

bin/c_translator : parser/PrintStructure.o parser/CParser.tab.o parser/Lexer/lex.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_translator $^

compiler: bin/c_compiler

codeGen/CParser.tab.cpp codeGen/CParser.tab.hpp: codeGen/CParser.y
	bison -v -d codeGen/CParser.y -o codeGen/CParser.tab.cpp

codeGen/Lexer/lex.yy.cpp : codeGen/Lexer/c90.flex codeGen/CParser.tab.hpp
	flex -o codeGen/Lexer/lex.yy.cpp  codeGen/Lexer/c90.flex

bin/c_compiler : codeGen/generateCode.o codeGen/CParser.tab.o codeGen/Lexer/lex.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_compiler $^

clean :
	-rm parser/*.o
	-rm bin/*
	-rm parser/Lexer/*.o
	-rm parser/*.tab.cpp
	-rm parser/*.tab.hpp
	-rm parser/Lexer/*.yy.cpp
	-rm tmp//formative/*
	-rm codeGen/Lexer/*.o
	-rm codeGen/*.tab.cpp
	-rm codeGen/*.tab.hpp
	-rm codeGen/Lexer/*.yy.cpp

run:
	./bin/c_compiler

translate:
	./bin/c_translator
