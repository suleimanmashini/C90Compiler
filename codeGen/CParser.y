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
%type <Node> STATEMENT DECLARATION_SPECIFIERS
%type <Node> TRANSLATION_UNIT
%type <Function> FUNCTION_DEFINITION EXTERNAL_DECLARATION
%type <Statement> COMPOUND_STATEMENT
%type <Number> TYPE_SPECIFIER
%type <Declarator> DECLARATOR


%union {
const ASTNode* Node;
const ASTStatement* Statement;
const ASTDeclaration* Declarator;
const std::string* word;
const ASTFunctionDefinition* Function;
const ASTExpression* Expression
const int* Number;
}

%start TRANSLATION_UNIT

%%

TRANSLATION_UNIT: EXTERNAL_DECLARATION {$$ = $1;}
                | TRANSLATION_UNIT EXTERNAL_DECLARATION {;}
/*
EXTERNAL_DECLARATION: FUNCTION_DEFINITION {$$ = $1;}
                    | DECLARATION {;}
*/
EXTERNAL_DECLARATION: FUNCTION_DEFINITION {$$ = $1;}
/*
FUNCTION_DEFINITION: DECLARATION_SPECIFIERS DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT {;}
                   | DECLARATOR DECLARATION_LIST COMPOUND_STATEMENT {;}
                   | DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT {$$ = new ASTFunctionDefinition($1, $2, $3);}
                   | DECLARATOR COMPOUND_STATEMENT {;}
*/
FUNCTION_DEFINITION: DECLARATION_SPECIFIERS DECLARATOR COMPOUND_STATEMENT {$$ = new ASTFunctionDefinition($1, $2, $3);}

/*
STATEMENT: LABELED_STATEMENT {;}
         | COMPOUND_STATEMENT {;}
         | EXPRESSION_STATEMENT {;}
         | SELECTION_STATEMENT {;}
         | ITERATION_STATEMENT {;}
         | JUMP_STATEMENT {;}
*/
STATEMENT: COMPOUND_STATEMENT {;}
         | JUMP_STATEMENT {;}
/*
LABELLED_STATEMENT: T_IDENTIFIER TP_colon STATEMENT {;}
                  | TK_case CONSTANT_EXPRESSION TP_colon STATEMENT {;}
                  | TK_default TP_colon STATEMENT {;}
*/

/*
COMPOUND_STATEMENT: T_LBRACE DECLARATION_LIST STATEMENT_LIST T_RBRACE {;}
                  | T_LBRACE STATEMENT_LIST T_RBRACE {;}
                  | T_LBRACE DECLARATION_LIST T_RBRACE {;}
                  | T_LBRACE T_RBRACE {;}
*/
COMPOUND_STATEMENT: T_LBRACE STATEMENT_LIST T_RBRACE {;}
/*
DECLARATION_LIST: DECLARATION {;}
                | DECLARATION_LIST DECLARATION {;}
*/
STATEMENT_LIST: STATEMENT {;}
              | STATEMENT_LIST STATEMENT {;}

/*
EXPRESSION_STATEMENT: EXPRESSION TP_semiColon {;}
                    | TP_semiColon {;}

SELECTION_STATEMENT: TK_if T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {;}
                   | TK_if T_LBRACKET EXPRESSION T_RBRACKET STATEMENT TK_else STATEMENT {;}
                   | TK_switch T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {;}

ITERATION_STATEMENT: TK_while T_LBRACKET EXPRESSION T_RBRACKET STATEMENT {;}
                   | TK_do STATEMENT TK_while T_LBRACKET EXPRESSION T_RBRACKET TP_semiColon {;}
                   | TK_for T_LBRACKET EXPRESSION TP_semiColon EXPRESSION TP_semiColon EXPRESSION T_LBRACKET STATEMENT {;}
*/
/*
JUMP_STATEMENT: TK_goto T_IDENTIFIER TP_semiColon {;}
              | TK_continue TP_semiColon {;}
              | TK_break TP_semiColon {;}
              | TK_return EXPRESSION TP_semiColon {;}
              | TK_return TP_semiColon {;}
*/

JUMP_STATEMENT: TK_return TP_semiColon {;}

/*
DECLARATION: DECLARATION_SPECIFIERS {;}
           | DECLARATION_SPECIFIERS INIT_DECLARATION_LIST {;}
*/
/*
DECLARATION_SPECIFIERS: STORAGE_CLASS_SPECIFIER DELCARATION DECLARATION_SPECIFIERS {;}
                       | TYPE_SPECIFIER DECLARATION_SPECIFIERS {;}
                       | TYPE_SPECIFIER {;}
                       | TYPE_QUALIFIER DECLARATION_SPECIFIERS {;}
                       | TYPE_QUALIFIER {;}

INIT_DECLARATION_LIST: INIT_DECLARATOR {;}
                     | INIT_DECLARATOR_LIST TP_comma INIT_DECLARATOR DECLARATION_SPECIFIERS {;}
*/
DECLARATION_SPECIFIERS: TYPE_SPECIFIER {;}
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
DECLARATOR: DIRECT_DECLARATOR {;}

