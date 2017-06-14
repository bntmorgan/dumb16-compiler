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

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "sem.h"
#include "sym.h"
#include "parser.h"
#include "opt.h"

int csize;
t_sym_table t;
struct d16_table ta;
uint16_t caddr;

struct temp_addr {
  char *name; // Name of the corresponding symbol
  int off; // Offset in the file
  int size; // Size in bytes
};

void sem_init() {
  caddr = 0;
  d16_table_init(&ta, sizeof(struct temp_addr));
}

void sem_temp_addr_add(char *name, int off, int size) {
  struct temp_addr tmp = {
    name,
    off,
    size
  };
  table_get(&ta, struct temp_addr) = tmp;
  ta.idx++;
  d16_table_inc(&ta);
}

/**
 * Parser Actions
 */

void sem_nop(struct ins *i) {
  compile(i);
}

void sem_add(struct ins *i) {
  compile(i);
}

void sem_sub(struct ins *i) {
  compile(i);
}

void sem_shl(struct ins *i) {
  compile(i);
}

void sem_shr(struct ins *i) {
  compile(i);
}

void sem_equ(struct ins *i) {
  compile(i);
}

void sem_afc(struct ins *i) {
  compile(i);
}

void sem_cop(struct ins *i) {
  compile(i);
}

void sem_lod(struct ins *i) {
  compile(i);
}

void sem_lop(struct ins *i) {
  compile(i);
}

void sem_str(struct ins *i) {
  compile(i);
}

void sem_stp(struct ins *i) {
  compile(i);
}

void sem_jmp(struct ins *i) {
  compile(i);
}

void sem_jmz(struct ins *i) {
  compile(i);
}


void sem_label(char *l) {
  if (sym_add(&t, l)) {
    log_error("Symbol %s already declared\n", l);
  }
  table_get_idx(&t, t_sym, t.idx - 1).addr = caddr;
}

/**
 * Compilation
 */
void compile(struct ins *i) {
  if (fwrite(i, INS_SIZE, 1, file_out) <= 0) {
    log_error("Failed to compile");
    error();
  }
  caddr += INS_SIZE;
  if (i->sym != NULL) {
    if (i->opcode == D16_OP_AFC) {
      sem_temp_addr_add(i->sym, caddr - 4, 2);
    } else {
      sem_temp_addr_add(i->sym, caddr - 3, 2);
    }
  }
}

int pad(void) {
  uint32_t zero = 0;
  int padsize = caddr % 4;
  printf("PADDING %x\n", padsize);
  fwrite(&zero, sizeof(char), padsize, file_out);
  return padsize;
}

void sem_second_pass() {
  int i;
  sym_print(&t);
  printf("Second pass :\n");
  for (i = 0; i < ta.idx; i++) {
    struct temp_addr tmpa = table_get_idx(&ta, struct temp_addr, i);
    t_sym *sym = sym_get(&t, tmpa.name);
    printf("name %s, off %08x, size %02x\n", tmpa.name, tmpa.off, tmpa.size);
    if (sym == NULL) {
      log_error("Undeclared symbol %s\n", tmpa.name);
    }
    fseek(file_out, tmpa.off, SEEK_SET);
    fwrite(&sym->addr, tmpa.size, 1, file_out);
  }
}

/**
 * Error handling
 */
void error(void) {
  exit(1);
}
