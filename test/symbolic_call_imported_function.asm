BITS 32

global start
extern _MessageBoxA@16

section .data

szTitle: db "title", 0
szMsg: db "message", 0

section .text

start:
  push 0x10
  push szTitle
  push szMsg
  push 0x0
  call _MessageBoxA@16

  ret
