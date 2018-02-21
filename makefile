CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

all : bin/parser_test

parser/CParser.tab.cpp parser/CParser.tab.hpp : parser/CParser.y
	bison -v -d parser/CParser.y -o parser/CParser.tab.cpp

Lexer/lex.yy.cpp : Lexer/c90.flex parser/CParser.tab.hpp
	flex -o Lexer/lex.yy.cpp  Lexer/c90.flex

bin/parser_test : parser/PrintStructure.o parser/CParser.tab.o Lexer/lex.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/print_canonical $^

clean :
	rm parser/*.o
	rm Lexer/*.o
	rm parser/*.tab.cpp
	rm parser/*.tab.hpp
	rm Lexer/*.yy.cpp
