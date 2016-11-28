#!/usr/bin/env python2

from pydusa import *
import sys

class FunctionInformation(object):
  def __init__(self, function, execution, calling_convention, symbol):
    self.function = function
    self.execution = execution
    self.calling_convention = calling_convention
    self.symbol = symbol

  def __call__(self):
    params = []
    param_no = len(self.function.func_code.co_varnames)
    for i in range(param_no):
      params.append(self.execution.get_func_param(self.calling_convention, i))
    ret_val = self.function(*params)
    if ret_val:
      self.execution.ret_val_from_func(self.calling_convention, param_no, ret_val)
    else:
      self.execution.ret_from_func(self.calling_convention, param_no)

    print '%s(%s) -> %08x' % (self.symbol, ', '.join(['%08x' % x for x in params]), ret_val)


class Hook(object):

  def __init__(self, execution, calling_convention, symbol):
    self.execution = execution
    self.calling_convention = calling_convention
    self.symbol = symbol

  def __call__(self, f):
    FI = FunctionInformation(f, self.execution, self.calling_convention, self.symbol)
    self.execution.hook_fn(self.symbol, FI)

def stdcall_ret(cpu, mem, param_no):
  ret_addr = mem.read_u32(cpu.esp)
  cpu.eip = ret_addr
  cpu.esp += (4 + param_no * 4)

def on_symbol(cpu, mem, ad):
  global emul
  print 'function %s called' % emul.get_hook_name()

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

@Hook(emul, 'stdcall', 'kernel32.dll!ExitProcess')
def on_ExitProcess(ExitCode):
  global emul
  emul.cpu.rip = 0xdeaddead

@Hook(emul, 'stdcall', 'user32.dll!MessageBoxA')
def on_MessageBoxA(hWnd, lpCaption, lpText, MsgType):
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

  print caption, text
  return 0

@Hook(emul, 'cdecl', 'msvcr120.dll!rand')
def on_rand(cpu, mem, ad):
  import random
  return random.randint(0x00000000, 0xffffffff)

  emul.mem.write_u32(emul.cpu.esp, 0xdeaddead)

  ep_addr = doc.get_label_addr('start')
  print 'start: %s' % ep_addr
  emul.execute(ep_addr)

if __name__ == '__main__':
  sys.exit(main(sys.argv))

