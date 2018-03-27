%code requires{
  #include <string>
  #include <stdio.h>
  #include <iostream>
  #include <cassert>

  #include "AST.hpp"

  extern  ASTTranslationUnit *ASTRoot;

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
%token TO_OrEqual TO_exOrEqual TO_andEqual TO_rightEqual TO_leftEqual TO_minusEqual TO_plusEqual TO_divEqual TO_multEqual
%token TO_OR TO_logicOr TO_exclussiveOr TO_questionMark TO_modEqual
%type <word> T_StringLiteral T_IDENTIFIER TK_int
%type <Number> TYPE_NAME DECLARATION_SPECIFIERS TYPE_SPECIFIER ASSIGNMENT_OPERATOR
%type <Function> FUNCTION_DEFINITION
%type <DirectD> DECLARATOR DIRECT_DECLARATOR INIT_DECLARATION_LIST INIT_DECLARATOR
%type <StateList> STATEMENT_LIST
%type <Compound> COMPOUND_STATEMENT
%type <Return> JUMP_STATEMENT
%type <Expression> SHIFT_EXPRESSION EXPRESSION CONDITIONAL_EXPRESSION PRIMARY_EXPRESSION ASSIGNMENT_EXPRESSION POSTFIX_EXPRESSION AND_EXPRESSION
%type <Expression> RELATIONAL_EXPRESSION EQUALITY_EXPRESSION UNARY_EXPRESSION CAST_EXPRESSION MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION INCLUSIVE_OR_EXPRESSION EXCLUSIVE_OR_EXPRESSION
%type <Expression> CONSTANT_EXPRESSION ARGUMENT_EXPRESSION_LIST LOGICAL_OR_EXPRESSION LOGICAL_AND_EXPRESSION
%type <Integer> TC_integer
%type <Statement> STATEMENT EXPRESSION_STATEMENT SELECTION_STATEMENT ITERATION_STATEMENT
%type <DecList> DECLARATION_LIST
%type <ActualDeclaration> DECLARATION PARAMETER_DECLARATION PARAMETER_TYPE_LIST PARAMETER_LIST
%type <Translation> TRANSLATION_UNIT START
%type <Float> TC_float


%union {
 ASTVariableExp* Vexp;
 ASTNode* Node;
 ASTArgumentList* ArgList;
 ASTFunctionCall* FuncCall;
 ASTInclusiveOr* Incl;
 ASTParameterList* Plist;
 ASTDeclaration* ActualDeclaration;
 ASTExclusiveOr* ExclOr;
 ASTBitwiseAnd* bitAnd;
 ASTVariableDeclaration* Declaration;
 ASTMultiplicativeExpression* MultExp;
 ASTAdditiveExpression* AddExp;
 ASTDeclarationList* DecList;
 ASTDirectDeclarator* DirectD;
 ASTReturnStatement* Return;
 ASTStatementList* StateList;
 ASTStatement* Statement;
 ASTCompoundStatement* Compound;
 ASTDeclaration* Declarator;
 std::string* word;
 ASTFunctionDefinition* Function;
 ASTExpression* Expression;
 ASTAssignmentExpression* AssExpression;
 int* Number;
 ASTTranslationUnit* Translation;
 ASTNumConst* IntegerP;
int Integer;
float Float;
}

%start START

%%

START: TRANSLATION_UNIT {ASTRoot = $1;}

TRANSLATION_UNIT: FUNCTION_DEFINITION {$$ = new ASTTranslationUnit($1, NULL, NULL);}
                | DECLARATION {$$ = new ASTTranslationUnit(NULL, $1, NULL);}
                | FUNCTION_DEFINITION TRANSLATION_UNIT  {$$ = new ASTTranslationUnit($1, NULL, $2);}
                | DECLARATION TRANSLATION_UNIT  {$$ = new ASTTranslationUnit(NULL, $1, $2);}
/*
EXTERNAL_DECLARATION: FUNCTION_DEFINITION {$$ = $1;}
                    | DECLARATION {;}
*/

/*
FUNCTION_DEFINITION: DECLARATION_SPECIFIERS DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT {;}
                   | DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT {;}
                   | DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT {$$ = new ASTFunctionDefinition($1, $2, $3);}
                   | DECLARATOR COMPOUND_STATEMENT {;}
*/
FUNCTION_DEFINITION: DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT {$$ = new ASTFunctionDefinition(*$1, $2, $3);}

/*
STATEMENT: LABELED_STATEMENT {;}
         | COMPOUND_STATEMENT {;}
         | EXPRESSION_STATEMENT {;}
         | SELECTION_STATEMENT {;}
         | ITERATION_STATEMENT {;}
         | JUMP_STATEMENT {;}
*/
STATEMENT: COMPOUND_STATEMENT {$$ = $1;}
         | JUMP_STATEMENT {$$ = $1;}
         | SELECTION_STATEMENT {$$ = $1;}
         | EXPRESSION_STATEMENT {$$ = $1;}
         | ITERATION_STATEMENT {$$ = $1;}

/*
LABELLED_STATEMENT: T_IDENTIFIER TP_colon STATEMENT {;}
                  | TK_case CONSTANT_EXPRESSION TP_colon STATEMENT {;}
                  | TK_default TP_colon STATEMENT {;}
*/

COMPOUND_STATEMENT: T_LBRACE DECLARATION_LIST STATEMENT_LIST T_RBRACE {$$ = new ASTCompoundStatement($3, $2);}
                  | T_LBRACE STATEMENT_LIST T_RBRACE {$$ = new ASTCompoundStatement($2, nullptr);}
                  | T_LBRACE DECLARATION_LIST T_RBRACE {$$ = new ASTCompoundStatement(nullptr, $2);}
                  | T_LBRACE T_RBRACE {$$ = new ASTCompoundStatement(NULL, NULL);}

                  //THESE TWO MAY NEED TO BE SWITCHED AROUND!!!

DECLARATION_LIST: DECLARATION {$$ = new ASTDeclarationList(NULL, $1);}
                | DECLARATION DECLARATION_LIST {$$ = new ASTDeclarationList($2, $1);}

STATEMENT_LIST: STATEMENT {$$ = new ASTStatementList(NULL, $1);}
              | STATEMENT STATEMENT_LIST {$$ = new ASTStatementList($2, $1);}

EXPRESSION_STATEMENT: EXPRESSION TP_semiColon {$$ = new ASTExpressionStatement($1);}
                    | TP_semiColon {;}

SELECTION_STATEMENT: TK_if T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {$$ = new ASTIfStatement($3, $5, NULL);}
                   | TK_if T_LBRACKET EXPRESSION T_RBRACKET STATEMENT TK_else STATEMENT {$$ = new ASTIfStatement($3, $5, $7);}


ITERATION_STATEMENT: TK_while T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {$$ = new ASTWhileStatement($3, $5);}
                   | TK_do STATEMENT TK_while T_LBRACKET EXPRESSION T_RBRACKET TP_semiColon {$$ = new ASTDoWhileSatement($5, $2);}
                   | TK_for T_LBRACKET EXPRESSION TP_semiColon EXPRESSION TP_semiColon EXPRESSION T_RBRACKET STATEMENT {$$ = new ASTForLoopStatement($3, $5, $7, $9);}


/*
SELECTION_STATEMENT: TK_if T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {;}
                   | TK_if T_LBRACKET EXPRESSION T_RBRACKET STATEMENT TK_else STATEMENT {;}
                   | TK_switch T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {;}
*/

/*
JUMP_STATEMENT: TK_goto T_IDENTIFIER TP_semiColon {;}
              | TK_continue TP_semiColon {;}
              | TK_break TP_semiColon {;}
              | TK_return EXPRESSION TP_semiColon {;}
              | TK_return TP_semiColon {;}
*/

JUMP_STATEMENT: TK_return TP_semiColon {$$ = new ASTReturnStatement(NULL);}
              | TK_return EXPRESSION TP_semiColon {$$ = new ASTReturnStatement($2);}


/*
DECLARATION: DECLARATION_SPECIFIERS TP_semiColon{;}
             | DECLARATION_SPECIFIERS `INIT_DECLARATION_LIST` TP_semiColon {;}
*/

DECLARATION: DECLARATION_SPECIFIERS INIT_DECLARATION_LIST TP_semiColon {$$ = new ASTVariableDeclaration(*$1, $2);}

/*
DECLARATION_SPECIFIERS: STORAGE_CLASS_SPECIFIER DELCARATION DECLARATION_SPECIFIERS {;}
                       | TYPE_SPECIFIER DECLARATION_SPECIFIERS {;}
                       | TYPE_SPECIFIER {;}
                       | TYPE_QUALIFIER DECLARATION_SPECIFIERS {;}
                       | TYPE_QUALIFIER {;}
*/
/*
INIT_DECLARATION_LIST: INIT_DECLARATOR {;}
                     | INIT_DECLARATOR_LIST TP_comma INIT_DECLARATOR DECLARATION_SPECIFIERS {;}
*/
DECLARATION_SPECIFIERS: TYPE_SPECIFIER {$$ = $1;}
                      | TYPE_SPECIFIER DECLARATION_SPECIFIERS {$$ = $1;}
                      | TYPE_QUALIFIER {;}
                      | TYPE_QUALIFIER DECLARATION_SPECIFIERS {$$ = $2;}

INIT_DECLARATION_LIST: INIT_DECLARATOR {$$ = $1;}

INIT_DECLARATOR: DECLARATOR {$$ = $1;}

/*
INIT_DECLARATOR: DECLARATOR {;}
               | DECLARATOR TO_equal INITIALIZER {;}

INITIALIZER: ASSIGNMENT_EXPRESSION
           | T_LBRACE INITIALIZER_LIST T_RBRACE
           | T_LBRACE INITIALIZER_LIST TP_comma T_RBRACE

INITIALIZER_LIST: INITIALIZER
                | INITIALIZER_LIST TP_comma INITIALIZER
*/
/*
DECLARATOR: POINTER DIRECT_DECLARATOR {;}
          | DIRECT_DECLARATOR {;}
*/
DECLARATOR: DIRECT_DECLARATOR {$$ = $1;}

/*
DIRECT_DECLARATOR: T_IDENTIFIER {;}
                 | T_LBRACKET DECLARATOR T_RBRACKET {;}
                 | DIRECT_DECLARATOR T_LINDEX CONSTANT_EXPRESSION T_RINDEX {;}
                 | DIRECT_DECLARATOR T_LINDEX  T_RINDEX {;}
                 | DIRECT_DECLARATOR T_LBRACKET PARAMETER_TYPE_LIST T_RBRACKET {;}
                 | DIRECT_DECLARATOR T_LBRACKET IDENTIFIER_LIST T_RBRACKET {;}
                 | DIRECT_DECLARATOR T_LBRACKET T_RBRACKET {;}
*/
DIRECT_DECLARATOR: T_IDENTIFIER {$$ = new ASTDirectDeclarator(*$1, NULL);}
                 | T_LBRACKET DECLARATOR T_RBRACKET {$$ = $2;}
                 | T_IDENTIFIER T_LBRACKET T_RBRACKET {$$ = new ASTDirectDeclarator(*$1, NULL);}
                 | T_IDENTIFIER T_LBRACKET PARAMETER_TYPE_LIST T_RBRACKET {$$ = new ASTDirectDeclarator(*$1, $3);}

PARAMETER_TYPE_LIST: PARAMETER_LIST {$$ = $1;}
              //    | PARAMETER_LIST TP_comma T_dot T_dot T_dot {;}

PARAMETER_LIST: PARAMETER_DECLARATION {$$ = new ASTParameterList($1, NULL);}
              | PARAMETER_DECLARATION TP_comma PARAMETER_LIST {$$ = new ASTParameterList($1, $3);}


PARAMETER_DECLARATION: DECLARATION_SPECIFIERS DECLARATOR {$$ = new ASTVariableDeclaration(*$1, $2);}

/*
POINTER: TO_asterix TYPE_QUALIFIER_LIST {;}
       | TO_asterix {;}
       | TO_asterix TYPE_QUALIFIER_LIST POINTER {;}
       | TO_asterix POINTER {;}
*/


/*
IDENTIFIER_LIST: T_IDENTIFIER {;}
               | IDENTIFIER_LIST TP_comma T_IDENTIFIER {;}

*/

TYPE_SPECIFIER: TK_void {$$ = new int(0);}
              | TK_char {$$ = new int(2);}
              | TK_short {$$ = new int(3);}
              | TK_int {$$ = new int(1);}
              | TK_long {$$ = new int(4);}
              | TK_float {$$ = new int(5);}
              | TK_double {$$ = new int(6);}
              | TK_signed {$$ = new int(7);}
              | TK_unsigned {$$ = new int(8);}


TYPE_QUALIFIER: TK_const {;}
              | TK_volatile {;}

EXPRESSION: PRIMARY_EXPRESSION {$$ = $1;}
          | MULTIPLICATIVE_EXPRESSION {$$ = $1;}
          | ADDITIVE_EXPRESSION {$$ = $1;}
          | ASSIGNMENT_EXPRESSION {$$ = $1;}
          | AND_EXPRESSION {$$ = $1;}
          | EXCLUSIVE_OR_EXPRESSION {$$ = $1;}
          | INCLUSIVE_OR_EXPRESSION {$$ = $1;}
          | SHIFT_EXPRESSION {$$ = $1;}
          | RELATIONAL_EXPRESSION {$$ = $1;}
          | EQUALITY_EXPRESSION {$$ = $1;}
          | POSTFIX_EXPRESSION {$$ = $1;}
          | UNARY_EXPRESSION {$$ = $1;}
          | CAST_EXPRESSION {$$ = $1;}
          | LOGICAL_AND_EXPRESSION {$$ = $1;}
          | LOGICAL_OR_EXPRESSION {$$ = $1;}
          | CONDITIONAL_EXPRESSION {$$ = $1;}



PRIMARY_EXPRESSION: TC_integer {$$ = new ASTNumConst($1);}
                  | TC_float {$$ = new ASTNumConst($1);}
                  | T_LBRACKET EXPRESSION T_RBRACKET {$$ = $2;}
                  | T_IDENTIFIER {$$ = new ASTVariableExp(*$1);}

/*
EXPRESSION: POSTFIX_EXPRESSION {;}
          | LOGICAL_AND_EXPRESSION {;}
          | LOGICAL_OR_EXPRESSION {;}
          | CONDITIONAL_EXPRESSION {;}
          | EXPRESSION TP_comma ASSIGNMENT_EXPRESSION {;}




PRIMARY_EXPRESSION: T_IDENTIFIER {;}
                  | CONSTANT {;}
                  | STRING_LITERAL {;}
                  | T_LBRACKET EXPRESSION T_RBRACKET {} {;}
*/
POSTFIX_EXPRESSION: PRIMARY_EXPRESSION {$$ = $1;}
                  | T_IDENTIFIER T_LBRACKET ARGUMENT_EXPRESSION_LIST T_RBRACKET {$$ = new ASTFunctionCall(*$1, $3);}
                  | T_IDENTIFIER T_LBRACKET T_RBRACKET {$$ = new ASTFunctionCall(*$1, NULL);}
                  | POSTFIX_EXPRESSION TO_increment {$$ = new ASTPostfixIncrement($1, 0);}
                  | POSTFIX_EXPRESSION TO_decrement {$$ = new ASTPostfixIncrement($1, 1);}

ARGUMENT_EXPRESSION_LIST: ASSIGNMENT_EXPRESSION {$$ = new ASTArgumentList($1, NULL);}
                        | ASSIGNMENT_EXPRESSION TP_comma ARGUMENT_EXPRESSION_LIST {$$ = new ASTArgumentList($1, $3);}
/*

POSTFIX_EXPRESSION: PRIMARY_EXPRESSION {;}
                  | POSTFIX_EXPRESSION T_LINDEX EXPRESSION T_RINDEX {;}
                  | POSTFIX_EXPRESSION T_LBRACKET ARGUMENT_EXPRESSION_LIST T_RBRACKET {;}
                  | POSTFIX_EXPRESSION T_LBRACKET T_RBRACKET {;}
                  | POSTFIX_EXPRESSION T_dot T_IDENTIFIER {;}
                  | POSTFIX_EXPRESSION TO_memberAccess T_IDENTIFIER {;}
                  | POSTFIX_EXPRESSION TO_increment {;}
                  | POSTFIX_EXPRESSION TO_decrement {;}



UNARY_EXPRESSION: POSTFIX_EXPRESSION {;}
                | TO_increment UNARY_EXPRESSION {;}
                | TO_decrement UNARY_EXPRESSION {;}
                | UNARY_OPERATOR CAST_EXPRESSION {;}
                | TK_sizeof UNARY_EXPRESSION {;}
                | TK_sizeof T_LBRACKET TYPE_NAME T_RBRACKET {;}
*/
UNARY_EXPRESSION: POSTFIX_EXPRESSION {$$ = $1;}
                | TO_not CAST_EXPRESSION { $$ = new ASTLogicNotExpression($2);}
                | TO_bitwiseNot CAST_EXPRESSION {$$ = new ASTbitwiseNotExpression($2);}
                | TO_minus CAST_EXPRESSION {$$ = new ASTNegativeOperator($2);}
                | TO_plus CAST_EXPRESSION {$$ = $2;}
                | TO_increment UNARY_EXPRESSION {$$ = new ASTPrefixIncrement($2, 0);}
                | TO_decrement UNARY_EXPRESSION {$$ = new ASTPrefixIncrement($2, 1);}
                | TK_sizeof T_LBRACKET TYPE_NAME T_RBRACKET {$$ = new ASTSizeOfExpression(*$3);}


TYPE_NAME: TK_char {$$ = new int(1);}
         | TK_unsigned TK_char {$$ = new int(1);}
         | TK_signed TK_char {$$ = new int(1);}
         | TK_short {$$ = new int(2);}
         | TK_signed TK_short {$$ = new int(2);}
         | TK_unsigned TK_short {$$ = new int(2);}
         | TK_int {$$ = new int(4);}
         | TK_signed TK_int {$$ = new int(4);}
         | TK_unsigned TK_int {$$ = new int(4);}
         | TK_long {$$ = new int(4);}
         | TK_signed TK_long {$$ = new int(4);}
         | TK_unsigned TK_long {$$ = new int(4);}
         | TK_enum {$$ = new int(4);}
         | TYPE_NAME TO_asterix {$$ = new int(4);}
         | TYPE_NAME T_LBRACKET TO_asterix T_RBRACKET T_LBRACKET T_RBRACKET {$$ = new int(4);}
         | TK_float {$$ = new int(4);}
         | TK_double {$$ = new int(8);}
         | TK_long TK_double {$$ = new int(8);}


/*
UNARY_OPERATOR: TO_ampersand {;}
              | TO_asterix {;}

*/
/*
CAST_EXPRESSION: UNARY_EXPRESSION {;}
               | T_LBRACKET TYPE_NAME T_RBRACKET {;}
*/

CAST_EXPRESSION: UNARY_EXPRESSION {$$ = $1;}

MULTIPLICATIVE_EXPRESSION: CAST_EXPRESSION {$$ = $1;}
                         | MULTIPLICATIVE_EXPRESSION TO_asterix CAST_EXPRESSION {$$ = new ASTMultiplicativeExpression($1, $3, 1);;}
                         | MULTIPLICATIVE_EXPRESSION TO_divide CAST_EXPRESSION {$$ = new ASTMultiplicativeExpression($1, $3, 2);;}
                         | MULTIPLICATIVE_EXPRESSION TO_mod CAST_EXPRESSION {$$ = new ASTMultiplicativeExpression($1, $3, 3);;}

ADDITIVE_EXPRESSION: MULTIPLICATIVE_EXPRESSION {$$ = $1;}
                   | ADDITIVE_EXPRESSION TO_plus MULTIPLICATIVE_EXPRESSION {$$ = new ASTAdditiveExpression($1, $3, 1);}
                   | ADDITIVE_EXPRESSION TO_minus MULTIPLICATIVE_EXPRESSION {$$ = new ASTAdditiveExpression($1, $3, 2);}


CONSTANT_EXPRESSION: CONDITIONAL_EXPRESSION {$$ = $1;}


SHIFT_EXPRESSION: ADDITIVE_EXPRESSION {$$ = $1;}
                | SHIFT_EXPRESSION TO_bitwiseLeft ADDITIVE_EXPRESSION {$$ = new ASTShiftExpression($3, $1, 1);}
                | SHIFT_EXPRESSION TO_bitwiseRight ADDITIVE_EXPRESSION {$$ = new ASTShiftExpression($3, $1, 1);}

RELATIONAL_EXPRESSION: SHIFT_EXPRESSION {$$ = $1;}
                     | RELATIONAL_EXPRESSION TO_lessThan SHIFT_EXPRESSION {$$ = new ASTRelationalExpression($3, $1, 1);}
                     | RELATIONAL_EXPRESSION TO_moreThan SHIFT_EXPRESSION {$$ = new ASTRelationalExpression($3, $1, 2);}
                     | RELATIONAL_EXPRESSION TO_lessThanOrEqual SHIFT_EXPRESSION {$$ = new ASTRelationalExpression($3, $1, 3);}
                     | RELATIONAL_EXPRESSION TO_moreThanOrEqual SHIFT_EXPRESSION {$$ = new ASTRelationalExpression($3, $1, 4);}

EQUALITY_EXPRESSION: RELATIONAL_EXPRESSION {$$ = $1;}
                   | EQUALITY_EXPRESSION TO_equalTo RELATIONAL_EXPRESSION {$$ = new ASTEqualityExpression($3, $1, 1);}
                   | EQUALITY_EXPRESSION TO_notEqualTo RELATIONAL_EXPRESSION {$$ = new ASTEqualityExpression($3, $1, 2);}

AND_EXPRESSION: EQUALITY_EXPRESSION {$$ = $1;}
              | AND_EXPRESSION TO_ampersand ADDITIVE_EXPRESSION {$$ = new ASTBitwiseAnd($3 ,$1);}

EXCLUSIVE_OR_EXPRESSION: AND_EXPRESSION {$$ = $1;}
                       | EXCLUSIVE_OR_EXPRESSION TO_exclussiveOr AND_EXPRESSION {$$ = new ASTExclusiveOr($3, $1);}

INCLUSIVE_OR_EXPRESSION: EXCLUSIVE_OR_EXPRESSION {$$ = $1;}
                       | INCLUSIVE_OR_EXPRESSION TO_OR EXCLUSIVE_OR_EXPRESSION {$$ = new ASTInclusiveOr($3, $1);}

CONDITIONAL_EXPRESSION: LOGICAL_OR_EXPRESSION {$$ = $1;}
                      | LOGICAL_OR_EXPRESSION TO_questionMark EXPRESSION TP_colon CONDITIONAL_EXPRESSION {$$ = new ASTConditionalExpression($1, $3, $5);}

LOGICAL_AND_EXPRESSION: INCLUSIVE_OR_EXPRESSION {$$ = $1;}
                      | LOGICAL_AND_EXPRESSION TO_logicAnd INCLUSIVE_OR_EXPRESSION {$$ = new ASTLogicANDExpression($1, $3);}

LOGICAL_OR_EXPRESSION: LOGICAL_AND_EXPRESSION {$$ = $1;}
                     | LOGICAL_OR_EXPRESSION TO_logicOr LOGICAL_AND_EXPRESSION {$$ = new ASTLogicORExpression($1, $3);}

ASSIGNMENT_EXPRESSION: CONDITIONAL_EXPRESSION {$$ = $1;}
                     | UNARY_EXPRESSION ASSIGNMENT_OPERATOR ASSIGNMENT_EXPRESSION {$$ = new ASTAssignmentExpression($1, $3, *$2);}

ASSIGNMENT_OPERATOR: TO_equal {$$ = new int(0);}
                   | TO_multEqual {$$ = new int(1);}
                   | TO_divEqual {$$ = new int(2);}
                   | TO_modEqual {$$ = new int(3);}
                   | TO_plusEqual {$$ = new int(4);}
                   | TO_minusEqual {$$ = new int(5);}
                   | TO_rightEqual {$$ = new int(6);}
                   | TO_leftEqual {$$ = new int(7);}
                   | TO_andEqual {$$ = new int(8);}
                   | TO_exOrEqual {$$ = new int(9);}
                   | TO_OrEqual {$$ = new int(10);}


%%

 ASTTranslationUnit *ASTRoot; // Definition of variable (to match declaration earlier)

 ASTTranslationUnit *parseAST()
{
  ASTRoot=0;
  yyparse();
  return ASTRoot;
}