/*
DIRECT_DECLARATOR: T_IDENTIFIER {;}
                 | T_LBRACKET DECLARATOR T_RBRACKET {;}
                 | DIRECT_DECLARATOR T_LINDEX CONSTANT_EXPRESSION T_RINDEX {;}
                 | DIRECT_DECLARATOR T_LINDEX  T_RINDEX {;}
                 | DIRECT_DECLARATOR T_LBRACKET PARAMETER_TYPE_LIST T_RBRACKET {;}
                 | DIRECT_DECLARATOR T_LBRACKET IDENTIFIER_LIST T_RBRACKET {;}
                 | DIRECT_DECLARATOR T_LBRACKET T_RBRACKET {;}
*/
DIRECT_DECLARATOR: T_IDENTIFIER {;}
/*
POINTER: TO_asterix TYPE_QUALIFIER_LIST {;}
       | TO_asterix {;}
       | TO_asterix TYPE_QUALIFIER_LIST POINTER {;}
       | TO_asterix POINTER {;}
*/

/*
PARAMETER_TYPE_LIST: PARAMETER_LIST {;}
                   | PARAMETER_LIST TP_comma T_dot T_dot T_dot {;}

PARAMETER_LIST: PARAMETER_DECLARATION {;}
              | PARAMETER_LIST TP_comma PARAMETER_DECLARATION {;}


PARAMETER_DECLARATION: DECLARATION_SPECIFIERS DECLARATION {;}
*/
/*
IDENTIFIER_LIST: T_IDENTIFIER {;}
               | IDENTIFIER_LIST TP_comma T_IDENTIFIER {;}

*/

TYPE_SPECIFIER: TK_void {$$ = new int(0);}
              | TK_char {;}
              | TK_short {;}
              | TK_int {$$ = new int(1);}
              | TK_long {;}
              | TK_float {;}
              | TK_double {;}
              | TK_signed {;}
              | TK_unsigned {;}

