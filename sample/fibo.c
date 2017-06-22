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

int screen(int a) {
  int white = 0x0f00, blink = 0x9f00;
  int i = 0x4000;
  int dash = '-';
  while (i < 0x4fa0) {
    // draw character
    if (i == 0x4100) {
      *i = blink | dash;
    } else {
      if (i == 0x4200) {
        *i = blink | dash;
      }
      else {
        *i = white | dash;
      }
    }
    i = i + 2;
  }
  // Draw message
  i = 0x4710;
  *i = 'P' | blink; i = i + 2;
  *i = 'r' | blink; i = i + 2;
  *i = 'o' | blink; i = i + 2;
  *i = 'j' | blink; i = i + 2;
  *i = 'e' | blink; i = i + 2;
  *i = 't' | blink; i = i + 2;
  *i = ' ' | blink; i = i + 2;
  *i = 'S' | blink; i = i + 2;
  *i = 'y' | blink; i = i + 2;
  *i = 's' | blink; i = i + 2;
  *i = 't' | blink; i = i + 2;
  *i = 'e' | blink; i = i + 2;
  *i = 'm' | blink; i = i + 2;
  *i = 'e' | blink; i = i + 2;
  *i = ' ' | blink; i = i + 2;
  *i = 'I' | blink; i = i + 2;
  *i = 'n' | blink; i = i + 2;
  *i = 'f' | blink; i = i + 2;
  *i = 'o' | blink; i = i + 2;
  *i = ' ' | blink; i = i + 2;
  *i = '2' | blink; i = i + 2;
  *i = '0' | blink; i = i + 2;
  *i = '1' | blink; i = i + 2;
  *i = '8' | blink; i = i + 2;
  *i = ' ' | blink; i = i + 2;
  *i = ':' | blink; i = i + 2;
  *i = ')' | blink; i = i + 2;
  return 0;
}

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
  int a = screen(1);
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
