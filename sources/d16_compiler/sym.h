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

enum sym_type {
  TYPE_VOID,
  TYPE_INT
};

extern struct d16_table syms;
extern struct d16_table funs;
extern struct d16_table lbls;

struct sym {
  char *s; // Name
  enum sym_type t; // Type
  unsigned int d; // Depth
  int16_t a; // Address
  unsigned char init;
  unsigned char global;
  unsigned char cons;
};

struct fun {
  char *s; // Name
  uint16_t a; // Address
  unsigned int n; // Number of parameters
  char d; // Defined or not
};

int sym_init(void);
int sym_free(void);
int sym_add(char *s, char t, char p);
int sym_get(char *s, struct sym **sym);
int sym_depth_inc(void);
int sym_depth_dec(void);
int sym_reset_params(void);
void sym_reset_local(void);
void sym_print_all(struct sym *s);

void sym_temp_add(void);
int sym_temp_get(void);
void sym_temp_dec(void);
void sym_temp_del(unsigned int n);

int fun_add(char *s, unsigned int n, int16_t a);
int fun_get(char *s, struct fun **f);
void fun_print_all(struct fun *f);

int lbl_add(char *s, int16_t a);

#endif//_SYM_H__
