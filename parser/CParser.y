%code requires{
  #include <string>
  #include <stdio.h>
  #include <iostream>

  #include "AST.hpp"

  extern ASTNode *ASTRoot;

  int yylex(void)
  void yyerror(const char*);

}

%token  T_IDENTIFIER 
%token  T_LBRACE T_RBRACE T_LBRACKET T_RBRACKET T_LINDEX T_RINDEX
%token	TK_auto TK_break TK_case TK_char TK_const TK_continue TK_default
%token	TK_do TK_double TK_else TK_enum TK_extern TK_float TK_for TK_goto 
%token  TK_if TK_int TK_long TK_register TK_return TK_short TK_signed
%token	TK_sizeof TK_static TK_struct TK_switch TK_typedef TK_union TK_unsigned
%token  TK_void TK_volatile TK_while
%token	T_StringLiteral T_LStringLiteral
%token  T_dot TO_memberAccess TO_not TO_bitwiseNot TO_ampersand TO_logicAnd
%token  TO_asterix TO_mod TO_bitwiseLeft TO_bitwiseRight
%token  TO_lessThan TO_moreThan TO_lessThanOrEqual TO_moreThanOrEqual TO_equalTo TO_notEqualTo
%token	TO_plus TO_minus TO_increment TO_decrement 
%token	TP_comma TP_colon TP_semiColon
%token	TC_integer TC_unsigned TC_long TC_longLong TC_float TC_longDouble
%token  TC_true TC_false TC_NULL TC_nullptr
%type <string> 
%type <integer> 
%type <function> 

%start PROGRAM

%%

%union {
ASTNode* Node;
int Integer;
string Identifier;
}

PROGRAM: STRUCTURE			{$$ = $1;}
	   ;

STRUCTURE: STRUCTURE FUNCTION {$$ = new ASTNode($2);}
		 | STRUCTURE STATEMENT_DECLARATION {;}
		 | FUNCTION {$$ = $1;}
		 | STATEMENT_DECLARATIONS
		 ;

FUNCTION: RETURNTYPE IDENTIFIER T_LBRACKET T_RBRACKET T_LBRACE SCOPE T_RBRACE {$$ = new ASTFunction($1, $2, nullptr, $7);}
		;

SCOPE: BLOCK	{$$ = $1;}
	 | SCOPE BLOCK {;}
	 ;

BLOCK: DECLARATION_LIST	{;}
	 | STATEMENT_LIST {$$ = $1;}
	 | EXPRESSION_LIST {;}
	 ;

STATEMENT_LIST: TK_return EXPRESSION {$$ = new ASTReturnStatement($2);}
		      ;

EXPRESSION_LIST: EXPRESSION {$$ = $1;}
			   ;

EXPRESSION: TC_INTEGER {$$ = new ASTInteger($1); }
		  ;


%%

const ASTNode *ASTRoot; // Definition of variable (to match declaration earlier)

const ASTNode *parseAST()
{
  ASTRoot=0;
  yyparse();
  return ASTRoot;
}

