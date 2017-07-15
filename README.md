# Dumb compiler for Dumb SoC

Dumb compiler is a C compiler, assembler and disassembler for Dumb SoC.

## C and assembly langages

You can build this kind of c files:

```
int fibo(int n);

int main(void) {
  int n = fibo(4);
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
```

You can assemble this kind of assembly file :

```
rom:
  afc r0, '-'
  afc r1, '-'
  afc r2, $loop_init
  afc r7, $0x0f00 // white style
  afc r8, $0x9f00 // blink style
loop_init:
  add r0, r0, r7
  add r1, r1, r8
  afc r12, $0x4fa0
  afc r11, $0x4000 // video memory
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
```

## Workflow

The worklow is as follows:

```
        ------------          -------------
C in -> | Compiler | -> As -> | Assembler | -> Bin out
        ------------          -------------
```

Have fun !
