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

#ifndef __SEM_H__
#define __SEM_H__

#include "d16.h"

int sem_init(void);
int sem_free(void);
void compile_op_label(uint8_t opcode, uint8_t op0, uint8_t op1, uint8_t op2,
    char *sym);
void compile_label(uint8_t opcode, char *sym);
void compile(uint8_t opcode, uint8_t op0, uint8_t op1, uint8_t op2);

// Semantics
void sem_exp_op(int op);
void sem_exp_id(char *id);
void sem_exp_integer(int v);
void sem_afc(char *id);
void sem_afc_pointer(char *id);
void sem_body_begin(char *f, unsigned int n);
void sem_body_end(void);
void sem_proto_end(char *f, unsigned int n);
void sem_funcall(char *f);
void sem_return(void);
void sem_if_begin(void);
void sem_if_cond(void);
void sem_end_then(void);
void sem_else(void);
void sem_if_end(void);
void sem_while_begin(void);
void sem_while_cond(void);
void sem_while_end(void);

#endif//__SEM_H__
