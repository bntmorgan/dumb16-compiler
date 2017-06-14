/*
Copyright (C) 2017  Benoît Morgan

This file is part of dumb16-compiler.

dumb16-compiler is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

dumb16-compiler is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dumb16-compiler.  If not, see <http://www.gnu.org/licenses/>.
*/

%{
  #include "sem.h"
  #include "parser.h"
  extern int line;
  #define YY_NO_INPUT
%}

WHITESPACE [ \t]
ALPHA [a-zA-Z_]
WORD [a-zA-Z0-9_]
HEXA_INTEGER 0x[0-9a-fA-F]+
DEC_INTEGER [0-9]+
INTEGER {HEXA_INTEGER}|{DEC_INTEGER}
ID {ALPHA}{WORD}*

%x COMMENT

%option nounput

%%

"/*" {BEGIN COMMENT;}
<COMMENT>\n {line++;}
<COMMENT>. {}
<COMMENT>"*/" {BEGIN INITIAL;printf("\n");}

"//".*\n {line++;}

"=" {return tAFC;}
"+" {return tADD;}
"-" {return tSUB;}
"*" {return tSTAR;}
"/" {return tDIV;}
"|" {return tOR;}
"&" {return tAND;}
"{" {return tLBRACE;}
"}" {return tRBRACE;}
"[" {return tLBRACKET;}
"]" {return tRBRACKET;}
"(" {return tLPAR;}
")" {return tRPAR;}
"," {return tCOMMA;}
";" {return tSEMI;}
"==" {return tEQU;}
int {return tINT;}
void {return tVOID;}
return {return tRETURN;}
if {return tIF;}
else {return tELSE;}
while {return tWHILE;}

{INTEGER} {yylval.integer = strtol(yytext, NULL, 0); return tINTEGER;}
{WHITESPACE} {}
{ID} {yylval.string = strdup(yytext); return tID;}
\n {line++;}
'.' {yylval.integer = yytext[1]; return tCHAR;}
. {return tERROR;}

%%

int line = 1;
