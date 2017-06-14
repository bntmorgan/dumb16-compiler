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

int fibo(int n);

int lordum(int a, int b, int c) {
  int c = 'a';
  int d = 0x1000;
  *d = c;
  while (1) {
    c = c + 1;
    while (2) {
      int e;
      e = c + 1;
    }
  }
  return a + b + c;
}

int toto(int b) {
  int e = b;
  return b;
}

int main(void) {
  int n = toto(4);
  return 0;
}

int fibo(int n) {
  if (n == 0) {
    if (n == 1) {
      return 1;
    }
    return 0;
  }
  if (n == 1) {
    return 1;
  } else {
    return fibo(n - 1) + fibo(n - 2);
  }
}
