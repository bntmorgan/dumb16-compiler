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

#include <stdio.h>
#include "d16.h"
#include "opt.h"

int main(int argc, char *argv[]) {
  struct d16_table inst;

  d16_table_init(&inst, sizeof(struct ins));

  do_options(argc, argv);
  d16_disassemble(infile, &inst);
  d16_print_program(inst.t, inst.idx, NULL, 0, 1);
  close_files();
  return 0;
}
