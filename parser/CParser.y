%code requires{
  #include <string>
  #include <stdio.h>
  #include <iostream>
  #include <cassert>

  #include "AST.hpp"

  extern const ASTNode *ASTRoot;

  int yylex(void);
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
%token  T_dot TO_divide TO_memberAccess TO_not TO_bitwiseNot TO_ampersand TO_logicAnd
%token  TO_asterix TO_mod TO_bitwiseLeft TO_bitwiseRight
%token  TO_lessThan TO_moreThan TO_lessThanOrEqual TO_moreThanOrEqual TO_equalTo TO_notEqualTo
%token	TO_plus TO_minus TO_increment TO_decrement TO_equal
%token	TP_comma TP_colon TP_semiColon
%token	TC_integer TC_unsigned TC_long TC_longLong TC_float TC_longDouble
%token  TC_true TC_false TC_NULL TC_nullptr
%type <word> T_StringLiteral T_IDENTIFIER TK_int
%type <Integer> TC_integer
%type <Expression> EXPRESSION TERM FACTOR CONDITION
%type <Node> PROGRAM
%type <Function> FUNCTION
%type <Statement> STATEMENT BLOCK VARIABLE_DECLARATION  STRUCTURE
%type <Primitive> TK_void
%type <Keyword> RETURNTYPE
%type <Argument> VARIABLES
%type <Variable> ARGUMENT_DECLARATION
%type <Selection> SELECTION_STATEMENT


%union {
const ASTNode* Node;
const ASTMultiply* Mult;
const ASTDivide* Divide;
const ASTVariable* Variable;
const ASTKeyword* Keyword;
const ASTSelectionStatement* Selection;
const ASTPlus* Plus;
const ASTMinus* Minus;
const ASTArgumentStatement* Argument;
const ASTExpression* Expression;
const ASTReturnStatement* RStatement;
const ASTStatement* Statement;
const ASTPrimitive* Primitive;
const ASTFunction* Function;
std::string* word;
int Integer;
float FNumber;
}

%start PROGRAM

%%

PROGRAM: STRUCTURE		{ASTRoot = $1;}


STRUCTURE: FUNCTION STRUCTURE {$$ = new ASTStatement($1, $2);}
	    	 | FUNCTION {$$ = new ASTStatement($1);}

FUNCTION: RETURNTYPE T_IDENTIFIER T_LBRACKET T_RBRACKET T_LBRACE BLOCK T_RBRACE { $$ = new ASTFunction($1, *$2, NULL, $6);}
        | RETURNTYPE T_IDENTIFIER T_LBRACKET VARIABLES T_RBRACKET T_LBRACE BLOCK T_RBRACE {$$ = new ASTFunction($1, *$2, $4, $7);}

BLOCK: STATEMENT {$$ = new ASTStatement($1);}
     | BLOCK STATEMENT {$$ = new ASTStatement($1, $2);}

STATEMENT: TK_return EXPRESSION TP_semiColon {$$ = new ASTReturnStatement($2);}
         | EXPRESSION TP_semiColon {$$ = new ASTStatement($1);}
         | VARIABLE_DECLARATION TP_semiColon {$$ = $1;}
         | SELECTION_STATEMENT {$$ = $1;}

SELECTION_STATEMENT: TK_if T_LBRACKET CONDITION T_RBRACKET STATEMENT {$$ = new ASTSelectionStatement($3, $5);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE {$$ = new ASTSelectionStatement($3, $6);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET STATEMENT TK_else STATEMENT {$$ = new ASTSelectionStatement($3, $5, $7);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE TK_else STATEMENT {$$ = new ASTSelectionStatement($3, $6, $9);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET STATEMENT TK_else T_LBRACE BLOCK T_RBRACE {$$ = new ASTSelectionStatement($3, $5, $8);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE TK_else T_LBRACE BLOCK T_RBRACE {$$ = new ASTSelectionStatement($3, $6, $10);}

CONDITION: EXPRESSION TO_equalTo EXPRESSION {$$ = new ASTEquality($1, $3); }

VARIABLES: ARGUMENT_DECLARATION {$$ = new ASTArgumentStatement($1);}
         | ARGUMENT_DECLARATION TP_comma VARIABLES {$$ = new ASTArgumentStatement($1, $3);}

ARGUMENT_DECLARATION: RETURNTYPE T_IDENTIFIER  {$$ = new ASTVariable($1, *$2);}
| RETURNTYPE TO_ampersand T_IDENTIFIER {$$ = new ASTVariable($1, *$3);}

VARIABLE_DECLARATION: RETURNTYPE T_IDENTIFIER  {$$ = new ASTDeclarationStatement($1, *$2);}
                    | RETURNTYPE T_IDENTIFIER TO_equal EXPRESSION {$$ = new ASTDeclarationStatement($4, $1, *$2);}

RETURNTYPE: TK_void {$$ = new ASTKeyword("void");}
          | TK_int {$$ = new ASTKeyword("int");}

EXPRESSION: TERM {$$ = $1;}
          | EXPRESSION TO_plus EXPRESSION {$$ = new ASTPlus($1, $3);}
          | EXPRESSION TO_minus EXPRESSION {$$ = new ASTMinus($1, $3);}

TERM: FACTOR {$$ = $1;}
    | TERM TO_asterix FACTOR {$$ = new ASTMultiply($1, $3);}
    | TERM TO_divide FACTOR {$$ = new ASTDivide($1, $3);}

FACTOR: TC_integer {$$ = new ASTInteger($1); }


%%

const ASTNode *ASTRoot; // Definition of variable (to match declaration earlier)

const ASTNode *parseAST()
{
  ASTRoot=0;
  yyparse();
  return ASTRoot;
}
