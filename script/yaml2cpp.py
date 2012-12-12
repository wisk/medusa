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

    def _ConvertSemanticToCode(self, sem):
        class Expr:
            def __init__(self, oper, lexpr, rexpr, depth = 0):
                self.oper  = self.ConvertOper(oper)
                self.lexpr = self.ConvertOp(lexpr)
                self.rexpr = self.ConvertOp(rexpr)
                self.depth = depth

            def ConvertOp(self, op):
                if type(op) == str:
                    op_format = lambda x: 'new IdentifierExpression(%s, &m_CpuInfo)\n' % x

                    if op.startswith('sz('):
                        op_format = lambda x: 'new ConstantExpression(m_CpuInfo.GetSizeOfRegisterInBit(%s), m_CpuInfo.GetSizeOfRegisterInBit(%s) / 8)\n' % (x, x)
                        op = op[3:-1]
                    elif op.startswith('mem('):
                        op_format = lambda x: 'new MemoryExpression(nullptr, new IdentifierExpression(%s, &m_CpuInfo))\n' % x
                        op = op[4:-1]

                    if op.startswith('op'):
                        return 'rInsn.Operand(%d)->GetSemantic(&m_CpuInfo)\n' % int(op[2:])
                    elif op == 'reg_sk': # stack register
                        return op_format('m_CpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister)')
                    elif op == 'reg_ip': # instruction pointer register
                        return op_format('m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister)')
                    elif op == 'ni': # next instruction
                        return 'new OperationExpression(OperationExpression::OpAdd,\n\
                            new IdentifierExpression(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister), &m_CpuInfo),\n\
                            new ConstantExpression(m_CpuInfo.GetSizeOfRegisterInBit(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister)), rInsn.GetLength()))\n'
                    else:
                        assert(0)
                else:
                    return str(op)

            def ConvertOper(self, op):
                conv = { '=': 'Aff', '+': 'Add', '-': 'Sub', '&': 'And', '|': 'Or', '^': 'Xor' }
                return 'new OperationExpression(OperationExpression::Op%s,\n%%s,\n%%s)\n' % conv[op]

            def __str__(self):
                lexpr = Indent(str(self.lexpr), self.depth + 1)[:-1]
                rexpr = Indent(str(self.rexpr), self.depth + 1)[:-1]
                oper  = Indent((self.oper % (lexpr, rexpr)), self.depth)
                return oper

        insns = sem.split(';')
        sem_bind = []
        res = ''
        for insn in insns:
            tok = insn.split()
            oprd = []
            oper = []
            if len(tok) == 3 and len(tok[1]) == 1:
                expr_root = Expr(tok[1], tok[0], tok[2])
                sem_bind.append(expr_root)
            elif len(tok) == 3 and len(tok[1]) == 2:
                expr_sub  = Expr(tok[1][0], tok[0], tok[2])
                expr_root = Expr(tok[1][1], tok[0], expr_sub)
                sem_bind.append(expr_root)
        if len(sem_bind) == 1:
            res += 'auto pExpr = %s;\n' % str(sem_bind[0])[:-1]
            res += 'rInsn.SetSemantic(pExpr);\n'

        else:
            res += 'Expression::List ExprList;\n'
            for i in range(len(sem_bind)):
                res += 'auto pSem%d = %s;\n' % (i, str(sem_bind[i]))
                res += 'ExprList.push_back(pSem%d);\n' % i
            res += 'rInsn.SetSemantic(new BindExpression(ExprList));\n'

        return self._GenerateBrace(res)


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

