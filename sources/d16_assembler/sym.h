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

#ifndef __SYM_H__
#define __SYM_H__

#include <stdint.h>

#include "d16.h"

typedef struct _t_sym {
  char *name;
  uint16_t addr;
} t_sym;

typedef struct d16_table t_sym_table;

t_sym *sym_get(t_sym_table *t, char *name);
int sym_get_idx(t_sym_table *t, char *name);
int sym_add(t_sym_table *t, char *name);
void sym_init(t_sym_table *t);
void sym_print(t_sym_table *t);

#endif//_SYM_H__
