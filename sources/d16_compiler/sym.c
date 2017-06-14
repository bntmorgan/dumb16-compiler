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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "d16.h"
#include "sym.h"

char *types[] = {
  "void",
  "int",
};

unsigned int depth = 0;
unsigned int gaddress = 0;
unsigned int paddress = 0;
unsigned int laddress = 0;
unsigned int taddress = 0;
struct d16_table syms;
struct d16_table funs;
struct d16_table lbls;

int sym_init(void) {
  d16_table_init(&syms, sizeof(struct sym));
  d16_table_init(&funs, sizeof(struct fun));
  d16_table_init(&lbls, sizeof(struct lbl));
  d16_table_inc(&syms);
  gaddress = 0;
  sym_reset_local();
  return 0;
}

int sym_free(void) {
  // XXX
  // d16_table_free(&syms);
  return 0;
}

int sym_add(char *s, char t, char p) {
  struct sym sym = {
    .d = depth,
    .s = s,
    .t = t
  };
  // Parameter
  if (p) {
    sym.a = paddress;
    paddress -= 2;
  // Local or global
  } else {
    if (depth == 0) {
      sym.a = gaddress;
      gaddress -= 2;
    } else {
      sym.a = laddress;
      laddress += 2;
      taddress = laddress; // Temp are starting after regular variables
    }
  }
  table_get(&syms, struct sym) = sym;
  syms.idx++;
  d16_table_inc(&syms);
  return 0;
}

int sym_get(char *s, struct sym **sym) {
  int i;
  for (i = syms.idx - 1; i >= 0; i--) {
    if (strcmp(s, table_get_idx(&syms, struct sym, i).s) == 0) {
      *sym = &table_get_idx(&syms, struct sym, i);
      return 0;
    }
  }
  return 1;
}

int sym_depth_inc(void) {
  depth++;
  return 0;
}

int sym_depth_dec(void) {
  if (depth == 0 || syms.idx == 0) {
    return 1;
  }
  while (syms.idx > 0 && table_get_idx(&syms, struct sym, syms.idx - 1).d ==
      depth) {
    syms.idx--;
  }
  depth--;
  return 0;
}

int sym_reset_params(void) {
  if (depth != 0 && syms.idx == 0) {
    return 1;
  }
  while (syms.idx > 0 && table_get_idx(&syms, struct sym, syms.idx - 1).global
      != 1) {
    syms.idx--;
  }
  paddress = -4;
  return 0;
}

void sym_reset_local(void) {
  // BP + 0 : return address
  // BP - 2 : BP save
  laddress = 2;
  paddress = -4;
  taddress = laddress;
}

void sym_print(struct sym *s) {
  printf("s(%s), t(%s), d(%d), a(0x%04hx, %d)\n", s->s, types[s->t], s->d, s->a,
      s->a);
}

void sym_print_all(struct sym *s) {
  int i;
  printf("syms: depth(%d), laddress(0x%02hx), gaddress(0x%02hx) {\n", depth,
      laddress, gaddress);
  for (i = 0; i < syms.idx; i++) {
    sym_print(&s[i]);
  }
  printf("}\n");
}

void sym_temp_add(void) {
  taddress += 2;
  printf("add temp(0x%04hx)\n", sym_temp_get());
}

int sym_temp_get(void) {
  return taddress - 2;
}

void sym_temp_dec(void) {
  taddress -= 2;
  printf("dec temp(0x%04hx)\n", sym_temp_get());
}

void sym_temp_del(unsigned int n) {
  taddress -= 2 * n;
  printf("del temp(0x%04hx)\n", sym_temp_get());
}

int fun_add(char *s, unsigned int n, int16_t a) {
  struct fun f = {
    .s = s,
    .n = n,
    .a = a
  };
  table_get(&funs, struct fun) = f;
  funs.idx++;
  d16_table_inc(&funs);
  return 0;
}

int fun_get(char *s, struct fun **f) {
  int i;
  for (i = funs.idx - 1; i >= 0; i--) {
    if (strcmp(s, table_get_idx(&funs, struct fun, i).s) == 0) {
      *f = &table_get_idx(&funs, struct fun, i);
      return 0;
    }
  }
  return 1;
}

void fun_print(struct fun *f) {
  printf("s(%s), n(%u), a(0x%04hx)\n", f->s, f->n, f->a);
}

void fun_print_all(struct fun *f) {
  int i;
  printf("funs { \n");
  for (i = 0; i < funs.idx; i++) {
    fun_print(&table_get_idx(&funs, struct fun, i));
  }
  printf("}\n");
}

int lbl_add(char *s, int16_t a) {
  struct lbl l = {
    .s = s,
    .a = a
  };
  table_get(&lbls, struct lbl) = l;
  lbls.idx++;
  d16_table_inc(&lbls);
  // XXX
  printf("> %s:\n", s);
  return 0;
}
