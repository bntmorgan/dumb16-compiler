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

// output file
FILE* file_out = NULL;

// save stdin if any
int stdin_fd = 0;

// flag verbose mode
int mode_verbose = 0;
FILE *stdin_save = NULL;

void do_options(int argc, char **argv) {
  int c;

  while ((c = getopt (argc, argv, "vo:")) != -1) {
    switch (c) {
    case 'v':
      mode_verbose = 1;
      break;
    case 'o':
      file_out = fopen(optarg, "w");
      if (file_out == NULL) {
        perror("Error while creating output file");
        exit(1);
      }
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

  // no given file, defaults to a.out
  if (file_out == NULL) {
    file_out = fopen("a.out", "w");
    if (file_out == NULL) {
      perror("Error while creating output file");
      exit(1);
    }
  }

  // input file handling
  if (optind < argc) {
    // std input save
    stdin_fd = dup(STDIN_FILENO);
    // std in buffered reopen
    stdin_save = fdopen(stdin_fd, "r");
    if (stdin == NULL) {
      perror("Error while reopenning stdin");
      exit(1);
    }
    // free old stdin fileno
    close(STDIN_FILENO);
    // open input file which takes stdin place
    if (open(argv[optind], O_RDONLY) == -1) {
      perror("Error while openning file to compile");
      exit(1);
    }
  } else {
    fprintf(stderr, "Cannot compile no file\n");
    exit(1);
  }
}

void close_files() {
  close(STDIN_FILENO);
  // get stdin back in place
  dup(stdin_fd);
  close(stdin_fd);
  fclose(file_out);
}