class X86ArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()
        self.all_oprd = set()
        self.all_dec = set()

    # Architecture dependant methods
    def __X86_GenerateMethodName(self, type_name, opcd_no, in_class = False):
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)' % self.arch['arch_info']['name'].capitalize()

        if opcd_no == None:
            return meth_fmt % 'Invalid'

        return meth_fmt % ('%s_%02x' % (type_name.capitalize(), opcd_no))

    def __X86_GenerateInstructionComment(self, opcd_info):
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

    def __X86_GenerateSingleInstruction(self, opcd):
        res = ''
        if 'prefix' in opcd:
            res += self._GenerateRead('Prefix', 'Offset - 2', 8)
        if 'suffix' in opcd:
            res += self._GenerateRead('Suffix', 'Offset + 0', 8)

        if 'reference' in opcd:
            ref_oprd = None
            if 'operand' in opcd:
                ref_oprd = opcd['operand']
            res += self.__X86_GenerateReference(opcd['reference'], ref_oprd)
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



    def __X86_GenerateMultipleInstruction(self, opcd_arr):
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
                res += self._GenerateCondition('else', None, self.__X86_GenerateInstruction(opcd))
                return res

        res += 'return false;\n'
        return res

    def __X86_GenerateInstruction(self, opcd):
        if 'sub_opcodes' in opcd:
            return self.__X86_GenerateMultipleInstruction(opcd)
        else:
            return self.__X86_GenerateSingleInstruction(opcd)


    def __X86_GenerateInstructionBody(self, opcd):
        res = ''
        pfx_n = None

        if 'constraint' in opcd:
            if opcd['constraint'].startswith('pfx'):
                pfx_n = int(opcd['constraint'][-1])
            if opcd['constraint'] == 'd64':
                res += 'rInsn.Prefix() |= X86_Prefix_REX_w; /* d64 constraint */\n'

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

        conv_flags = { 'cf':'X86_FlCf', 'pf':'X86_FlPf', 'af':'X86_FlAf', 'zf':'X86_FlZf',
                'sf':'X86_FlSf', 'tf':'X86_FlTf', 'if':'X86_FlIf', 'df':'X86_FlDf', 'of':'X86_FlOf' }

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
            res += self._GenerateCondition(
                    'if',
                    '%s == false' % self.__X86_GenerateOperandMethod(opcd['operand']),
                    'return false;\n')
        if 'semantic' in opcd:
            res += self._ConvertSemanticToCode(opcd['semantic'])
        res += 'return true;\n'
        return res


    def __X86_GenerateInstructionCondition(self, opcd):
        cond = []

        if 'cpu_model' in opcd:
            cond.append(' || '.join(['m_CpuModel %s' %  x for x in opcd['cpu_model'].split(',')]))

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
        res = 'Operand__%s(rBinStrm, Offset, rInsn)' % '_'.join(oprd)
        self.all_oprd.add('_'.join(oprd))
        return res

    def __X86_GenerateReference(self, ref, oprd = None):
        res = ''
        tbl_off = ''

        if ref.startswith('table_FP'):
            tbl_off = '- 0xc0'

        if ref.startswith('table_'):
            res += self._GenerateRead('Opcode', 'Offset', 8)
            res += 'rInsn.Length()++;\n'
            res += self._GenerateCondition('if', 'Opcode + 1 > sizeof(m_%s)' % ref.capitalize(), 'return false;')
            res += 'return (this->*m_%s[Opcode%s])(rBinStrm, Offset + 1, rInsn);\n' % (ref.capitalize(), tbl_off)

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
                    case_statm += self.__X86_GenerateInstruction(tmp_opcd)

                elif 'invalid' in opcd_g:
                    case_statm += 'return false;\n'

                else:
                    case_statm += self.__X86_GenerateInstruction(opcd_g)

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
                        self.__X86_GenerateReference(fpu_info['group'], fpu_oprd))+\
                    self._GenerateCondition('else', None,\
                        self.__X86_GenerateReference(fpu_info['table']))

        return res


    def GenerateHeader(self):
        res = ''

        res += 'private:\n'
        res += Indent('typedef bool (%sArchitecture:: *TDisassembler)(BinaryStream const&, TOffset, Instruction&);\n' % self.arch['arch_info']['name'].capitalize())

        for name in sorted(self.arch['insn']['table']):
            if 'FP' in name:  opcd_no = 0xc0
            else:             opcd_no = 0x00

            res += Indent('static const TDisassembler m_%s[%#x];\n' % (name.capitalize(), 0x100 - opcd_no))
            for opcd in self.arch['insn']['table'][name]:
                res += Indent('%s;\n' % self.__X86_GenerateMethodName(name, opcd_no, True))
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
                res += self.__X86_GenerateInstructionComment(opcd)
                res += '%s\n' % self.__X86_GenerateMethodName(name, opcd_no, False)
                res += self._GenerateBrace(Indent(self.__X86_GenerateInstruction(opcd)))
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

class ArmArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()

        for insn in self.arch['arm32']:
            self.all_mnemo.add(insn['mnemonic'])

    def __ARM_GenerateInstruction(self, insn):
        res = ''
        res += 'rInsn.SetOpcode(ARM_Opcode_%s);\n' % insn['mnemonic'].capitalize()
        res += 'rInsn.Length() += 4;\n'

        if 'operation_type' in insn:
            map_op_type = { 'jmp' : 'Instruction::OpJump', 'call' : 'Instruction::OpCall' }
            res += 'rInsn.SetOperationType(%s);\n' % map_op_type[insn['operation_type']]

        oprd_cnt = 0

        if 'format' in insn:
            fmt = insn['format']

            for field, value in fmt.items():
                var_name = ''.join([x.capitalize() for x in field.split('_')])
                if type(value) == int:
                    res += 'u32 %s = ExtractBit<%d>(Opcode);\n' % (var_name, value)
                elif type(value) == list:
                    res += 'u32 %s = ExtractBits<%d, %d>(Opcode);\n' % (var_name, value[0], value[1])
                else:
                    raise Exception('Unknown value type')

                if var_name == 'CondField':
                    res += 'rInsn.SetTestedFlags(CondField);\n\n'

                elif var_name == 'LField':
                    res += self._GenerateCondition('if', 'LField', 'rInsn.Prefix() |= ARM_Prefix_L;\n')
                elif var_name == 'WField':
                    res += self._GenerateCondition('if', 'WField', 'rInsn.Prefix() |= ARM_Prefix_W;\n')
                elif var_name == 'SField':
                    res += self._GenerateCondition('if', 'SField', 'rInsn.Prefix() |= ARM_Prefix_S;\nrInsn.SetUpdatedFlags(ARM_FlNf | ARM_FlCf | ARM_FlVf | ARM_FlZf);\n')
                elif var_name == 'UField':
                    res += self._GenerateCondition('if', 'UField', 'rInsn.Prefix() |= ARM_Prefix_U;\n')
                elif var_name == 'PField':
                    res += self._GenerateCondition('if', 'PField', 'rInsn.Prefix() |= ARM_Prefix_P;\n')

                if (var_name == 'RdField' and (insn['mnemonic'] == 'ldr' or insn['mnemonic'] == 'mov')) or (var_name == 'RegListField' and insn['mnemonic'] == 'stm'):
                    res += self._GenerateCondition('if', '%s & ARM_RegPC' % var_name, 'rInsn.SetOperationType(Instruction::OpRet);\n')

            if 'operand' in insn:
                oprd_cnt = 0
                while oprd_cnt < len(insn['operand']):
                    oprd = insn['operand'][oprd_cnt]
                    var_name = ''.join([x.capitalize() for x in oprd.split('_')])

                    # LATER: Quick workaround...
                    if (insn['mnemonic'] == 'str' or insn['mnemonic'] == 'ldr') and oprd == 'rn_field' and 'p_field' in insn['format']:
                        oprd_flag = None
                        oprd_meth = None

                        if 'rm_field' in insn['format']:
                            oprd_flag = 'O_MEM32 | O_REG32 | O_SREG'
                            oprd_meth = 'rInsn.Operand(%d)->SetSecReg(1 << RmField);\n'
                            oprd_name = 'RmField'
                            oprd_type = 'O_REG32'
                        elif 'imm_field' in insn['format']:
                            oprd_flag = 'O_MEM32 | O_REG32 | O_DISP32'
                            oprd_meth = 'rInsn.Operand(%d)->SetValue(ImmField);\n'
                            oprd_name = 'ImmField'
                            oprd_type = 'O_IMM32'

                        res_p_true = ''
                        res_p_true += 'rInsn.Operand(%d)->SetType(%s);\n' % (oprd_cnt, oprd_flag)
                        res_p_true += 'rInsn.Operand(%d)->SetReg(1 << %s);\n' % (oprd_cnt, var_name)
                        res_p_true += oprd_meth % oprd_cnt

                        res_p_false = ''
                        res_p_false += 'rInsn.Operand(%d)->SetType(O_MEM32 | O_REG32);\n' % oprd_cnt
                        res_p_false += 'rInsn.Operand(%d)->SetReg(1 << %s);\n' % (oprd_cnt, var_name)
                        res_p_false += 'rInsn.Operand(%d)->SetType(%s);\n' % (oprd_cnt + 1, oprd_type)
                        res_p_false += oprd_meth.replace('SecReg', 'Reg') % (oprd_cnt + 1)

                        oprd_cnt += 1

                        res += self._GenerateCondition('if', 'PField', res_p_true)
                        res += self._GenerateCondition('else', None,   res_p_false)

                        if 'imm_field' in insn['format']:
                            res_rel = ''
                            res_rel += self._GenerateRead('Imm', 'Offset + ImmField + 8', 32) # Prefetch thing?
                            res_rel += 'rInsn.Operand(%d)->SetValue(Imm);\n' % (oprd_cnt - 1)
                            res_rel += 'rInsn.Operand(%d)->Type() |= O_REG_PC_REL;\n' % (oprd_cnt - 1)
                            res += self._GenerateCondition('if', '(1 << %s) & ARM_RegPC' % var_name, res_rel)

                    elif oprd == 'rl_field':
                        res += 'rInsn.Operand(%d)->SetType(O_REG32);\n' % oprd_cnt
                        res += 'rInsn.Operand(%d)->SetReg(RlField);\n' % oprd_cnt

                    elif oprd[0] == 'r':
                        res += 'rInsn.Operand(%d)->SetType(O_REG32);\n' % oprd_cnt
                        res += 'rInsn.Operand(%d)->SetReg(1 << %s);\n' % (oprd_cnt, var_name)
                    elif oprd[0] == 'i':
                        res += 'rInsn.Operand(%d)->SetType(O_IMM32);\n' % oprd_cnt
                        res += 'rInsn.Operand(%d)->SetValue(%s);\n' % (oprd_cnt, var_name)
                    elif oprd[0] == 'o':
                        res += 'rInsn.Operand(%d)->SetType(O_REL32);\n' % oprd_cnt
                        res += 'rInsn.Operand(%d)->SetValue(SignExtend<s64, %d>((%s << 2) + 4)); /* NOTE: +8 for prefetch -4 */\n' % (oprd_cnt, (insn['format'][oprd][1] + 2), var_name)

                    oprd_cnt += 1



        res += 'return true;\n'

        return self.__ARM_GenerateMethodPrototype(insn, False) + '\n' + self._GenerateBrace(res)

    def __ARM_GenerateMethodName(self, insn):
        return 'Instruction%s_%08x_%08x' % (insn['mnemonic'].capitalize(), insn['mask'], insn['value'])

    def __ARM_GenerateMethodPrototype(self, insn, in_class = False):
        mnem = insn['mnemonic']
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)' % self.arch['arch_info']['name'].capitalize()

        return meth_fmt % self.__ARM_GenerateMethodName(insn)

    def GenerateHeader(self):
        res = ''
        res += 'static char const *m_Mnemonic[%#x];\n' % (len(self.all_mnemo) + 1)

        for insn in sorted(self.arch['arm32'], key=lambda a:a['mnemonic']):
            res += self.__ARM_GenerateMethodPrototype(insn, True) + ';\n'

        return res

    def GenerateSource(self):
        res = ''

        res += 'bool ArmArchitecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res_disasm = ''
        res_disasm += self._GenerateRead('Opcode', 'Offset', 32)

        cond = 'if'
        for insn in sorted(self.arch['arm32'], key=lambda a:a['mnemonic']):
            res_disasm += self._GenerateCondition(cond, '(Opcode & %#010x) == %#010x' % (insn['mask'], insn['value']),
                'return ' + self.__ARM_GenerateMethodName(insn) + '(rBinStrm, Offset, Opcode, rInsn);')
            cond = 'else if'
        res_disasm += 'return false;\n'
        res += self._GenerateBrace(res_disasm)

        for insn in self.arch['arm32']:
            res += self.__ARM_GenerateInstruction(insn)

        return res

    def GenerateOpcodeEnum(self):
        res = ',\n'.join('ARM_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum ARM_Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.arch['arch_info']['name'].capitalize(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''
        return res

    def GenerateOperandCode(self):
        res = ''
        return res

def main():
    f = open(sys.argv[1])
    d = yaml.load(f)

    conv = None

    if d['arch_info']['name'] == 'x86':
        conv = X86ArchConvertion(d)
    elif d['arch_info']['name'] == 'arm':
        conv = ArmArchConvertion(d)

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
