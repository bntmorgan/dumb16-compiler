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
#include "sem.h"
#include "sym.h"
#include "opt.h"
#include "parser.h"

extern int line;

void yyerror(const char *s) {
  fprintf(stderr, "Vous ne maîtrisez pas les concepts : %s at line %d\n", s,
      line);
}

int main(int argc, char *argv[]) {
  do_options(argc, argv);
  sym_init();
  sem_init();
  yyparse();
  // sem_second_pass();
  close_files();
  sym_free();
  sem_free();
  return 0;
}
