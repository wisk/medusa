#!/usr/bin/env python2

from pydusa import *
import sys

def stdcall_ret(cpu, mem, param_no):
  ret_addr = mem.read_u32(cpu.esp)
  cpu.eip = ret_addr
  # cpu.esp += (4 + param_no * 4)

def on_symbol(cpu, mem, ad):
  global emul
  print 'function %s called' % emul.get_hook_name()

def on_ExitProcess(cpu, mem, ad):
  ExitCode = cpu.rcx
  print '[!] kernel32.dll!ExitProcess(%08x)' % ExitCode
  cpu.rip = 0xdeaddead

def on_MessageBoxA(cpu, mem, ad):
  hWnd = cpu.rcx
  lpCaption = cpu.rdx
  lpText = cpu.r8
  MsgType = cpu.r9

  caption = mem.read_utf8(lpCaption)
  text = mem.read_utf8(lpText)

  if not caption:
    caption = '%016x' % lpCaption
  else:
    caption = '"%s"' % caption
  if not text:
    text = '%016x' % lpText
  else:
    text = '"%s"' % text

  cpu.eax = 0

  print '[!] user32.dll!MessageBoxA(%08x, %s, %s %08x) -> %08x' % (hWnd, caption, text, MsgType, cpu.eax)

  stdcall_ret(cpu, mem, 4)

def on_rand(cpu, mem, ad):
  import random
  cpu.eax = random.randint(0x00000000, 0xffffffff)
  print '[!] msvcr120.dll!rand() -> %08x' % cpu.eax
  stdcall_ret(cpu, mem, 0)

def main(argv):
  log_to_stdout()

  if len(argv) != 3:
    print 'usage: %s <exe> <db>' % (sys.argv[0])
    sys.exit(1)

  core = Medusa()
  core.open_exe(argv[1], argv[2], False)

  doc = core.document

  global emul
  emul = Execution(doc)
  if not emul.init([ sys.argv[1] ], [], '.'):
    print 'failed to initialize execution'
    return 1
  if not emul.set_emulator('llvm'):
    print 'cannot set emulator'
    return 1

  for lbl in doc.labels:
    lbl_addr = doc.get_label_addr(lbl.name)
    print 'hook %s at %s' % (lbl.name, lbl_addr)
    emul.hook_fn(lbl.name, on_symbol)

  emul.hook_fn('kernel32.dll!ExitProcess', on_ExitProcess)
  emul.hook_fn('user32.dll!MessageBoxA', on_MessageBoxA)
  emul.hook_fn('msvcr120.dll!rand', on_rand)

  emul.mem.write_u32(emul.cpu.esp, 0xdeaddead)

  ep_addr = doc.get_label_addr('start')
  print 'start: %s' % ep_addr
  emul.execute(ep_addr)

if __name__ == '__main__':
  sys.exit(main(sys.argv))

