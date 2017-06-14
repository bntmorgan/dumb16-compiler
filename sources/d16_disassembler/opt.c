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
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include "opt.h"

// Sauvegarde de l'entrée standard
int stdin_fd = 0;

// Flag mode verbose
int mode_verbose = 0;

// Entrée standard bufferisée
FILE *infile = NULL;

void do_options(int argc, char **argv) {
  int c;

  while ((c = getopt (argc, argv, "v")) != -1) {
    switch (c) {
    case 'v':
      mode_verbose = 1;
      break;
    case '?':
      if (optopt == 'o') {
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      } else if (isprint (optopt)) {
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
      }
      exit(1);
    default:
      abort ();
    }
  }

  // Gestion du fichier d'entrée à compiler
  if (optind < argc) {
    infile = fopen(argv[optind], "r");
    if (!infile) {
      fprintf(stderr, "Unable to open the file %s\n", argv[optind]);
    }
  } else {
    fprintf(stderr, "Cannot compile no file\n");
    exit(1);
  }
}

void close_files() {
  fclose(infile);
}
