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

#include "sym.h"
#include "d16.h"

void sym_init(t_sym_table *t) {
  d16_table_init(t, sizeof(t_sym));
}

t_sym *sym_get(t_sym_table *t, char *name) {
  int i;
  for (i = 0; i < t->idx; i++) {
    if (strcmp(table_get_idx(t, t_sym, i).name, name) == 0) {
      return &table_get_idx(t, t_sym, i);
    }
  }
  return NULL;
}

int sym_get_idx(t_sym_table *t, char *name) {
  int i;
  for (i = 0; i < t->idx; i++) {
    if (strcmp(table_get_idx(t, t_sym, i).name, name) == 0) {
      return i;
    }
  }
  return -1;
}

int sym_add(t_sym_table *t, char *name) {
  t_sym s = {
    .name = name,
    .addr = 0
  };
  if (sym_get(t, name) != NULL) {
    return -1;
  }
  table_get(t, t_sym) = s;
  t->idx++;
  d16_table_inc(t);
  return 0;
}

void sym_print(t_sym_table *t) {
  int i;
  printf("Symbol table :\n");
  for (i = 0; i < t->idx ; i++) {
    printf("%s 0x%04x\n", table_get_idx(t, t_sym, i).name, table_get_idx(t,
          t_sym, i).addr);
  }
}
