%code requires{
  #include <string>
  #include <stdio.h>
  #include <iostream>

  #include "AST.hpp"

  extern Program *g_root;

  int yylex(void)
  void yyerror(const char*);

}

%union{
  Function *func
  Statement *statement
  Expression *expression
}

%token 
