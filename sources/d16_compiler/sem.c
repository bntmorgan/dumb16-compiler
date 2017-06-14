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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sem.h"
#include "opt.h"
#include "sym.h"
#include "parser.h"
#include "d16.h"

// label counter to handle conditional structures
uint16_t lc;

uint16_t pc;
struct d16_table lcs;

void sem_start(void) {
  char l[0x100];
  // Add label start
  sprintf(&l[0], "start");
  // add label name
  lbl_add(strdup(l), pc);
  // init stack base
  compile(D16_OP_AFC, 13, 0, 0);
  // call the main function
  sprintf(&l[0], "main");
  // save return address
  // afc r0 with return adress
  compile_op_label(D16_OP_AFC, 0, 0, 0, l);
  // afc BP with r0
  compile(D16_OP_STP, 0, 13, 0);
  // jmp to main
  compile_label(D16_OP_JMP, strdup(l));
  // Add label end
  sprintf(&l[0], "end");
  // add label name
  lbl_add(strdup(l), pc);
  // infinite loop
  compile_label(D16_OP_JMP, strdup(l));
}

int sem_init(void) {
  d16_table_init(&lcs, sizeof(char));
  lc = -1;
  pc = 0;
  sem_start();
  return 0;
}

int sem_free(void) {
  return 0;
}

void comp(struct ins *i) {
  int j;
  for (j = 0; j < lbls.idx; j++) {
    if (pc == table_get_idx(&lbls, struct lbl, j).a) {
      fprintf(file_out, "%s:\n", table_get_idx(&lbls, struct lbl, j).s);
    }
  }
  printf(">   "); d16_ifprintf(i, stdout);
  fprintf(file_out, "  "); d16_ifprintf(i, file_out);
  pc += 2;
}

void compile(uint8_t opcode, uint8_t op0, uint8_t op1, uint8_t op2) {
  struct ins i = {
    .opcode = opcode,
    .op0 = op0,
    .op1 = op1,
    .op2 = op2,
    .sym = NULL
  };
  comp(&i);
}

void compile_op_label(uint8_t opcode, uint8_t op0, uint8_t op1, uint8_t op2,
    char *sym) {
  struct ins i = {
    .opcode = opcode,
    .op0 = op0,
    .op1 = op1,
    .op2 = op2,
    .sym = sym
  };
  comp(&i);
}

void compile_label(uint8_t opcode, char *sym) {
  struct ins i = {
    .opcode = opcode,
    .op0 = 0,
    .op1 = 0,
    .op2 = 0,
    .sym = sym
  };
  comp(&i);
}

void sem_exp_op(int op) {
  // Load temp var left
  compile(D16_OP_LOP, 0, (sym_temp_get() - 2) & 0xff, 13);
  // Load temp var right
  compile(D16_OP_LOP, 1, sym_temp_get() & 0xff, 13);
  // Execute op
  compile(op, 2, 0, 1);
  // Remove unused right temp var
  sym_temp_dec();
  // Store the result in left temp var
  // add
  compile(D16_OP_STP, sym_temp_get() & 0xff, 13, 2);
}

void sem_exp_id(char *id) {
  // Get the symbol to copy in temp var
  struct sym *s;
  if (sym_get(id, &s)) {
    fprintf(stderr, "%s undeclared\n", id);
    exit(1);
  }
  // Create new temp var
  sym_temp_add();
  // Load local var
  compile(D16_OP_LOP, 0, s->a & 0xff, 13);
  // Store it in temp var
  compile(D16_OP_STP, sym_temp_get() & 0xff, 13, 0);
}

void sem_exp_integer(int v) {
  // Create new temp var
  sym_temp_add();
  // AFC temp var
  compile(D16_OP_AFC, 0, (v >> 8) & 0xff, v & 0xff);
  // Store it in temp var
  compile(D16_OP_STP, sym_temp_get() & 0xff, 13, 0);
}

void sem_afc_pointer(char *id) {
  // Get the symbol to copy in temp var
  struct sym *s;
  if (sym_get(id, &s)) {
    fprintf(stderr, "%s undeclared\n", id);
    exit(1);
  }
  // load pointer address
  compile(D16_OP_LOP, 1, s->a & 0xff, 13);
  // load pointer
  compile(D16_OP_LOP, 0, sym_temp_get() & 0xff, 13);
  // remove last temp var from exp
  sym_temp_dec();
  // store it in temp var
  compile(D16_OP_STP, 0, 1, 0);
}

void sem_afc(char *id) {
  // get the symbol to copy in temp var
  struct sym *s;
  if (sym_get(id, &s)) {
    fprintf(stderr, "%s undeclared\n", id);
    exit(1);
  }
  // load local var
  compile(D16_OP_LOP, 0, sym_temp_get() & 0xff, 13);
  // remove last temp var from exp
  sym_temp_dec();
  // store it in temp var
  compile(D16_OP_STP, s->a & 0xff, 13, 0);
}

void sem_body_begin(char *f, unsigned int n) {
  // define function
  struct fun *fs;
  if (fun_get(f, &fs) == 0) {
    if (fs->d) {
      fprintf(stderr, "redeclaring (%s)\n", f);
      exit(1);
    }
  } else {
    fun_add(f, n, pc);
    fun_print_all((struct fun*)funs.t);
    fs = &table_get_idx(&funs, struct fun, funs.idx - 1);
    fs->d = 1;
    // TODO handle function address here
  }
  // Compile label
  lbl_add(f, pc);
}

