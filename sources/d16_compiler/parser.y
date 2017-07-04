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
  #include <stdlib.h>
  #include <stdio.h>
  #include "sem.h"
  #include "sym.h"
  #include "d16.h"
  #include "parser.h"

  extern void yyerror(const char *s);
  extern int yylex();

  char *cfun; // current function
  unsigned int n; // number of arguments
%}

%error-verbose

// Types declaration
%union {
  int integer;
  char *string;
};

// Token definition
%token tEQU tADD tSUB tSTAR tDIV tLBRACE tRBRACE tLBRACKET tRBRACKET tOR tAND
%token tLPAR tRPAR tCOMMA tINT tRETURN tIF tELSE tWHILE tERROR tSEMI tVOID
%token tAFC tGT tLT tGTE tLTE
%token<integer> tINTEGER tCHAR
%token<string> tID

// Axiome
%start c

// Adjust rules priorities
%nonassoc ELSE
%nonassoc tELSE

// Token priorities
%nonassoc tAFC
%left tEQU tLT tGT tLTE tGTE
%left tOR tAND
%left tADD tSUB
%left tSTAR tDIV

%%

c : prototype proto_body c | ;

prototype : tINT tID tLPAR { n = 0; } p_params tRPAR {
  cfun = $2;
  printf("prototype start(%s), n(%u)\n", cfun, n);
}

proto_body : tSEMI {
  sem_proto_end(cfun, n);
  printf("prototype end(%s)\n", cfun);
} | {
  printf("body begin(%s)\n", cfun);
  sem_body_begin(cfun, n);
} block {
  sem_body_end();
  printf("body end(%s)\n", cfun);
}

block : tLBRACE {
  printf("block start\n");
  sym_depth_inc();
} statements tRBRACE {
  sym_print_all((struct sym*)syms.t);
  sym_depth_dec();
  printf("block end\n");
}

statements : statement_control statements | statement_control ;

p_params : p_param tCOMMA p_params | p_param;

p_param :
    tINT tID { sym_add($2, TYPE_INT, 1); n++; } |
    tVOID ;

statement_control : control | statement tSEMI ;

control : if | while ;

statement : afc | return | declaration ;

declaration : tINT ids { }

ids : id tCOMMA ids | id ;

id : tID {
  sym_add($1, TYPE_INT, 0);
  sym_print_all((struct sym*)syms.t);
  printf("declaration end\n");
} | tID {
  printf("declaration afc start\n");
  sym_add($1, TYPE_INT, 0);
  sym_print_all((struct sym*)syms.t);
} tAFC exp {
  sem_afc($1);
  printf("declaration afc end\n");
}

afc : tID tAFC exp {
  sem_afc($1);
  printf("afc end\n");
} | tSTAR tID tAFC exp {
  sem_afc_pointer($2);
  printf("afc pointer end\n");
} | tID tLBRACKET exp tRBRACKET tAFC exp {
  sem_afc_bracket($1);
  printf("afc pointer end\n");
}

while : tWHILE { sem_while_begin(); } tLPAR exp {
  sem_while_cond();
  printf("condition end\n");
} tRPAR while_statement_control_block  {
  sem_while_end();
  printf("while end\n");
}

while_statement_control_block : statement_control | block ;

if : tIF { sem_if_begin(); } tLPAR exp {
  sem_if_cond();
  printf("condition end\n");
} tRPAR if_statement_control_block {
  sem_if_end();
  printf("if end\n");
}

if_statement_control_block :
    statement_control else_none |
    block else_none ;

else_none : tELSE {
  sem_else();
} statement_control | tELSE {
  sem_else();
} block | %prec ELSE {
  sem_end_then();
}

return : tRETURN exp {
  sem_return();
  printf("return end\n");
}

exp :
  tLPAR exp tRPAR |
  exp tADD exp {
    sem_exp_op(D16_OP_ADD);
  printf("add end\n");
  } |
  exp tSUB exp {
    sem_exp_op(D16_OP_SUB);
    printf("sub end\n");
  } |
  exp tSTAR exp {
    sem_exp_op(D16_OP_SHL);
    printf("mul end\n");
  } |
  exp tDIV exp {
    sem_exp_op(D16_OP_SHR);
    printf("div end\n");
  } |
  exp tEQU exp {
    sem_exp_op(D16_OP_EQU);
    printf("equ end\n");
  } |
  exp tLTE exp {
    sem_exp_op(D16_OP_LTE);
    printf("gte end\n");
  } |
  exp tGTE exp {
    sem_exp_op(D16_OP_GTE);
    printf("lte end\n");
  } |
  exp tLT exp {
    sem_exp_op(D16_OP_LT);
    printf("gt end\n");
  } |
  exp tGT exp {
    sem_exp_op(D16_OP_GT);
    printf("lt end\n");
  } |
  exp tOR exp {
    sem_exp_op(D16_OP_OR);
    printf("or end\n");
  } |
  exp tAND exp {
    sem_exp_op(D16_OP_AND);
    printf("and end\n");
  } |
  tAND tID {
    sem_exp_and_id($2);
  } |
  tID {
    sem_exp_id($1);
  } |
  tINTEGER {
    sem_exp_integer($1);
  } |
  tCHAR {
    sem_exp_integer($1);
  } |
  funcall {
    printf("funcall end\n");
  } |
  tID tLBRACKET exp tRBRACKET {
    sem_exp_bracket($1);
  }

funcall : tID tLPAR c_params tRPAR {
  printf("funcall c_params end\n");
  sem_funcall($1);
}

c_params : exp tCOMMA c_params | exp

%%
