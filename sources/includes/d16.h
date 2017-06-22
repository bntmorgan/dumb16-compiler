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

#ifndef __D16_H__
#define __D16_H__

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) {fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n" \
    , __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); error();}
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__);}

extern void error(void);

enum D16_OP_CODE {
  D16_OP_NOP = 0x00,
  D16_OP_ADD = 0x01,
  D16_OP_SUB = 0x02,
  D16_OP_SHL = 0x03,
  D16_OP_SHR = 0x04,
  D16_OP_OR  = 0x05,
  D16_OP_AND = 0x06,
  D16_OP_EQU = 0x07,
  D16_OP_LTE = 0x08,
  D16_OP_GTE = 0x09,
  D16_OP_LT  = 0x0a,
  D16_OP_GT  = 0x0b,
  D16_OP_AFC = 0xd0,
  D16_OP_COP = 0xd1,
  D16_OP_LOD = 0xe0,
  D16_OP_STR = 0xe1,
  D16_OP_LOP = 0xe2,
  D16_OP_STP = 0xe3,
  D16_OP_JMP = 0xf0,
  D16_OP_JMZ = 0xf1,
  D16_OP_JMR = 0xf2,
};

#define INS_SIZE 4

struct d16_table {
  size_t nmemb; // Total number of elements of size size allocated
  size_t idx; // current used size in nmemb
  size_t size; // Size of one element
  void *t; // table pointer
};

#define table_get(table, type) ((type *)((table)->t))[(table)->idx]
#define table_get_idx(table, type, idx) ((type *)((table)->t))[idx]

struct ins {
  uint8_t op2;
  uint8_t op1;
  uint8_t op0;
  uint8_t opcode;
  // Metadata
  char *sym;
} __attribute__((packed));

struct lbl {
  char *s; // Name
  int16_t a; // Address
};

int d16_disassemble(FILE *in, struct d16_table *inst);
void d16_print_program(struct ins *inst, size_t inst_len, struct lbl *lbls,
    size_t lbls_len, int mode);

void d16_assert_reg_size_sel(uint32_t size, uint32_t  reg, uint32_t sel);
void d16_assert_size_int(struct ins *i, int op);
void d16_assert_op2ode(struct ins *i);
void d16_ifprintf(struct ins *i, FILE *out);
void d16_regfprintf(uint8_t r, FILE *out, int last);

int d16_table_inc(struct d16_table *t);
void d16_table_init(struct d16_table *t, size_t size);

int d16_pow(int x, int y);

extern char *mnemonics[0x100];

void d16_assert_opcode(struct ins *i);
void d16_assert_immediate(unsigned int imm);
void d16_assert_address(unsigned int addr);
void d16_assert_index(unsigned char i);
int d16_is_label(struct ins *i);

#endif//__D16_H__