void sem_proto_end(char *f, unsigned int n) {
  // declare function
  struct fun *fs;
  if (fun_get(f, &fs) == 0) {
    fprintf(stderr, "redeclaring (%s)\n", f);
    exit(1);
  } else {
    fun_add(f, n, 0);
    fun_print_all((struct fun*)funs.t);
  }
  // params
  sym_print_all((struct sym*)syms.t);
  sym_reset_params();
}

void sem_body_end(void) {
  // params
  sym_reset_local();
  sym_reset_params();
  // read return address
  compile(D16_OP_LOP, 14, 0, 13);
  // jump back to caller
  compile(D16_OP_JMR, 14, 0, 0);
}

// TODO remove arguments temps and add a temp for return value
void sem_funcall(char *f) {
  // first get the function symbol
  struct fun *fs;
  if (fun_get(f, &fs) != 0) {
    fprintf(stderr, "error: function %s undeclared\n", f);
    exit(1);
  }
  // save base pointer
  compile(D16_OP_STP, (sym_temp_get() + 2) & 0xff, 13, 13);
  // increment base pointer
  // afc r0 avec temp address + 4 (BP save + return @ save)
  compile(D16_OP_AFC, 0, 0, (sym_temp_get() + 4) & 0xff);
  // add r14 avec R0
  compile(D16_OP_ADD, 13, 13, 0);
  // save return address
  // afc r0 with return adress
  compile_op_label(D16_OP_AFC, 0, 0, 0, f);
  // afc BP with r0
  compile(D16_OP_STP, 0, 13, 0);
  // call the function
//  compile(D16_OP_JMP, (fs->a >> 8) & 0xff, fs->a & 0xff, 0);
  compile_label(D16_OP_JMP, fs->s);
  // restore base pointer (@ BP - 2)
  compile(D16_OP_LOP, 13, -2, 13);
  // lop r13, -2(r13)
  // remove the temp arguments
  sym_temp_del(fs->n);
  // copy the returned value in R0 to a temp var
  sym_temp_add();
  compile(D16_OP_STP, sym_temp_get() & 0xff, 13, 0);
}

void sem_return(void) {
  compile(D16_OP_LOP, 0, sym_temp_get() & 0xff, 13);
  sym_temp_dec();
}

uint16_t get_last_label(void) {
  int i;
  for (i = lcs.idx - 1; i >= 0; i--) {
    if (table_get_idx(&lcs, char, i) == 0) {
      return i;
    }
  }
  fprintf(stderr, "Error no current conditionnal structure label available\n");
  exit(1);
}

void sem_while_begin(void) {
  char l[0x100];
  lc++;
  table_get(&lcs, char) = 0;
  lcs.idx++;
  d16_table_inc(&lcs);
  // create label name
  sprintf(&l[0], "while_%u", lc);
  // add label name
  lbl_add(strdup(l), pc);
}

void sem_while_cond(void) {
  char l[0x100];
  // get the temp exp cond
  compile(D16_OP_LOP, 0, sym_temp_get() & 0xff, 13);
  // create else label
  sprintf(l, "end_while_%u", get_last_label());
  // jmp if false
  compile_op_label(D16_OP_JMZ, 0, 0, 0, strdup(l));
  // remove condition temp var
  sym_temp_dec();
  // create label name
  sprintf(&l[0], "then_%u", get_last_label());
  // add label name
  lbl_add(strdup(l), pc);
}

void sem_while_end(void) {
  char l[0x100];
  // create label name
  sprintf(&l[0], "while_%u", get_last_label());
  // jump back to condition
  compile_label(D16_OP_JMP, strdup(l));
  // create label name
  sprintf(&l[0], "end_while_%u", get_last_label());
  // add label name
  lbl_add(strdup(l), pc);
  // confirm end of treatment for this conditional label
  printf("end while for lc(%d)\n", get_last_label());
  table_get_idx(&lcs, char, get_last_label()) = 1;
}

void sem_if_begin(void) {
  char l[0x100];
  lc++;
  table_get(&lcs, char) = 0;
  lcs.idx++;
  d16_table_inc(&lcs);
  // create label name
  sprintf(&l[0], "if_%u", lc);
  // add label name
  lbl_add(strdup(l), pc);
}

void sem_if_cond(void) {
  char l[0x100];
  // get the temp exp cond
  compile(D16_OP_LOP, 0, sym_temp_get() & 0xff, 13);
  // create else label
  sprintf(l, "end_then_%u", get_last_label());
  // jmp if false
  compile_op_label(D16_OP_JMZ, 0, 0, 0, strdup(l));
  // remove condition temp var
  sym_temp_dec();
  // create label name
  sprintf(&l[0], "then_%u", get_last_label());
  // add label name
  lbl_add(strdup(l), pc);
}

void sem_end_then(void) {
  char l[0x100];
  // create label name
  sprintf(&l[0], "end_then_%u", get_last_label());
  // add label name
  lbl_add(strdup(l), pc);
}

void sem_else(void) {
  char l[0x100];
  // create label name
  sprintf(&l[0], "end_if_%u", get_last_label());
  // jump over the else
  compile_label(D16_OP_JMP, strdup(l));
  // add end then label
  sem_end_then();
  // create label name
  sprintf(&l[0], "else_%u", get_last_label());
  // add label name
  lbl_add(strdup(l), pc);
}

void sem_if_end(void) {
  char l[0x100];
  // create label name
  sprintf(&l[0], "end_if_%u", get_last_label());
  // add label name
  lbl_add(strdup(l), pc);
  // confirm end of treatment for this conditional label
  printf("end if for lc(%d)\n", get_last_label());
  table_get_idx(&lcs, char, get_last_label()) = 1;
}
