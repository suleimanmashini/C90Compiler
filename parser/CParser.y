%code requires{
  #include <string>
  #include <stdio.h>
  #include <iostream>
  #include <cassert>

  #include "TST.hpp"

  extern const TSTNode *TSTRootTrans;

  int zzlex(void);
  void zzerror(const char*);

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
%token  TO_TSTerix TO_mod TO_bitwiseLeft TO_bitwiseRight
%token  TO_lessThan TO_moreThan TO_lessThanOrEqual TO_moreThanOrEqual TO_equalTo TO_notEqualTo
%token	TO_plus TO_minus TO_increment TO_decrement TO_equal
%token	TP_comma TP_colon TP_semiColon
%token	TC_integer TC_unsigned TC_long TC_longLong TC_float TC_longDouble
%token  TC_true TC_false TC_NULL TC_nullptr
%token TO_OrEqual TO_exOrEqual TO_andEqual TO_rightEqual TO_modEqual TO_plusEqual TO_minusEqual
%token TO_leftEqual TO_divEqual TO_questionMark TO_exclussiveOr TO_logicOr TO_OR TO_multEqual
%type <word> T_StringLiteral T_IDENTIFIER TK_int
%type <Integer> TC_integer
%type <Expression> EXPRESSION TERM FACTOR CONDITION
%type <Node> PROGRAM
%type <Function> FUNCTION
%type <Statement> STATEMENT BLOCK VARIABLE_DECLARATION  STRUCTURE ITERATION_STATEMENT VARIABLE_DECLARATION_GLOBAL
%type <Primitive> TK_void
%type <Keyword> RETURNTYPE
%type <Argument> VARIABLES
%type <VName> VARIABLE_DECLARATION_FUNCTION
%type <Selection> SELECTION_STATEMENT
%type <Call> FUNCTION_CALL
%type <EStatement> EXPRESSION_STATEMENTS


%union {
const TSTNode* Node;
const TSTMultiply* Mult;
const TSTDivide* Divide;
const TSTVariable* Variable;
const TSTVariableName* VName;
const TSTKeyword* Keyword;
const TSTSelectionStatement* Selection;
const TSTExpressionStatement* EStatement;
const TSTPlus* Plus;
const TSTMinus* Minus;
const TSTFunctionCall* Call;
const TSTArgumentStatement* Argument;
const TSTExpression* Expression;
const TSTReturnStatement* RStatement;
const TSTStatement* Statement;
const TSTPrimitive* Primitive;
const TSTFunction* Function;
std::string* word;
int Integer;
float FNumber;
}

%start PROGRAM

%%

PROGRAM: STRUCTURE		{TSTRootTrans = $1;}


STRUCTURE: FUNCTION STRUCTURE {$$ = new TSTStatement($1, $2);}
	    	 | FUNCTION {$$ = new TSTStatement($1);}
         | VARIABLE_DECLARATION_GLOBAL {$$ = $1;}
         | VARIABLE_DECLARATION_GLOBAL STRUCTURE {$$ = new TSTStatement($1, $2); }

FUNCTION: RETURNTYPE T_IDENTIFIER T_LBRACKET T_RBRACKET T_LBRACE BLOCK T_RBRACE { $$ = new TSTFunction($1, *$2, NULL, $6);}
        | RETURNTYPE T_IDENTIFIER T_LBRACKET VARIABLES T_RBRACKET T_LBRACE BLOCK T_RBRACE {$$ = new TSTFunction($1, *$2, $4, $7);}

BLOCK: STATEMENT {$$ = new TSTStatement($1);}
     | BLOCK STATEMENT {$$ = new TSTStatement($1, $2);}

STATEMENT: TK_return EXPRESSION TP_semiColon {$$ = new TSTReturnStatement($2);}
         | EXPRESSION TP_semiColon {$$ = new TSTStatement($1);}
         | VARIABLE_DECLARATION TP_semiColon {$$ = $1;}
         | SELECTION_STATEMENT {$$ = $1;}
         | EXPRESSION TO_equal EXPRESSION TP_semiColon {$$ = new TSTAssignmentStatement($1, $3);}
         | ITERATION_STATEMENT {$$ = $1;}

 ITERATION_STATEMENT: TK_while T_LBRACKET CONDITION T_RBRACKET STATEMENT {$$ = new TSTIteratorStatementWhile($3, $5);}
                    | TK_while T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE {$$ = new TSTIteratorStatementWhile($3, $6);}

SELECTION_STATEMENT: TK_if T_LBRACKET CONDITION T_RBRACKET STATEMENT {$$ = new TSTSelectionStatement($3, $5);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE {$$ = new TSTSelectionStatement($3, $6);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET STATEMENT TK_else STATEMENT {$$ = new TSTSelectionStatement($3, $5, $7);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE TK_else STATEMENT {$$ = new TSTSelectionStatement($3, $6, $9);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET STATEMENT TK_else T_LBRACE BLOCK T_RBRACE {$$ = new TSTSelectionStatement($3, $5, $8);}
                   | TK_if T_LBRACKET CONDITION T_RBRACKET T_LBRACE BLOCK T_RBRACE TK_else T_LBRACE BLOCK T_RBRACE {$$ = new TSTSelectionStatement($3, $6, $10);}

CONDITION: EXPRESSION TO_equalTo EXPRESSION {$$ = new TSTEquality($1, $3); }
         | EXPRESSION TO_lessThan EXPRESSION {$$ = new TSTLessThan($1, $3); }
         | EXPRESSION TO_notEqualTo EXPRESSION {$$ = new TSTNotEqualTo($1, $3);}
         | EXPRESSION TO_moreThan EXPRESSION {$$ = new TSTMoreThan($1, $3); }
         | EXPRESSION TO_moreThanOrEqual EXPRESSION {$$ = new TSTMoreThanOrEqual($1, $3);}
         | EXPRESSION TO_lessThanOrEqual EXPRESSION {$$ = new TSTLessThanOrEqual($1, $3);}
         | EXPRESSION TO_logicAnd EXPRESSION {$$ = new TSTLogicAnd($1, $3);}
         | EXPRESSION TO_logicOr EXPRESSION {$$ = new TSTLogicOr($1, $3);}
         | EXPRESSION {$$ = $1;}


VARIABLES: VARIABLE_DECLARATION_FUNCTION {$$ = new TSTArgumentStatement($1);}
         | VARIABLE_DECLARATION_FUNCTION TP_comma VARIABLES {$$ = new TSTArgumentStatement($1, $3);}

VARIABLE_DECLARATION_FUNCTION: RETURNTYPE T_IDENTIFIER  {$$ = new TSTVariableName(*$2);}
| RETURNTYPE TO_ampersand T_IDENTIFIER {$$ = new TSTVariableName(*$3);}

VARIABLE_DECLARATION: RETURNTYPE T_IDENTIFIER  {$$ = new TSTDeclarationStatement($1, *$2, 0);}
                    | RETURNTYPE T_IDENTIFIER TO_equal EXPRESSION {$$ = new TSTDeclarationStatement($4, $1, *$2, 0); }

VARIABLE_DECLARATION_GLOBAL: RETURNTYPE T_IDENTIFIER TP_semiColon  {$$ = new TSTDeclarationStatement($1, *$2, 1);}
                            | RETURNTYPE T_IDENTIFIER TO_equal EXPRESSION TP_semiColon {$$ = new TSTDeclarationStatement($4, $1, *$2, 1); }

RETURNTYPE: TK_void {$$ = new TSTKeyword("void");}
          | TK_int {$$ = new TSTKeyword("int");}

EXPRESSION: TERM {$$ = $1;}
          | EXPRESSION TO_plus EXPRESSION {$$ = new TSTPlus($1, $3);}
          | EXPRESSION TO_minus EXPRESSION {$$ = new TSTMinus($1, $3);}
          | T_LBRACKET EXPRESSION T_RBRACKET {$$ = $2;}
          | FUNCTION_CALL {$$ = $1;}


TERM: FACTOR {$$ = $1;}
    | TERM TO_TSTerix FACTOR {$$ = new TSTMultiply($1, $3);}
    | TERM TO_divide FACTOR {$$ = new TSTDivide($1, $3);}

FACTOR: TC_integer {$$ = new TSTInteger($1); }
      | T_IDENTIFIER {$$ = new TSTVariableName(*$1);}
      | FUNCTION_CALL {$$ = $1;}

FUNCTION_CALL: T_IDENTIFIER T_LBRACKET T_RBRACKET {$$ = new TSTFunctionCall(*$1);}
             | T_IDENTIFIER T_LBRACKET EXPRESSION_STATEMENTS T_RBRACKET {$$ = new TSTFunctionCall(*$1, $3);}

EXPRESSION_STATEMENTS: EXPRESSION {$$ = new TSTExpressionStatement($1);}
                     | EXPRESSION TP_comma EXPRESSION_STATEMENTS {$$ = new TSTExpressionStatement($1, $3);}


%%

const TSTNode *TSTRootTrans; // Definition of variable (to match declaration earlier)

const TSTNode *parseTSTTrans()
{
  TSTRootTrans=0;
  zzparse();
  return TSTRootTrans;
}
