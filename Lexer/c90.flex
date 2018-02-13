%option noyywrap

%{
  #include "CLexer.hpp"
  #include <stdio.h>

extern "C" int fileno(File *stream)
#define SAVE_TOKEN yylval.string = new
%}

DIGIT [0-9]
ID   [a-zA-Z_]

%%
[ \n\t]                  {;}

"auto"					 {;}
"break"					 {;}
"case"					 {;}
"char"					 {;}
"const"					 {;}
"continue"				 {;}
"default"				 {;}
"do"					 {;}
"double"				 {;}
"else"					 {;}
"enum"					 {;}
"extern"				 {;}
"float"					 {;}
"for"					 {;}
"goto"					 {;}
"if"					 {;}
"int"					 {;}
"long"					 {;}
"register"				 {;}
"return"				 {;}
"short"					 {;}
"signed"				 {;}
"sizeof"				 {;}
"static"				 {;}
"struct"				 {;}
"switch"				 {;}
"typedef"				 {;}
"union"					 {;}
"unsigned"				 {;}
"void"					 {;}
"volatile"				 {;}
"while"					 {;}




%%

 void yyerror (char const *s) {
   fprintf (stderr, "Parse Error: %s\n", s);
 }
