#!/usr/bin/env python

import sys
import yaml
import time

def Indent(text, indent = 1):
    if text == None:
        return ''
    res = ''
    for l in text.split('\n')[0:-1]:
        if l == '':
            res += '\n'
            continue
        res += '  ' * indent + l + '\n'
    return res

class ArchConvertion:
    def __init__(self, arch):
        self.arch = arch

    def GenerateBanner(self):
        return '/* This file has been automatically generated, you must _NOT_ edit it directly. (%s) */\n' % time.ctime()


    # Private methods
    def _GenerateBrace(self, code):
        return '{\n' + Indent(code) + '}\n'

    def _GenerateSwitch(self, cond, cases, default):
        res = ''

        res += 'switch(%s)\n' % cond
        res += '{\n'
        for case in cases:
            res += 'case %s:\n' % case[0]
            res += Indent(case[1])
            if case[2]: res += Indent('break;\n')
        res += 'default:\n'
        res += Indent('%s;\n' % default)
        res += '}\n'
        return res

    def _GenerateRead(self, var_name, addr, sz):
        return 'u%d %s;\nrBinStrm.Read(%s, %s);\n\n' % (sz, var_name, addr, var_name)

    def _GenerateMethodName(self, type_name, opcd_no, in_class = False):
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)'
        if in_class == True:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)' % self.arch['arch_info']['name'].capitalize()

        if opcd_no == None:
            return meth_fmt % 'Invalid'

        return meth_fmt % ('%s_%02x' % (type_name.capitalize().replace('Table', 'Opcode'), opcd_no))

    def _GenerateInstructionComment(self, opcd_info):
        def Format(opcd):
            res = ''
            for k, v in opcd.items():
                if type(v) == int:
                    res += ' * %s: %02x\n' % (k, v)
                elif v == None:
                    res += ' * %s\n' % k
                else:
                    res += ' * %s: %s\n' % (k, v)
            return res

        res = ''
        if 'sub_opcodes' in opcd_info:
            res += '/** instructions\n'
            res += ' * opcode: %02x\n' % opcd_info['opcode']
            res += ' *\n'
            for opcd in opcd_info['sub_opcodes']:
                res += Format(opcd)
                res += ' *\n'
            res += '**/\n'
        elif 'group' in opcd_info and opcd_info['group'].startswith('group_FP'):
            res += 'fpu:\n'
        elif 'reference' in opcd_info and opcd_info['reference'].startswith('group_'):
            res += '/** group:\n'
            for opcd in self.arch['insn'][opcd_info['reference']]:
                res += Format(opcd)
                res += ' *\n'
            res += '**/\n'
        else:
            res += '/** instruction\n'
            res += Format(opcd_info)
            res += '**/\n'

        return res

    def _GenerateInstruction(self, opcd):
        res = ''
        return res

    def GenerateHeader(self):
        pass

    def GenerateSource(self):
        pass

    def GenerateOpcodeEnum(self):
        pass

    def GenerateOpcodeString(self):
        pass

    def GenerateOperandDefinition(self):
        pass

    def GenerateOperandCode(self):
        pass

class ComplexArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()
        self.all_oprd = set()
        self.all_dec = set()

    def GenerateHeader(self):
        res = ''

        res += 'private:\n'
        res += Indent('typedef bool (%sArchitecture:: *TDisassembler)(BinaryStream const& TOffset, Instruction&);\n')

        for name in sorted(self.arch):
            if not name.startswith('table_'): continue

            if 'FP' in name:
                opcd_no = 0xc0
                res += Indent('static const TDisassembler m_%s[0x40];\n' % name.capitalize())
                for opcd in self.arch[name]:
                    res += Indent('%s;\n' % self._GenerateMethodName(name, opcd_no, True))
                    opcd_no += 1
                res += '\n'

            else:
                opcd_no = 0x0
                res += Indent('static const TDisassembler m_%s[0x100];\n' % name.capitalize())
                for opcd in self.arch[name]:
                    res += Indent('%s;\n' % self._GenerateMethodName(name, opcd_no, True))
                    opcd_no += 1
                res += '\n'
        return res

    def GenerateSource(self):
        res = ''
        arch_name = self.arch['arch_info']['name'].capitalize()

        for name in sorted(self.arch['insn'], key=lambda a:a[0]):
            if not name.startswith('table_'): continue

            if 'FP' in name:  opcd_no = 0xc0
            else:             opcd_no = 0x00

            res += 'const %sArchitecture::TDisassembler %sArchitecture::m_%s[%#x] =\n' % (arch_name, arch_name, name.capitalize(), 0x100 - opcd_no)
            res += '{\n'
            tbl_elm = []
            for opcd in self.arch['insn'][name]:
                tbl_elm.append(Indent('&%sArchitecture::%s_%02x\n' % (arch_name, name.capitalize().replace('Table', 'Opcode'), opcd_no))[:-1])
                opcd_no += 1
            res += ',\n'.join(tbl_elm)
            res += '\n};\n\n'

        for name in sorted(self.arch['insn']):
            if not name.startswith('table_'): continue

            if 'FP' in name: opcd_no = 0xc0
            else:            opcd_no = 0x00

            for opcd in self.arch['insn'][name]:
                res += self._GenerateInstructionComment(opcd)
                res += '%s\n' % self._GenerateMethodName(name, opcd_no)
                res += self._GenerateBrace(Indent(self._GenerateInstruction(opcd)))
                res += '\n'
                opcd_no += 1

        return res

    def GenerateOpcodeEnum(self):
        res = ''
        for insns in sorted(self.arch):
            if insns.startswith('table_'):
                pass
            elif insns.startswith('group_'):
                pass
        return res


    def GenerateOpcodeString(self):
        res = ''
        return res

    def GenerateOperandDefinition(self):
        res = ''
        return res

    def GenerateOperandCode(self):
        res = ''
        return res

class ReducedArchConvertion(ArchConvertion):
    def GenerateHeader(self):
        res = ''
        return res

    def GenerateSource(self):
        res = ''
        return res

    def GenerateOpcodeEnum(self):
        res = ''
        return res

    def GenerateOpcodeString(self):
        res = ''
        return res

    def GenerateOperandDefinition(self):
        res = ''
        return res

    def GenerateOperandCode(self):
        res = ''
        return res

def ConvertYamlToCpp(arch):
    if arch.insn_set_type == 'complex':
        ComplexArchConvertion(arch)
    elif arch.insn_set_type == 'reduced':
        ReducedArchConvertion(arch)
    else:
        print('Unknown instruction set type')

def main():
    f = open(sys.argv[1])
    d = yaml.load(f)

    conv = None

    if d['arch_info']['insn_set_type'] == 'complex':
        conv = ComplexArchConvertion(d)
    elif d['arch_info']['insn_set_type'] == 'reduced':
        conv = ReducedArchConvertion(d)

    hdr = conv.GenerateHeader()
    src = conv.GenerateSource()
    enm = conv.GenerateOpcodeEnum()
    mns = conv.GenerateOpcodeString()
    opd = conv.GenerateOperandDefinition()
    opc = conv.GenerateOperandCode()

    arch_hpp = open('%s_opcode.ipp' % d['arch_info']['name'], 'w')
    arch_cpp = open('%s_opcode.cpp' % d['arch_info']['name'], 'w')

    arch_hpp.write(conv.GenerateBanner())
    arch_hpp.write(enm)
    arch_hpp.write(hdr)
    arch_hpp.write(opd)

    arch_cpp.write(conv.GenerateBanner())
    arch_cpp.write('#include "%s_architecture.hpp"\n' % d['arch_info']['name'])
    arch_cpp.write(mns)
    arch_cpp.write(src)
    arch_cpp.write(opc)

if __name__ == "__main__":
    main()
