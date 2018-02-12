%code requires{
  #include <string>
  #include <stdio.h>
  #include <iostream>

  #include "AST.hpp"

  extern Program *ASTRoot;

  int yylex(void)
  void yyerror(const char*);

}

%union{
  Function *func
  Statement *statement
  Expression *expression
  int integer
}

%token  T_IDENTIFIER T_INTEGER T_LBRACE T_RBRACE T_LBRACKET T_RBRACKET
%token  T_int T_return
%type <string> T_IDENTIFIER
%type <integer> T_INTEGER
%type <function> FUNCTION

%start PROGRAM

%%

PROGRAM: STRUCTURE

STRUCTURE: FUNCTION			{ ASTRoot->add( $1 ); }
		   STATEMENT		{ASTRoot->add( $1 ); }
		 ;

FUNCTION: DATATYPE T_IDENTIFIER PARAMETERS COMPOUND_STATEMENT		{$$ = new Function(*$2, $3, $4); }
		;

COMPOUND_STATEMENT: T_LBRACE T_RBRACE								{$$ = new Block();}
				  |	T_LBRACE BLOCK T_RBRACE							{$$ = $2;}
				  ;

BLOCK: STATEMENT													{$$ = ;}
	 ;

STATEMENT: RETURN_STATEMENT											{$$ = $1;}

RETURN_STATEMENT: T_return BLOCK								{$$ = NULL; /*BLOCK IS WRONG BUT I DONT HAVE EXPRESSIONS YET */}

PARAMETERS: T_LBRACKET T_RBRACKET									{$$ = new ParameterList();}
		  ;

DATATYPE: T_int 
		;



