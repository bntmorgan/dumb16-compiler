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

  extern void yyerror(const char *s);
  extern int yylex();
%}

%error-verbose

// Declaration des types utilisés
%union {
  int integer;
  char *string;
};

// Definition des types des tokens
%token <integer> tNOP tADD tSUB tSHL tSHR tAFC tCOP tLOP tLOD tSTR tSTP tJMP
%token <integer> tJMZ tJMR tCHAR tEQU
%token <integer> tINTEGER tERROR tC tRPAR tLPAR
%token <integer> tREG tREGSEP tDOLLAR tIMMEDIATE
%token <string> tULABEL tDLABEL tRLABEL

%type <integer> reg

// Axiome
%start instructions

%%

instructions  : instruction instructions;
              | ;

instruction : tNOP {
  struct ins i = {
    .opcode = D16_OP_NOP,
    .op0 = 0,
    .op1 = 0,
    .op2 = 0,
  };
  sem_nop(&i);
}

instruction : tADD reg tC reg tC reg {
  struct ins i = {
    .opcode = D16_OP_ADD,
    .op0 = $2,
    .op1 = $4,
    .op2 = $6,
  };
  sem_add(&i);
}

            | tSUB reg tC reg tC reg {
  struct ins i = {
    .opcode = D16_OP_SUB,
    .op0 = $2,
    .op1 = $4,
    .op2 = $6,
  };
  sem_sub(&i);
}

            | tSHL reg tC reg tC reg {
  struct ins i = {
    .opcode = D16_OP_SHL,
    .op0 = $2,
    .op1 = $4,
    .op2 = $6,
  };
  sem_shl(&i);
}

            | tSHR reg tC reg tC reg {
  struct ins i = {
    .opcode = D16_OP_SHR,
    .op0 = $2,
    .op1 = $4,
    .op2 = $6,
  };
  sem_shr(&i);
}

            | tEQU reg tC reg tC reg {
  struct ins i = {
    .opcode = D16_OP_EQU,
    .op0 = $2,
    .op1 = $4,
    .op2 = $6,
  };
  sem_equ(&i);
}

            | tAFC reg tC tIMMEDIATE {
  d16_assert_immediate($4);
  struct ins i = {
    .opcode = D16_OP_AFC,
    .op0 = $2,
    .op1 = ($4 >> 8) & 0xff,
    .op2 = $4 & 0xff,
  };
  sem_afc(&i);
}

            | tAFC reg tC tCHAR {
  d16_assert_immediate($4);
  struct ins i = {
    .opcode = D16_OP_AFC,
    .op0 = $2,
    .op1 = ($4 >> 8) & 0xff,
    .op2 = $4 & 0xff,
  };
  sem_afc(&i);
}

            | tAFC reg tC tRLABEL {
  struct ins i = {
    .opcode = D16_OP_AFC,
    .op0 = $2,
    .sym = $4,
  };
  sem_afc(&i);
}

            | tCOP reg tC reg {
  struct ins i = {
    .opcode = D16_OP_COP,
    .op0 = $2,
    .op1 = $4,
    .op2 = 0,
  };
  sem_cop(&i);
}

            | tJMZ tULABEL tC reg {
  struct ins i = {
    .opcode = D16_OP_JMZ,
    .op2 = $4,
    .sym = $2,
  };
  sem_jmz(&i);
}
            | tJMZ tINTEGER tC reg {
  d16_assert_address($2);
  struct ins i = {
    .opcode = D16_OP_JMZ,
    .op0 = ($2 >> 8) & 0xff,
    .op1 = $2 & 0xff,
    .op2 = $4,
  };
  sem_jmz(&i);
}

            | tJMP tULABEL {
  struct ins i = {
    .opcode = D16_OP_JMP,
    .sym = $2,
  };
  sem_jmp(&i);
}
            | tJMP tINTEGER {
  d16_assert_address($2);
  struct ins i = {
    .opcode = D16_OP_JMP,
    .op0 = ($2 >> 8) & 0xff,
    .op1 = $2 & 0xff,
    .op2 = 0,
  };
  sem_jmp(&i);
}

            | tJMR reg {
  struct ins i = {
    .opcode = D16_OP_JMR,
    .op0 = $2,
    .op1 = 0,
    .op2 = 0,
  };
  sem_jmp(&i);
}

            | tLOD reg tC tINTEGER {
  d16_assert_address($2);
  struct ins i = {
    .opcode = D16_OP_LOD,
    .op0 = $2,
    .op1 = ($4 >> 8) & 0xff,
    .op2 = $4 & 0xff,
  };
  sem_lod(&i);
}

            | tLOP reg tC tINTEGER tLPAR reg tRPAR {
  int8_t a = $4;
  d16_assert_index(a);
  struct ins i = {
    .opcode = D16_OP_LOP,
    .op0 = $2,
    .op1 = a,
    .op2 = $6,
  };
  sem_lop(&i);
}

            | tLOP reg tC tLPAR reg tRPAR {
  struct ins i = {
    .opcode = D16_OP_LOP,
    .op0 = $2,
    .op1 = 0,
    .op2 = $5,
  };
  sem_lop(&i);
}

            | tSTR tINTEGER tC reg {
  d16_assert_address($2);
  struct ins i = {
    .opcode = D16_OP_STR,
    .op0 = ($2 >> 8) & 0xff,
    .op1 = $2 & 0xff,
    .op2 = $4,
  };
  sem_str(&i);
}

            | tSTP tINTEGER tLPAR reg tRPAR tC reg {
  int8_t a = $2;
  d16_assert_index(a);
  struct ins i = {
    .opcode = D16_OP_STP,
    .op0 = a,
    .op1 = $4,
    .op2 = $7,
  };
  sem_stp(&i);
}
            | tSTP tLPAR reg tRPAR tC reg {
  struct ins i = {
    .opcode = D16_OP_STP,
    .op0 = 0,
    .op1 = $3,
    .op2 = $6,
  };
  sem_stp(&i);
}

            | tDLABEL {
  sem_label($1);
}

reg : tREG tINTEGER {
  $$ = $2;
}
%%
