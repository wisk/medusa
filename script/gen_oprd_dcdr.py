#!/usr/bin/env python

import sys
import db2cpp

oprd_reg = [ 'AH', 'AL', 'AX', 'BH', 'BL', 'CH', 'CL', 'CR8D', 'DH', 'DL', 'DX', 'ST0', 'ST1', 'ST2', 'ST3', 'ST4', 'ST5', 'ST6', 'ST7', 'eAX', 'eBP', 'eBX', 'eCX', 'eDI', 'eDX', 'eSI', 'eSP', 'r10', 'r10b', 'r11', 'r11b', 'r12', 'r12b', 'r13', 'r13b', 'r14', 'r14b', 'r15', 'r15b', 'r8', 'r8b', 'r9', 'r9b', 'rAX', 'rBP', 'rBX', 'rCX', 'rDI', 'rDX', 'rSI', 'rSP' ]

oprd_sreg = [ 'cs', 'ds', 'es', 'ss', 'fs', 'gs' ]

def OnSegRegister(sreg):
    res  = 'pOprd->SegReg(X86_Reg_%s);\n' % sreg.capitalize()
    res += 'pOprd->SetType(O_REG16);\n'
    res += 'return true;\n'
    return res


def GenCode(line):
    oprd = line[line.find('_') + 1: line.find('(')]
    res = 'bool X86Architecture::%s\n' % line

    body = 'return false; /* UNHANDLED OPERAND */\n'
    res += db2cpp.GenBrace(body)
    return res


def main():
    data = open(sys.argv[1], 'r').read()
    for l in data.split('\n')[1:]:
        if len(l):
            print(GenCode(l))

if __name__ == "__main__":
    main()
