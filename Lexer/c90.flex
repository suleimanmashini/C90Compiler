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
{ID}+({ID}|{DIGIT})*     {yylval.string = new std::string(yytext);return T_IDENTIFIER; }
{DIGIT}+                 {yylval.number = strtod(yytext, 0); return T_INTEGER; }
[{]                      {return T_LBRACE;}
[}]                      {return T_RBRACE;}
[(]                      {return T_LBRACKET;}
[)]                      {return T_RBRACKET;}

%%

 void yyerror (char const *s) {
   fprintf (stderr, "Parse Error: %s\n", s);
 }
