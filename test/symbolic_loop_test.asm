BITS 32

global start

section .text

start:
  rdtsc
  push eax
  call fn_loop

  ret


fn_loop:

  mov eax, [esp + 4] ; first parm

_0:
  test eax, 1
  jz _2

_1:
  inc eax
  jmp _3

_2:
  dec eax

_3:
  cmp eax, 10
  jb _0

_4:
  ret
