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
#include <string.h>

#include "d16.h"

#define D16_SIZE_BASE 0x10

/**
 * Adds to a dyn table
 * idx : current idx,
 * size : size of one element,
 * nmemb : total size in number of elements
 */
int d16_table_inc(struct d16_table *t) {
  if (t->nmemb == 0) {
    t->nmemb = D16_SIZE_BASE;
    // printf("Allocating %lu bytes\n", size * *nmemb);
    t->t = malloc(t->size * t->nmemb);
    if (t == NULL) {
      perror("Error allocating memory");
      return 1;
    }
  } else if (t->idx >= t->nmemb) {
    t->nmemb *= 2;
    // printf("Reallocating %lu bytes\n", size * *nmemb);
    t->t = realloc(t->t, t->size * t->nmemb);
    if (t->t == NULL) {
      perror("Error allocating memory");
      return 1;
    }
  }
  return 0;
}

void d16_table_init(struct d16_table *t, size_t size) {
  t->nmemb = 0;
  t->idx = 0;
  t->size = size;
  t->t = NULL;

  // Allocate for the first time
  d16_table_inc(t);
}

int d16_disassemble(FILE *in, struct d16_table *inst) {
  uint32_t r;
  struct ins i;

  // Lets go baby
  while (1) {
    memset(&i, 0, sizeof(struct ins));
    r = fread(&i, INS_SIZE, 1, in);
    if (r <= 0) {
      // This is the end, the correct end YOLORD
      break;
    }
    d16_assert_opcode(&i);
    // Copying the instruction
    table_get(inst, struct ins) = i;
    // Increment idx
    inst->idx++;
    d16_table_inc(inst);
  }

  return 0;
}

void d16_print_program(struct ins *inst, size_t inst_len, struct lbl *lbls,
    size_t lbls_len, int mode) {
  int i, a;
  if (mode == 1) {
    printf("Addr    OP  A  B  C  Instruction\n");
    printf("================================\n");
  }
  a = 0;
  for (i = 0; i < inst_len; i++) {
    if (inst != NULL) {
      int j;
      for (j = 0; j < lbls_len; j++) {
        if ((i << 2) == lbls[j].a) {
          if (mode == 1) {
            fprintf(stdout, "                     ");
          }
          fprintf(stdout, "%s:\n", lbls[j].s);
        }
      }
    }
    if (mode == 1) {
      fprintf(stdout, "0x%04x: ", a * 4);
      fprintf(stdout, "%02x %02x %02x %02x: ",
          inst[i].opcode, inst[i].op0, inst[i].op1, inst[i].op2);
    }
    fprintf(stdout, "  "); d16_ifprintf(&inst[i], stdout);
    a++;
  }
}

void d16_ifprintf(struct ins *i, FILE *out) {
  uint16_t imm;
  fprintf(out, "%s ", mnemonics[i->opcode]);
  if (i->opcode != D16_OP_NOP) {
    if (i->opcode == D16_OP_JMP || i->opcode == D16_OP_JMZ ) {
      if (i->sym != NULL) {
        fprintf(out, "*%s", i->sym);
      } else {
        imm = (i->op0 << 8) | i->op1;
        fprintf(out, "0x%x", imm);
      }
      if (i->opcode == D16_OP_JMZ) {
        fprintf(out, ", ");
        d16_regfprintf(i->op2, out, 1);
      }
    } else if (i->opcode == D16_OP_JMR ) {
      d16_regfprintf(i->op0, out, 1);
    } else if (i->opcode == D16_OP_STR) {
      imm = (i->op0 << 8) | i->op1;
      fprintf(out, "0x%x, ", imm);
      d16_regfprintf(i->op2, out, 1);
    } else if (i->opcode == D16_OP_STP) {
      fprintf(out, "%hhd(", i->op0);
      d16_regfprintf(i->op1, out, 1);
      fprintf(out, "), ");
      d16_regfprintf(i->op2, out, 1);
    } else if (i->opcode == D16_OP_LOP) {
      d16_regfprintf(i->op0, out, 0);
      fprintf(out, "%hhd(", i->op1);
      d16_regfprintf(i->op2, out, 1);
      fprintf(out, ")");
    } else {
      // Reg 0
      d16_regfprintf(i->op0, out, 0);
      if (i->opcode == D16_OP_AFC) {
        if (i->sym == NULL) {
          imm = (i->op1 << 8) | i->op2;
          fprintf(out, "$0x%x", imm);
        } else {
          fprintf(out, "$%s", i->sym);
        }
      } else if (i->opcode == D16_OP_COP) {
        d16_regfprintf(i->op1, out, 1);
      } else if (i->opcode == D16_OP_LOD) {
        imm = (i->op1 << 8) | i->op2;
        fprintf(out, "0x%x", imm);
      } else {
        // Reg 1 et 2
        d16_regfprintf(i->op1, out, 0);
        d16_regfprintf(i->op2, out, 1);
      }
    }
  }
  fprintf(out, "\n");
}

void d16_regfprintf(uint8_t r, FILE *out, int last) {
  fprintf(out, "r%d", r);
  if (!last) {
    fprintf(out, ", ");
  }
}

/**
 * Semantics
 */

void d16_assert_index(unsigned char i) {
  assertf(i < 0xff, "Bad immediate size 0x%08x", i);
}

void d16_assert_immediate(unsigned int imm) {
  assertf(imm < 0x10000, "Bad immediate size 0x%08x", imm);
}

void d16_assert_address(unsigned int addr) {
  assertf(addr < 0x10000, "Bad address size 0x%08x", addr);
}

void d16_assert_opcode(struct ins *i) {
  assertf(strcmp(mnemonics[i->opcode], "") != 0, "Bad opcode 0x%02x",
      i->opcode);
}

char *mnemonics[0x100] = {
  // 0x00
  "nop", "add", "sub", "shl", "shr", "or", "and", "equ", "lte", "gte", "lt",
      "gt", "", "", "", "",
  // 0x10
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x20
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x30
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x40
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x50
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x60
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x70
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x80
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0x90
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0xa0
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0xb0
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0xc0
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0xd0
  "afc", "cop", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0xe0
  "lod", "str", "lop", "stp", "", "", "", "", "", "", "", "", "", "", "", "",
  // 0xf0
  "jmp", "jmz", "jmr", "", "", "", "", "", "", "", "", "", "", "", "", "",
};
