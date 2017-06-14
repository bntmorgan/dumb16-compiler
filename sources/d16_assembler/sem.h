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

extern int csize;

void sem_init();
void sem_nop(struct ins *i);
void sem_add(struct ins *i);
void sem_sub(struct ins *i);
void sem_shl(struct ins *i);
void sem_shr(struct ins *i);
void sem_equ(struct ins *i);
void sem_afc(struct ins *i);
void sem_cop(struct ins *i);
void sem_lod(struct ins *i);
void sem_lop(struct ins *i);
void sem_str(struct ins *i);
void sem_stp(struct ins *i);
void sem_jmp(struct ins *i);
void sem_jmz(struct ins *i);
void sem_label(char *l);

void sem_error(void);

void compile(struct ins *i);
int pad(void);
void sem_second_pass();

#endif//__SEM_H__