/*
TYPE_QUALIFIER: TK_const {;}
              | TK_volatile {;}
*/
/*
EXPRESSION: PRIMARY_EXPRESSION {$$ = $1;}
          | POSTFIX_EXPRESSION {;}
          | UNARY_EXPRESSION {;}
          | MULTIPLICATIVE_EXPRESSION {;}
          | ADDITIVE_EXPRESSION {;}
          | SHIFT_EXPRESSION {;}
          | RELATIONAL_EXPRESSION {;}
          | EQUALITY_EXPRESSION {;}
          | AND_EXPRESSION {;}
          | EXCLUSIVE_OR_EXPRESSION {;}
          | INCLUSIVE_OR_EXPRESSION {;}
          | LOGICAL_AND_EXPRESSION {;}
          | LOGICAL_OR_EXPRESSION {;}
          | CONDITIONAL_EXPRESSION {;}
          | ASSIGNMENT_EXPRESSION {;}
          | ASSIGNMENT_EXPRESSION {;}
          | EXPRESSION TP_comma ASSIGNMENT_EXPRESSION {;}


PRIMARY_EXPRESSION: T_IDENTIFIER {;}
                  | CONSTANT {;}
                  | STRING_LITERAL {;}
                  | T_LBRACKET EXPRESSION T_RBRACKET {} {;}


POSTFIX_EXPRESSION: PRIMARY_EXPRESSION {;}
                  | POSTFIX_EXPRESSION T_LINDEX EXPRESSION T_RINDEX {;}
                  | POSTFIX_EXPRESSION T_LBRACKET ARGUMENT_EXPRESSION_LIST T_RBRACKET {;}
                  | POSTFIX_EXPRESSION T_LBRACKET T_RBRACKET {;}
                  | POSTFIX_EXPRESSION T_dot T_IDENTIFIER {;}
                  | POSTFIX_EXPRESSION TO_memberAccess T_IDENTIFIER {;}
                  | POSTFIX_EXPRESSION TO_increment {;}
                  | POSTFIX_EXPRESSION TO_decrement {;}

ARGUMENT_EXPRESSION_LIST: ASSIGNMENT_EXPRESSION {;}
                        | ASSIGNMENT_EXPRESSION_LIST TP_comma ASSIGNMENT_EXPRESSION {;}

UNARY_EXPRESSION: POSTFIX_EXPRESSION {;}
                | TO_increment UNARY_EXPRESSION {;}
                | TO_decrement UNARY_EXPRESSION {;}
                | UNARY_OPERATOR CAST_EXPRESSION {;}
                | TK_sizeof UNARY_EXPRESSION {;}
                | TK_sizeof T_LBRACKET TYPE_NAME T_RBRACKET {;}

UNARY_OPERATOR: TO_ampersand {;}
              | TO_asterix {;}
              | TO_plus {;}
              | TO_minus {;}
              | TO_bitwiseNot {;}
              | TO_not {;}

CAST_EXPRESSION: UNARY_EXPRESSION {;}
               | T_LBRACKET TYPE_NAME T_RBRACKET {;}

MULTIPLICATIVE_EXPRESSION: CAST_EXPRESSION {;}
                         | MULTIPLICATIVE_EXPRESSION TO_asterix CAST_EXPRESSION {;}
                         | MULTIPLICATIVE_EXPRESSION TO_divide CAST_EXPRESSION {;}
                         | MULTIPLICATIVE_EXPRESSION TO_mod CAST_EXPRESSION {;}

ADDITIVE_EXPRESSION: MULTIPLICATIVE_EXPRESSION {;}
                   | ADDITIVE_EXPRESSION TO_plus MULTIPLICATIVE_EXPRESSION {;}
                   | ADDITIVE_EXPRESSION TO_minus MULTIPLICATIVE_EXPRESSION {;}

SHIFT_EXPRESSION: ADDITIVE_EXPRESSION {;}
                | SHIFT_EXPRESSION TO_bitwiseLeft ADDITIVE_EXPRESSION {;}
                | SHIFT_EXPRESSION TO_bitwiseRight ADDITIVE_EXPRESSION {;}

RELATIONAL_EXPRESSION: SHIFT_EXPRESSION {;}
                     | RELATIONAL_EXPRESSION TO_lessThan SHIFT_EXPRESSION {;}
                     | RELATIONAL_EXPRESSION TO_moreThan SHIFT_EXPRESSION {;}
                     | RELATIONAL_EXPRESSION TO_lessThanOrEqual SHIFT_EXPRESSION {;}
                     | RELATIONAL_EXPRESSION TO_moreThanOrEqual SHIFT_EXPRESSION {;}

CONSTANT_EXPRESSION: CONDITIONAL_EXPRESSION {;}

EQUALITY_EXPRESSION: RELATIONAL_EXPRESSION {;}
                   | EQUALITY_EXPRESSION TO_equalTo RELATIONAL_EXPRESSION {;}
                   | EQUALITY_EXPRESSION TO_notEqualTo RELATIONAL_EXPRESSION {;}

AND_EXPRESSION: EQUALITY_EXPRESSION {;}
              | AND_EXPRESSION TO_ampersand EQUALITY_EXPRESSION {;}

EXCLUSIVE_OR_EXPRESSION: AND_EXPRESSION {;}
                       | EXCLUSIVE_OR_EXPRESSION TO_exclussiveOr AND_EXPRESSION {;}

INCLUSIVE_OR_EXPRESSION: EXCLUSIVE_OR_EXPRESSION {;}
                       | INCLUSIVE_OR_EXPRESSION TO_OR EXCLUSIVE_OR_EXPRESSION {;}

LOGICAL_AND_EXPRESSION: INCLUSIVE_OR_EXPRESSION {;}
                      | LOGICAL_AND_EXPRESSION TO_logicAnd INCLUSIVE_OR_EXPRESSION {;}

LOGICAL_OR_EXPRESSION: LOGICAL_AND_EXPRESSION {;}
                     | LOGICAL_OR_EXPRESSION TO_logicOr LOGICAL_AND_EXPRESSION {;}

CONDITIONAL_EXPRESSION: LOGICAL_OR_EXPRESSION {;}
                      | LOGICAL_OR_EXPRESSION TO_questionMark EXPRESSION TP_colon CONDITIONAL_EXPRESSION {;}

ASSIGNMENT_EXPRESSION: CONDITIONAL_EXPRESSION {;}
                     | UNARY_EXPRESSION ASSIGNMENT_OPERATOR ASSIGNMENT_EXPRESSION {;}

ASSIGNMENT_OPERATOR: TO_equal {;}
                   | TO_multEqual {;}
                   | TO_divEqual {;}
                   | TO_modEqual {;}
                   | TO_plusEqual {;}
                   | TO_minusEqual {;}
                   | TO_rightEqual {;}
                   | TO_leftEqual {;}
                   | TO_andEqual {;}
                   | TO_exOrEqual {;}
                   | TO_OrEqual {;}

*/


%%

const ASTNode *ASTRoot; // Definition of variable (to match declaration earlier)

const ASTNode *parseAST()
{
  ASTRoot=0;
  yyparse();
  return ASTRoot;
}
