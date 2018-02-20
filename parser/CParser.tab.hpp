/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_CPARSER_TAB_HPP_INCLUDED
# define YY_YY_CPARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "CParser.y" /* yacc.c:1909  */

  #include <string>
  #include <stdio.h>
  #include <iostream>

  #include "AST.hpp"

  extern ASTNode *ASTRoot;

  int yylex(void)
  void yyerror(const char*);


#line 58 "CParser.tab.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_IDENTIFIER = 258,
    T_LBRACE = 259,
    T_RBRACE = 260,
    T_LBRACKET = 261,
    T_RBRACKET = 262,
    T_LINDEX = 263,
    T_RINDEX = 264,
    TK_auto = 265,
    TK_break = 266,
    TK_case = 267,
    TK_char = 268,
    TK_const = 269,
    TK_continue = 270,
    TK_default = 271,
    TK_do = 272,
    TK_double = 273,
    TK_else = 274,
    TK_enum = 275,
    TK_extern = 276,
    TK_float = 277,
    TK_for = 278,
    TK_goto = 279,
    TK_if = 280,
    TK_int = 281,
    TK_long = 282,
    TK_register = 283,
    TK_return = 284,
    TK_short = 285,
    TK_signed = 286,
    TK_sizeof = 287,
    TK_static = 288,
    TK_struct = 289,
    TK_switch = 290,
    TK_typedef = 291,
    TK_union = 292,
    TK_unsigned = 293,
    TK_void = 294,
    TK_volatile = 295,
    TK_while = 296,
    T_StringLiteral = 297,
    T_LStringLiteral = 298,
    T_dot = 299,
    TO_memberAccess = 300,
    TO_not = 301,
    TO_bitwiseNot = 302,
    TO_ampersand = 303,
    TO_logicAnd = 304,
    TO_asterix = 305,
    TO_mod = 306,
    TO_bitwiseLeft = 307,
    TO_bitwiseRight = 308,
    TO_lessThan = 309,
    TO_moreThan = 310,
    TO_lessThanOrEqual = 311,
    TO_moreThanOrEqual = 312,
    TO_equalTo = 313,
    TO_notEqualTo = 314,
    TO_plus = 315,
    TO_minus = 316,
    TO_increment = 317,
    TO_decrement = 318,
    TP_comma = 319,
    TP_colon = 320,
    TP_semiColon = 321,
    TC_integer = 322,
    TC_unsigned = 323,
    TC_long = 324,
    TC_longLong = 325,
    TC_float = 326,
    TC_longDouble = 327,
    TC_true = 328,
    TC_false = 329,
    TC_NULL = 330,
    TC_nullptr = 331
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 38 "CParser.y" /* yacc.c:1909  */

ASTNode* Node;
ASTStatement Statement;
ASTPrimitive Primitive;
ASTExpression* Expression;
ASTFunction* Function;
ASTInteger* Integer;
string Identifier;
ASTFunction Function;

#line 158 "CParser.tab.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CPARSER_TAB_HPP_INCLUDED  */
