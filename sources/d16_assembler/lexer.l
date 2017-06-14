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
WORD [a-zA-Z0-9_]+
HEXA_INTEGER 0x[0-9a-fA-F]+
DEC_INTEGER [0-9]+
INTEGER [-]?({HEXA_INTEGER}|{DEC_INTEGER})
IMMEDIATE [$]{INTEGER}

%x COMMENT

%option nounput

%%

"/*" {BEGIN COMMENT;}
<COMMENT>\n {line++;}
<COMMENT>. {}
<COMMENT>"*/" {BEGIN INITIAL;printf("\n");}

"//".*\n {line++;}

(NOP|nop) {return tNOP;}
(ADD|add) {return tADD;}
(SUB|sub) {return tSUB;}
(SHL|shl) {return tSHL;}
(SHR|shr) {return tSHR;}
(EQU|equ) {return tEQU;}
(AFC|afc) {return tAFC;}
(COP|cop) {return tCOP;}
(LOD|lod) {return tLOD;}
(LOP|lop) {return tLOP;}
(STR|str) {return tSTR;}
(STP|stp) {return tSTP;}
(JMP|jmp) {return tJMP;}
(JMZ|jmz) {return tJMZ;}
(JMR|jmr) {return tJMR;}

[rR] {return tREG;}
_ {return tREGSEP;}
[,] {return tC;}
[(] {return tLPAR;}
[)] {return tRPAR;}

{IMMEDIATE} {yylval.integer = strtol(yytext+1, NULL, 0); return tIMMEDIATE;}
{INTEGER} {yylval.integer = strtol(yytext, NULL, 0); return tINTEGER;}
{WHITESPACE} {}
\n {line++;}
\${WORD} {yytext++; yylval.string = strdup(yytext); return tRLABEL;}
\*{WORD} {yytext++; yylval.string = strdup(yytext); return tULABEL;}
{WORD}: {yytext[(yytext[0] == '\0') ? 0 : strlen(yytext) - 1] = '\0';
  yylval.string = strdup(yytext); return tDLABEL;}
'.' {yylval.integer = yytext[1]; return tCHAR;}
. {return tERROR;}

%%

int line = 1;
