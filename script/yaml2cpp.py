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

    # Architecture dependant methods
    def __X86_GenerateInstructionBody(self, opcd):
        res = ''
        pfx_n = None
        if 'constraint' in opcd and opcd['constraint'].startswith('pfx'):
            pfx_n = int(opcd['constraint'][-1])

        if 'invalid' in opcd:
            return 'return false; /* INVALID */\n'

        if pfx_n != None:
            res += 'rInsn.Length()++;\n'
            if 'mnemonic' in opcd:
                res += 'rInsn.Prefix() |= X86_Prefix_%s;\n' % opcd['mnemonic']
            res += 'return Disassemble(rBinStrm, Offset + %d, rInsn);\n' % (pfx_n - 1)
            return res

        res += 'rInsn.Length()++;\n'
        if 'mnemonic' in opcd:
            res += 'rInsn.SetOpcode(X86_Opcode_%s);\n' % opcd['mnemonic'].capitalize()
            self.all_mnemo.add(opcd['mnemonic'])

        conv_flags = { 'cf':'X86_Flag::Carry', 'pf':'X86_Flag::Parity', 'af':'X86_Flag::Adjust', 'zf':'X86_Flag::Zero',
                'sf':'X86_Flag::Sign', 'tf':'X86_Flag::Trap', 'if':'X86_Flag::Interrupt', 'df':'X86_Flag::Direction', 'of':'X86_Flag::Overflow' }

        if 'test_flags' in opcd:
            res += 'rInsn.SetTestedFlags(%s);\n' % ' | '.join(conv_flags[x] for x in opcd['test_flags'])
        if 'update_flags' in opcd:
            res += 'rInsn.SetUpdatedFlags(%s);\n' % ' | '.join(conv_flags[x] for x in opcd['update_flags'])
        if 'clear_flags' in opcd:
            res += 'rInsn.SetClearedFlags(%s);\n' % ' | '.join(conv_flags[x] for x in opcd['clear_flags'])
        if 'fix_flags' in opcd:
            res += 'rInsn.SetFixedFlags(%s);\n' % ' | '.join(conv_flags[x] for x in opcd['fix_flags'])


        conv_optype = { 'jmp':'Instruction::OpJump', 'call':'Instruction::OpCall', 'ret':'Instruction::OpRet', 'cond':'Instruction::OpCond' }
        if 'operation_type' in opcd:
            res += 'rInsn.SetOperationType(%s);\n' % ' | '.join(conv_optype[x] for x in opcd['operation_type'])

        if 'operand' in opcd:
            res += 'return %s;\n' % self.__X86_GenerateOperandMethod(opcd['operand'])
            return res

        res += 'return true;\n'
        return res


    def __X86_GenerateInstructionCondition(self, opcd):
        cond = []

        if 'cpu_model' in opcd:
            cond.append(' || '.join(['m_CpuModel %s' % opcd['cpu_model']]))

        if 'attr' in opcd:
            for f in opcd['attr']:
                # Mode attr
                if f == 'm64':
                    cond.append('m_Cfg.Get("Bit") == X86_Bit_64')
                elif f == 'nm64':
                    cond.append('m_Cfg.Get("Bit") != X86_Bit_64')
                elif f == 'a64':
                    cond.append('m_Cfg.Get("Bit") == X86_Bit_64 && !(rInsn.Prefix() & X86_Prefix_AdSize)')
                elif f == 'na16':
                    cond.append('(m_Cfg.Get("Bit") == X86_Bit_16 && rInsn.Prefix() & X86_Prefix_AdSize) || (m_Cfg.Get("Bit") != X86_Bit_16 && !(rInsn.Prefix() & X86_Prefix_AdSize))')
                elif f == 'ia64':
                    cond.append('m_ProcType == X86_ProcType_IA64')

                # Manufacturer
                elif f == 'amd':
                    cond.append('m_ProcType == X86_ProcType_AMD')
                elif f == 'intel':
                    cond.append('m_ProcType == X86_ProcType_INTEL')
                elif f == 'cyrix':
                    cond.append('m_ProcType == X86_ProcType_CYRIX')
                elif f == 'iit':
                    cond.append('m_ProcType == X86_ProcType_IIT')

                # Prefix
                elif f == 'rexb':
                    cond.append('rInsn.GetPrefix() & X86_Prefix_REX_b')
                elif f == 'rexw':
                    cond.append('rInsn.GetPrefix() & X86_Prefix_REX_w')

                # Unknown attr ?
                #else:
                #    raise Exception('Unknown attr %s', f)

        if 'prefix' in opcd:
            if type(opcd['prefix']) == list:
                cond.append('(%s)' % ' || '.join(['Prefix == %#04x' % x for x in opcd['prefix']]))
            else:
                cond.append('Prefix == %#04x' % opcd['prefix'])
        if 'suffix' in opcd:
            cond.append('Suffix == %#04x' % opcd['suffix'])

        if len(cond) == 0:
            return None

        return ' && '.join(cond)

    def __X86_GenerateOperandMethod(self, oprd):
        res = 'Operand__%s(rBinStrm, Offsert, rInsn)' % '_'.join(oprd)
        self.all_oprd.add('_'.join(oprd))
        return res

    # Private methods
    def _GenerateBrace(self, code):
        return '{\n' + Indent(code) + '}\n'

    def _GenerateCondition(self, cond_type, cond, statm):
        res = ''

        if cond != None:
            res += '%s (%s)\n' % (cond_type, cond)
        else:
            res += '%s\n' % (cond_type)

        if statm[-1] != '\n':
            res += Indent(statm + '\n')
        else:
            res += self._GenerateBrace(statm)
        return res

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

    def _GenerateReference(self, ref, oprd = None):
        res = ''
        tbl_off = ''

        if ref.startswith('table_FP'):
            tbl_off = '- 0xc0'

        if ref.startswith('table_'):
            res += self._GenerateRead('Opcode', 'Offset', 8)
            res += 'rInsn.Length()++;\n'
            res += self._GenerateCondition('if', 'Opcode + 1 > sizeof(%s)' % ref.capitalize(), 'return false;')
            res += 'return (this->*m_%s[Opcode%s])(rBinStrm, Offset + 1, rInsn);\n' % (tbl_off, ref.capitalize())

        elif ref.startswith('group_'):
            grp = self.arch['insn']['group'][ref]
            refs = []
            grp_no = 0
            for opcd_g in grp:
                case_statm = ''
                if type(opcd_g) == list:
                    case_statm += self._GenerateMultipleInstruction(opcd_g)

                elif oprd != None:
                    tmp_opcd = opcd_g
                    tmp_opcd['operand'] = oprd
                    case_statm += self._GenerateInstruction(tmp_opcd)

                elif 'invalid' in opcd_g:
                    case_statm += 'return false;\n'

                else:
                    case_statm += self._GenerateInstruction(opcd_g)

                # HACK: needed to generate scope for reading (prefix/suffix)
                if case_statm.find('Read') != -1:
                    case_statm = self._GenerateBrace(case_statm)

                refs.append( ('%#x' % grp_no, case_statm, False) )
                grp_no += 1

            res += self._GenerateRead('ModRmByte', 'Offset', 8)
            res += 'x86::ModRM ModRm(ModRmByte);\n'
            res += self._GenerateSwitch('ModRm.Reg()', refs, 'return false')

        elif ref.startswith('fpu'):
            fpu_info = self.arch['insn']['fpu'][ref]
            fpu_oprd = None
            if 'operand' in fpu_info:
                fpu_oprd = fpu_info['operand']
            fpu = self.arch['insn']['fpu'][ref]
            res += self._GenerateRead('ModRmByte', 'Offset', 8)+\
                    self._GenerateCondition('if', 'ModRmByte < 0xc0',\
                        self._GenerateReference(fpu_info['group'], fpu_oprd))+\
                    self._GenerateCondition('else', None,\
                        self._GenerateReference(fpu_info['table']))

        return res

    def _GenerateMethodName(self, type_name, opcd_no, in_class = False):
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)'
        if in_class == True:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)' % self.arch['arch_info']['name'].capitalize()

        if opcd_no == None:
            return meth_fmt % 'Invalid'

        return meth_fmt % ('%s_%02x' % (type_name.capitalize(), opcd_no))

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
        elif 'reference' in opcd_info and opcd_info['reference'].startswith('group_FP'):
            res += 'fpu:\n'
        elif 'reference' in opcd_info and opcd_info['reference'].startswith('group_'):
            res += '/** group:\n'
            for opcd in self.arch['insn']['group'][opcd_info['reference']]:
                res += Format(opcd)
                res += ' *\n'
            res += '**/\n'
        else:
            res += '/** instruction\n'
            res += Format(opcd_info)
            res += '**/\n'

        return res

    def _GenerateSingleInstruction(self, opcd):
        res = ''
        if 'prefix' in opcd:
            res += self._GenerateRead('Prefix', 'Offset - 2', 8)
        if 'suffix' in opcd:
            res += self._GenerateRead('Suffix', 'Offset + 0', 8)

        if 'reference' in opcd:
            ref_oprd = None
            if 'operand' in opcd:
                ref_oprd = opcd['operand']
            res += self._GenerateReference(opcd['reference'], ref_oprd)
            return res

        if 'cpu_model' in opcd or 'attr' in opcd or 'prefix' in opcd or 'suffix' in opcd:
            cond = self.__X86_GenerateInstructionCondition(opcd)
            if cond == None:
                res += self.__X86_GenerateInstructionBody(opcd)
            else:
                res += self._GenerateCondition('if', cond, self.__X86_GenerateInstructionBody(opcd))
                res += self._GenerateCondition('else', None, 'return false;')
                return res

        res += self.__X86_GenerateInstructionBody(opcd)
        return res



    def _GenerateMultipleInstruction(self, opcd_arr):
        res = ''
        cond_type = 'if'
        has_pref = False
        has_suff = False

        for opcd in opcd_arr['sub_opcodes']:
            if not has_pref and 'prefix' in opcd:
                res += self._GenerateRead('Prefix', 'Offset - 2', 8)
                has_pref = True
            if not has_suff and 'suffix' in opcd:
                res += self._GenerateRead('Suffix', 'Offset + 0', 8)
                has_suff = True

        for opcd in reversed(opcd_arr['sub_opcodes']):
            if 'cpu_model' in opcd or 'attr' in opcd or 'prefix' in opcd or 'suffix' in opcd:
                cond = self.__X86_GenerateInstructionCondition(opcd)
                if cond_type == 'if' and cond == None:
                    if opcd != opcd_arr['sub_opcodes'][0]:
                        raise Exception('No condition for if\n%s', str(opcd))
                    cond_type = 'else'

                res += self._GenerateCondition(cond_type, cond, self.__X86_GenerateInstructionBody(opcd))
                cond_type = 'else if'
            else:
                if cond_type == 'if':
                    raise Exception('No condition for if\n%s', str(opcd))
                res += self._GenerateCondition('else', None, self._GenerateInstruction(opcd))
                return res

        res += 'return false;\n'
        return res

    def _GenerateInstruction(self, opcd):
        if 'sub_opcodes' in opcd:
            return self._GenerateMultipleInstruction(opcd)
        else:
            return self._GenerateSingleInstruction(opcd)

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

            if 'FP' in name:  opcd_no = 0xc0
            else:             opcd_no = 0x00

            opcd_no = 0xc0
            res += Indent('static const TDisassembler m_%s[%#x];\n' % (name.capitalize(), 0x100 - opcd_no))
            for opcd in self.arch[name]:
                res += Indent('%s;\n' % self._GenerateMethodName(name, opcd_no, True))
                opcd_no += 1
            res += '\n'

        return res

    def GenerateSource(self):
        res = ''
        arch_name = self.arch['arch_info']['name'].capitalize()

        for name in sorted(self.arch['insn']['table']):
            if 'FP' in name:  opcd_no = 0xc0
            else:             opcd_no = 0x00

            res += 'const %sArchitecture::TDisassembler %sArchitecture::m_%s[%#x] =\n' % (arch_name, arch_name, name.capitalize(), 0x100 - opcd_no)
            res += '{\n'
            tbl_elm = []
            for opcd in self.arch['insn']['table'][name]:
                tbl_elm.append(Indent('&%sArchitecture::%s_%02x\n' % (arch_name, name.capitalize(), opcd_no))[:-1])
                opcd_no += 1
            res += ',\n'.join(tbl_elm)
            res += '\n};\n\n'

        for name in sorted(self.arch['insn']['table']):
            if 'FP' in name: opcd_no = 0xc0
            else:            opcd_no = 0x00

            for opcd in self.arch['insn']['table'][name]:
                res += self._GenerateInstructionComment(opcd)
                res += '%s\n' % self._GenerateMethodName(name, opcd_no, False)
                res += self._GenerateBrace(Indent(self._GenerateInstruction(opcd)))
                res += '\n'
                opcd_no += 1

        return res

    def GenerateOpcodeEnum(self):
        res = ',\n'.join('X86_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum X86Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.arch['arch_info']['name'].capitalize(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''
        for oprd in self.all_oprd:
            if oprd == '': continue
            res += Indent('bool Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n' % oprd)
        return res

    def GenerateOperandCode(self):
        res = ''
        for oprd in self.all_oprd:
            if oprd == '': continue
            res += 'bool %sArchitecture::Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)' % (self.arch['arch_info']['name'].capitalize(), oprd)
            dec_op = []
            op_no = 0
            oprd = oprd.split('_')

            # HACK: Handling case with E?_I? is tedious, we have to figure out the size of ModR/M
            # (which can have SIB and/or ImmXX)
            ei_hack = ''
            for o in oprd:
                if o[0] == 'I' and oprd[0][0] == 'E':
                    ei_hack += 'size_t PrefixOpcodeLength = rInsn.GetLength();\n'

                    for o in oprd:
                        if o[0] == 'I' and oprd[0][0] == 'E':
                            ei_hack += self._GenerateCondition('if', '!Decode_%s(rBinStrm, Offset + (rInsn.GetLength() - PrefixOpcodeLength), rInsn, rInsn.Operand(%d))' % (o, op_no),\
                                    'return false;')
                        else:
                            ei_hack += self._GenerateCondition('if', '!Decode_%s(rBinStrm, Offset, rInsn, rInsn.Operand(%d))' % (o, op_no),\
                                    'return false;')
                        self.all_dec.add('Decode_%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)' % o)
                        op_no += 1

                    ei_hack += 'return true;\n'
                    break

            if len(ei_hack):
                res += self._GenerateBrace(ei_hack)
                continue

            for o in oprd:
                dec_op.append('Decode_%s(rBinStrm, Offset, rInsn, rInsn.Operand(%d))' % (o, op_no))
                op_no += 1
                self.all_dec.add('Decode_%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)' % o)

            res += self._GenerateBrace('return\n' + Indent(' &&\n'.join(dec_op) + ';\n'))
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
