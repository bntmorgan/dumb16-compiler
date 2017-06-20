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

  afc r0, '-'
  afc r1, '-'
  afc r2, $loop_init
  afc r7, $0x0f00 // white style
  afc r8, $0x9f00 // blink style
loop_init:
  add r0, r0, r7
  add r1, r1, r8
  afc r12, $0x4fa0
  afc r11, $0x4000
  afc r13, $0x2
  afc r14, $0x4100
  afc r15, $0x4120
loop_start:
  sub r10, r12, r11
  jmz *loop_end, r10
if:
  sub r10, r14, r11
  jmz *else, r10
then:
if2:
  sub r10, r15, r11
  jmz *else2, r10
then2:
  stp (r11), r0 // Write 'a' to the vga memory
  jmp *end_if2
else2:
  stp (r11), r1 // Write blink 'a' to the vga memory
end_if2:
  jmp *end_if
else:
  stp (r11), r1 // Write blink 'a' to the vga memory
end_if:
  add r11, r11, r13
  jmp *loop_start
loop_end:
  afc r11, $0x4710
  afc r2, 'P'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'r'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'o'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'j'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'e'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 't'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ' '
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'S'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'y'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 's'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 't'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'e'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'm'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'e'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ' '
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'I'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'n'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'f'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, 'o'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ' '
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, '2'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, '0'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, '1'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, '7'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ' '
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, '!'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, '!'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ' '
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ':'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
  afc r2, ')'
  add r2, r2, r8
  stp (r11), r2
  add r11, r11, r13
inifinite_loop:
  jmp *inifinite_loop
