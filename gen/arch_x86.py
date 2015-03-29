from arch import ArchConvertion
from helper import *

import ast

class X86ArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()
        self.all_oprd = set()
        self.all_dec = set()

        self.id_mapper = {
        'cf':'X86_FlCf', 'pf':'X86_FlPf', 'af':'X86_FlAf', 'zf':'X86_FlZf',
        'sf':'X86_FlSf', 'tf':'X86_FlTf', 'if':'X86_FlIf', 'df':'X86_FlDf', 'of':'X86_FlOf',
        'cs':'X86_Reg_Cs', 'ds':'X86_Reg_Ds', 'es':'X86_Reg_Es', 'ss':'X86_Reg_Ss', 'fs':'X86_Reg_Fs', 'gs':'X86_Reg_Gs',
        'al':'X86_Reg_Al', 'bl':'X86_Reg_Bl', 'cl':'X86_Reg_Cl', 'dl':'X86_Reg_Dl',
        'ah':'X86_Reg_Ah', 'bh':'X86_Reg_Bh', 'ch':'X86_Reg_Ch', 'dh':'X86_Reg_Dh',
        'spl':'X86_Reg_Spl', 'bpl':'X86_Reg_Bpl', 'sil':'X86_Reg_Sil', 'dil':'X86_Reg_Dil',
        'r8b':'X86_Reg_R8b', 'r9b':'X86_Reg_R9b', 'r10b':'X86_Reg_R10b', 'r11b':'X86_Reg_R11b',
        'r12b':'X86_Reg_R12b', 'r13b':'X86_Reg_R13b', 'r14b':'X86_Reg_R14b', 'r15b':'X86_Reg_R15b',
        'ax':'X86_Reg_Ax', 'bx':'X86_Reg_Bx', 'cx':'X86_Reg_Cx', 'dx':'X86_Reg_Dx',
        'si':'X86_Reg_Si', 'di':'X86_Reg_Di', 'sp':'X86_Reg_Sp', 'bp':'X86_Reg_Bp',
        'r8w':'X86_Reg_R8w', 'r9w':'X86_Reg_R9w', 'r10w':'X86_Reg_R10w', 'r11w':'X86_Reg_R11w',
        'r12w':'X86_Reg_R12w', 'r13w':'X86_Reg_R13w', 'r14w':'X86_Reg_R14w', 'r15w':'X86_Reg_R15w',
        'eax':'X86_Reg_Eax', 'ebx':'X86_Reg_Ebx', 'ecx':'X86_Reg_Ecx', 'edx':'X86_Reg_Edx',
        'esi':'X86_Reg_Esi', 'edi':'X86_Reg_Edi', 'esp':'X86_Reg_Esp', 'ebp':'X86_Reg_Ebp',
        'r8d':'X86_Reg_R8d', 'r9d':'X86_Reg_R9d', 'r10d':'X86_Reg_R10d', 'r11d':'X86_Reg_R11d',
        'r12d':'X86_Reg_R12d', 'r13d':'X86_Reg_R13d', 'r14d':'X86_Reg_R14d', 'r15d':'X86_Reg_R15d',
        'st0':'X86_Reg_St0', 'st1':'X86_Reg_St1', 'st2':'X86_Reg_St2', 'st3':'X86_Reg_St3',
        'st4':'X86_Reg_St4', 'st5':'X86_Reg_St5', 'st6':'X86_Reg_St6', 'st7':'X86_Reg_St7',
        'rax':'X86_Reg_Rax', 'rbx':'X86_Reg_Rbx', 'rcx':'X86_Reg_Rcx', 'rdx':'X86_Reg_Rdx',
        'rsi':'X86_Reg_Rsi', 'rdi':'X86_Reg_Rdi', 'rsp':'X86_Reg_Rsp', 'rbp':'X86_Reg_Rbp',
        'r8':'X86_Reg_R8', 'r9':'X86_Reg_R9', 'r10':'X86_Reg_R10', 'r11':'X86_Reg_R11',
        'r12':'X86_Reg_R12', 'r13':'X86_Reg_R13', 'r14':'X86_Reg_R14', 'r15':'X86_Reg_R15' }

    # Architecture dependant methods
    def _X86_GenerateMethodName(self, type_name, opcd_no, in_class = False):
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)' % self.GetArchName()

        if opcd_no == None:
            return meth_fmt % 'Invalid'

        return meth_fmt % ('%s_%02x' % (type_name.capitalize(), opcd_no))

    def _X86_GenerateInstructionComment(self, opcd_info):
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
            for opcd in self.arch['instruction']['group'][opcd_info['reference']]:
                res += Format(opcd)
                res += ' *\n'
            res += '**/\n'
        else:
            res += '/** instruction\n'
            res += Format(opcd_info)
            res += '**/\n'

        return res

    def _X86_GenerateSingleInstruction(self, opcd):
        res = ''
        if 'prefix' in opcd:
            res += self._GenerateRead('Prefix', 'Offset - 2', 8)
        if 'suffix' in opcd:
            res += self._GenerateRead('Suffix', 'Offset + 0', 8)

        if 'reference' in opcd:
            ref_oprd = None
            if 'operand' in opcd:
                ref_oprd = opcd['operand']
            res += self._X86_GenerateReference(opcd['reference'], ref_oprd)
            return res

        if 'cpu_model' in opcd or 'attr' in opcd or 'prefix' in opcd or 'suffix' in opcd:
            cond = self._X86_GenerateInstructionCondition(opcd)
            if cond == None:
                res += self._X86_GenerateInstructionBody(opcd)
            else:
                res += self._GenerateCondition('if', cond, self._X86_GenerateInstructionBody(opcd))
                res += self._GenerateCondition('else', None, 'return false;')
                return res

        res += self._X86_GenerateInstructionBody(opcd)
        return res

    def _X86_GenerateMultipleInstruction(self, opcd_arr):
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
                cond = self._X86_GenerateInstructionCondition(opcd)
                if cond_type == 'if' and cond == None:
                    if opcd != opcd_arr['sub_opcodes'][0]:
                        raise Exception('No condition for if\n%s', str(opcd))
                    cond_type = 'else'

                res += self._GenerateCondition(cond_type, cond, self._X86_GenerateInstructionBody(opcd))
                cond_type = 'else if'
            else:
                if cond_type == 'if':
                    raise Exception('No condition for if\n%s', str(opcd))
                res += self._GenerateCondition('else', None, self._X86_GenerateInstruction(opcd))
                return res

        res += 'return false;\n'
        return res

    def _X86_GenerateInstruction(self, opcd):
        if 'sub_opcodes' in opcd:
            return self._X86_GenerateMultipleInstruction(opcd)
        else:
            return self._X86_GenerateSingleInstruction(opcd)


    def _X86_GenerateInstructionBody(self, opcd):
        res = ''
        pfx_n = None

        if 'constraint' in opcd:
            if opcd['constraint'].startswith('pfx'):
                pfx_n = int(opcd['constraint'][-1])
            if opcd['constraint'] == 'd64' or opcd['constraint'] == 'df64':
                res += self._GenerateCondition('if', '!(rInsn.Prefix() & X86_Prefix_OpSize)', 'rInsn.Prefix() |= X86_Prefix_REX_w; /* d64/df64 constraint */\n')

        if 'invalid' in opcd:
            return 'return false; /* INVALID */\n'

        if pfx_n != None:
            res += 'rInsn.Length()++;\n'
            if 'mnemonic' in opcd:
                res += 'rInsn.Prefix() |= X86_Prefix_%s;\n' % opcd['mnemonic']
            res += 'return Disassemble(rBinStrm, Offset, rInsn, Mode);\n'
            return res

        if 'suffix' in opcd:
            res += 'rInsn.Length() += 2;\n'
        else:
            res += 'rInsn.Length()++;\n'
        if 'mnemonic' in opcd:
            res += 'rInsn.SetOpcode(X86_Opcode_%s);\n' % opcd['mnemonic'].capitalize()
            self.all_mnemo.add(opcd['mnemonic'].capitalize())

        conv_flags = { 'cf':'X86_FlCf', 'pf':'X86_FlPf', 'af':'X86_FlAf', 'zf':'X86_FlZf',
                'sf':'X86_FlSf', 'tf':'X86_FlTf', 'if':'X86_FlIf', 'df':'X86_FlDf', 'of':'X86_FlOf' }

        conv_optype = { 'jmp':'Instruction::JumpType', 'call':'Instruction::CallType', 'ret':'Instruction::ReturnType', 'cond':'Instruction::ConditionalType' }
        if 'operation_type' in opcd:
            res += 'rInsn.SubType() |= %s;\n' % ' | '.join(conv_optype[x] for x in opcd['operation_type'])

        if 'operand' in opcd:
            res += self._GenerateCondition(
                    'if',
                    '%s == false' % self._X86_GenerateOperandMethod(opcd['operand']),
                    'return false;\n')


        if 'semantic' in opcd:
            res += self._ConvertSemanticToCode(opcd, opcd['semantic'], self.id_mapper)
        else:
            res += self._ConvertSemanticToCode(opcd, None, self.id_mapper)
        res += 'return true;\n'
        return res


    def _X86_GenerateInstructionCondition(self, opcd):
        cond = []

        if 'cpu_model' in opcd:
            cond.append(' || '.join(['m_CfgMdl.GetEnum("Architecture") %s' %  x for x in opcd['cpu_model'].split(',')]))

        if 'attr' in opcd:
            for f in opcd['attr']:
                # Mode attr
                if f == 'm64':
                    cond.append('Mode == X86_Bit_64')
                elif f == 'nm64':
                    cond.append('Mode != X86_Bit_64')
                elif f == 'a64':
                    cond.append('Mode == X86_Bit_64 && !(rInsn.Prefix() & X86_Prefix_AdSize)')
                elif f == 'na16':
                    cond.append('(Mode == X86_Bit_16 && rInsn.Prefix() & X86_Prefix_AdSize) || (Mode != X86_Bit_16 && !(rInsn.Prefix() & X86_Prefix_AdSize))')
                elif f == 'ia64':
                    cond.append('m_CfgMdl.GetEnum("Vendor") == X86_ProcType_IA64')

                # Manufacturer
                elif f == 'amd':
                    cond.append('m_CfgMdl.GetEnum("Vendor") == X86_ProcType_AMD')
                elif f == 'intel':
                    cond.append('m_CfgMdl.GetEnum("Vendor") == X86_ProcType_INTEL')
                elif f == 'cyrix':
                    cond.append('m_CfgMdl.GetEnum("Vendor") == X86_ProcType_CYRIX')
                elif f == 'iit':
                    cond.append('m_CfgMdl.GetEnum("Vendor") == X86_ProcType_IIT')

                # Prefix
                elif f == 'op_size':
                    cond.append('(rInsn.GetPrefix() & X86_Prefix_OpSize)')
                elif f == 'ad_size':
                    cond.append('(rInsn.GetPrefix() & X86_Prefix_AdSize)')
                elif f == 'rep':
                    cond.append('(rInsn.GetPrefix() & X86_Prefix_Rep)')
                elif f == 'rexb':
                    cond.append('(rInsn.GetPrefix() & X86_Prefix_REX_b) == X86_Prefix_REX_b')
                elif f == 'rexw':
                    cond.append('(rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w')

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

    def _X86_GenerateOperandMethod(self, oprd):
        res = 'Operand__%s(rBinStrm, Offset, rInsn, Mode)' % '_'.join(oprd)
        self.all_oprd.add('_'.join(oprd))
        return res

    def _X86_GenerateReference(self, ref, oprd = None):
        res = ''
        tbl_off = ''

        if ref.startswith('table_FP'):
            tbl_off = '- 0xc0'

        if ref.startswith('table_'):
            res += self._GenerateRead('Opcode', 'Offset', 8)
            res += 'rInsn.Length()++;\n'
            res += self._GenerateCondition('if', 'Opcode + 1 > sizeof(m_%s)' % ref.capitalize(), 'return false;')
            res += 'return (this->*m_%s[Opcode%s])(rBinStrm, Offset + 1, rInsn, Mode);\n' % (ref.capitalize(), tbl_off)

        elif ref.startswith('group_'):
            grp = self.arch['instruction']['group'][ref]
            refs = []
            grp_no = 0
            for opcd_g in grp:
                case_statm = ''
                if type(opcd_g) == list:
                    case_statm += self._GenerateMultipleInstruction(opcd_g)

                elif oprd != None:
                    tmp_opcd = opcd_g
                    tmp_opcd['operand'] = oprd
                    case_statm += self._X86_GenerateInstruction(tmp_opcd)

                elif 'invalid' in opcd_g:
                    case_statm += 'return false;\n'

                else:
                    case_statm += self._X86_GenerateInstruction(opcd_g)

                # HACK: needed to generate scope for reading (prefix/suffix)
                if case_statm.find('Read') != -1:
                    case_statm = self._GenerateBrace(case_statm)

                refs.append( ('%#x' % grp_no, case_statm, False) )
                grp_no += 1

            res += self._GenerateRead('ModRmByte', 'Offset', 8)
            res += 'x86::ModRM ModRm(ModRmByte);\n'
            res += self._GenerateSwitch('ModRm.Reg()', refs, 'return false;\n')

        elif ref.startswith('fpu'):
            fpu_info = self.arch['instruction']['fpu'][ref]
            fpu_oprd = None
            if 'operand' in fpu_info:
                fpu_oprd = fpu_info['operand']
            fpu = self.arch['instruction']['fpu'][ref]
            res += self._GenerateRead('ModRmByte', 'Offset', 8)+\
                    self._GenerateCondition('if', 'ModRmByte < 0xc0',\
                        self._X86_GenerateReference(fpu_info['group'], fpu_oprd))+\
                    self._GenerateCondition('else', None,\
                        self._X86_GenerateReference(fpu_info['table']))

        return res


    def GenerateHeader(self):
        res = ''

        res += 'private:\n'
        res += Indent('typedef bool (%sArchitecture:: *TDisassembler)(BinaryStream const&, TOffset, Instruction&, u8);\n' % self.GetArchName())

        for name in sorted(self.arch['instruction']['table']):
            if 'FP' in name:  opcd_no = 0xc0
            else:             opcd_no = 0x00

            res += Indent('static const TDisassembler m_%s[%#x];\n' % (name.capitalize(), 0x100 - opcd_no))
            for opcd in self.arch['instruction']['table'][name]:
                res += Indent('%s;\n' % self._X86_GenerateMethodName(name, opcd_no, True))
                opcd_no += 1
            res += '\n'

        return res

    def GenerateSource(self):
        res = ''
        arch_name = self.GetArchName()

        for name in sorted(self.arch['instruction']['table']):
            if 'FP' in name:  opcd_no = 0xc0
            else:             opcd_no = 0x00

            res += 'const %sArchitecture::TDisassembler %sArchitecture::m_%s[%#x] =\n' % (arch_name, arch_name, name.capitalize(), 0x100 - opcd_no)
            res += '{\n'
            tbl_elm = []
            for opcd in self.arch['instruction']['table'][name]:
                tbl_elm.append(Indent('&%sArchitecture::%s_%02x\n' % (arch_name, name.capitalize(), opcd_no))[:-1])
                opcd_no += 1
            res += ',\n'.join(tbl_elm)
            res += '\n};\n\n'

        for name in sorted(self.arch['instruction']['table']):
            if 'FP' in name: opcd_no = 0xc0
            else:            opcd_no = 0x00

            for opcd in self.arch['instruction']['table'][name]:
                res += self._X86_GenerateInstructionComment(opcd)
                res += '%s\n' % self._X86_GenerateMethodName(name, opcd_no, False)
                res += self._GenerateBrace(Indent(self._X86_GenerateInstruction(opcd)))
                res += '\n'
                opcd_no += 1

        return res

    def GenerateOpcodeEnum(self):
        res = ',\n'.join('X86_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum X86Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x.lower() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.GetArchName(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''

        for oprd_name in self.arch['operand']:
            oprd_name = str(oprd_name)
            if oprd_name.startswith('decode_'):
                res += Indent('Expression::SPType %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);\n' % (oprd_name[0].upper() + oprd_name[1:]))
            else:
                res += Indent('bool Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);\n' % (oprd_name))
        return res

    def GenerateOperandCode(self):
        res = ''

        class OprdVisitor(ast.NodeVisitor):
            def __init__(self, parent):
                self.parent = parent
                ast.NodeVisitor.__init__(self)
                self.var_expr = []

            def generic_visit(self, node):
                print('generic:', type(node).__name__)
                assert(0)

            def visit_Module(self, node):
                res = ''
                for b in node.body:
                    res += self.visit(b)
                return res

            def visit_Call(self, node):
                func_name = self.visit(node.func)
                func_args = []

                for arg in node.args:
                    func_args.append(self.visit(arg))

                if func_name == 'move_decode_offset':
                    assert(len(func_args) == 1)
                    if func_args[0] == '1':
                        return '++Offset;'
                    elif func_args[0] == '-1':
                        return '--Offset;'
                    return 'Offset += %s;' % func_args[0]

                if func_name.startswith('const'):
                    assert(len(func_args) == 2)
                    return 'return Expr::MakeConst(%s, %s);' % tuple(func_args)

                if func_name.startswith('addr_'):
                    addr_type = func_name[5]
                    addr_size = None

                    seg = 'nullptr'
                    off = None

                    if len(func_args) == 1:
                        off = func_args[0]
                    elif len(func_args) == 2:
                        seg = func_args[0]
                        off = func_args[1]

                    addr_fmt = 'return Expr::MakeMem(%%d, %s, %s, true);' % (seg, off)
                    addr8  = addr_fmt % 8
                    addr16 = addr_fmt % 16
                    addr32 = addr_fmt % 32
                    addr64 = addr_fmt % 64

                    if addr_type == 'v':
                        return self.parent._GenerateSwitch('Mode', [
                            ('X86_Bit_16',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', addr32)+
                                self.parent._GenerateCondition('else', None, addr16),
                                False),
                            ('X86_Bit_64',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w', addr64),
                                False),
                            ('X86_Bit_32',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', addr16)+
                                self.parent._GenerateCondition('else', None, addr32),
                                False)],
                            'return nullptr;\n'
                            )

                    if addr_type == 'z':
                        return self.parent._GenerateSwitch('Mode', [
                            ('X86_Bit_16',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', addr32)+
                                self.parent._GenerateCondition('else', None, addr16),
                                False),
                            ('X86_Bit_32',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', addr16)+
                                self.parent._GenerateCondition('else', None, addr32),
                                False)],
                            'return nullptr;\n'
                            )

                    elif addr_type == 'b':
                        return addr8
                    elif addr_type == 'w':
                        return addr16
                    elif addr_type == 'd':
                        return addr32
                    elif addr_type == 'q':
                        return addr64

                    assert(0)

                if func_name == 'reg':
                    assert(len(func_args) == 1)
                    return 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[0]]

                if func_name == 'reg_v':
                    reg16 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[0]]
                    reg32 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[1]]
                    reg64 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[2]]

                    return self.parent._GenerateSwitch('Mode', [
                        ('X86_Bit_16',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', reg32)+
                            self.parent._GenerateCondition('else', None, reg16),
                            False),
                        ('X86_Bit_64',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w', reg64),
                            False),
                        ('X86_Bit_32',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', reg16)+
                            self.parent._GenerateCondition('else', None, reg32),
                            False)],
                        'return nullptr;\n'
                        )

                if func_name == 'reg_z':
                    reg16 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[0]]
                    reg32 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[1]]

                    return self.parent._GenerateSwitch('Mode', [
                        ('X86_Bit_16',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', reg32)+
                            self.parent._GenerateCondition('else', None, reg16),
                            False),
                        ('X86_Bit_32',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', reg16)+
                            self.parent._GenerateCondition('else', None, reg32),
                            False)],
                        'return nullptr;\n'
                        )

                if func_name == 'reg_d64_r':
                    reg16 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[0]]
                    reg32 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[1]]
                    reg64 = 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[2]]

                    return self.parent._GenerateSwitch('Mode', [
                        ('X86_Bit_16',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', reg32)+
                            self.parent._GenerateCondition('else', None, reg16),
                            False),
                        ('X86_Bit_64',
                            reg64,
                            False),
                        ('X86_Bit_32',
                            self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', reg16)+
                            self.parent._GenerateCondition('else', None, reg32),
                            False)],
                        'return nullptr;\n'
                        )


                if func_name == 'call':
                    assert(len(func_args) == 1)
                    return 'return __%s(rBinStrm, Offset, rInsn, Mode);' % func_args[0]

                if func_name.startswith('read_'):
                    read_type = func_name[5]

                    def __GenerateReadType(read_type):
                        read_body = ''
                        read_body += 'u%d Value;\n' % read_type
                        read_body += self.parent._GenerateCondition('if', '!rBinStrm.Read(Offset, Value)', 'return nullptr;')
                        read_body += 'Offset += sizeof(Value);\n'
                        read_body += 'rInsn.Length() += sizeof(Value);\n'
                        read_body += 'return Expr::MakeConst(%d, Value);\n' % read_type
                        return read_body
                    def __GenerateReadTypeSignExtend(read_type, sign_type):
                        read_body = ''
                        read_body += 'u%d Value;\n' % read_type
                        read_body += self.parent._GenerateCondition('if', '!rBinStrm.Read(Offset, Value)', 'return nullptr;')
                        read_body += 'Offset += sizeof(Value);\n'
                        read_body += 'rInsn.Length() += sizeof(Value);\n'
                        read_body += 'return Expr::MakeConst(%d, SignExtend<s%d, %d>(Value));\n' % (sign_type, sign_type, read_type)
                        return read_body

                    if read_type == 'b':
                        return __GenerateReadType(8)
                    if read_type == 'bsq':
                        return __GenerateReadTypeSignExtend(8, 64)
                    if read_type == 'w':
                        return __GenerateReadType(16)
                    if read_type == 'd':
                        return __GenerateReadType(32)
                    if read_type == 'q':
                        return __GenerateReadType(64)

                    if read_type == 'v':
                        return self.parent._GenerateSwitch('Mode', [
                            ('X86_Bit_16',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', __GenerateReadType(32))+
                                self.parent._GenerateCondition('else', None, __GenerateReadType(16)),
                                False),
                            ('X86_Bit_64',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w', __GenerateReadType(64)),
                                False),
                            ('X86_Bit_32',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', __GenerateReadType(16))+
                                self.parent._GenerateCondition('else', None, __GenerateReadType(32)),
                                False)],
                            'return nullptr;\n'
                            )

                    if read_type == 'z':
                        return self.parent._GenerateSwitch('Mode', [
                            ('X86_Bit_16',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', __GenerateReadType(32))+
                                self.parent._GenerateCondition('else', None, __GenerateReadType(16)),
                                False),
                            ('X86_Bit_64',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w', __GenerateReadTypeSignExtend(32, 64)),
                                False),
                            ('X86_Bit_32',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', __GenerateReadType(16))+
                                self.parent._GenerateCondition('else', None, __GenerateReadType(32)),
                                False)],
                            'return nullptr;\n'
                            )

                    if read_type == 'zsq':
                        return self.parent._GenerateSwitch('Mode', [
                            ('X86_Bit_16',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', __GenerateReadTypeSignExtend(32, 64))+
                                self.parent._GenerateCondition('else', None, __GenerateReadTypeSignExtend(16, 64)),
                                False),
                            ('X86_Bit_64',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_REX_w) == X86_Prefix_REX_w', __GenerateReadTypeSignExtend(32, 64)),
                                False),
                            ('X86_Bit_32',
                                self.parent._GenerateCondition('if', '(rInsn.GetPrefix() & X86_Prefix_OpSize) == X86_Prefix_OpSize', __GenerateReadTypeSignExtend(16, 64))+
                                self.parent._GenerateCondition('else', None, __GenerateReadTypeSignExtend(32, 64)),
                                False)],
                            'return nullptr;\n'
                            )

                    raise('Unknown read type: %s' % read_type)

                assert(0)

            def visit_Attribute(self, node):
                attr_name  = node.attr
                value_name = self.visit(node.value)

                assert(0)

            def visit_Name(self, node):
                node_name = node.id
                #print 'Name', node_name

                if node_name == 'const':
                    return node_name

                if node_name.startswith('reg'):
                    return node_name

                if node_name == 'call':
                    return node_name

                if node_name.startswith('read_'):
                    return node_name

                if node_name.startswith('addr_'):
                    return node_name

                if node_name == 'off':
                    return 'Offset'

                if node_name == 'move_decode_offset':
                    return node_name

                if node_name == 'pc':
                    return 'Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode), &m_CpuInfo)'

                if node_name.startswith('op'):
                    return node_name

                if node_name.startswith('decode_'):
                    return '%s(rBinStrm, Offset, rInsn, Mode)' % (node_name[0].upper() + node_name[1:])

                if node_name == 'next_operand':
                    return '(rInsn.GetLength() - LastLen)'

                if node_name == 'last_len':
                    return 'auto LastLen = rInsn.GetLength();'

                assert(0)

            def visit_Str(self, node):
                #print 'Str', node.s
                return node.s

            def visit_Num(self, node):
                return str(node.n)

            def visit_Assign(self, node):
                assert(len(node.targets) == 1)
                target_name = self.visit(node.targets[0])
                value_name  = self.visit(node.value)

                if target_name.startswith('op'):
                    oprd_name = 'spOprd%d' % int(target_name[2:])
                    res = ''
                    res += 'auto %s = %s;\n' % (oprd_name, value_name)
                    res += self.parent._GenerateCondition('if', '%s == nullptr' % oprd_name, 'return false;')
                    return res

                assert(0)

            def visit_AugAssign(self, node):
                oper_name   = self.visit(node.op)
                target_name = self.visit(node.target)
                value_name  = self.visit(node.value)
                print 'AugAssign', oprd_name, target_name, value_name
                assert(0)

            def visit_BinOp(self, node):
                op = self.visit(node.op)
                left = self.visit(node.left)
                right = self.visit(node.right)

                def __GenerateScope(body):
                    return '[&]() { %s }()' % body

                res = ''
                res += 'auto spLeftOp = %s;\n' % __GenerateScope(left)
                res += 'auto spRightOp = %s;\n' % __GenerateScope(right)
                res += self.parent._GenerateCondition('if', 'spLeftOp == nullptr || spRightOp == nullptr', 'return nullptr;')
                res += 'return Expr::MakeBinOp(%s, spLeftOp, spRightOp);' % op
                return res

            def visit_Add(self, node):
                return 'OperationExpression::OpAdd'

            def visit_Expr(self, node):
                return self.visit(node.value)

        for oprd in sorted(self.arch['operand'].items(), key = lambda x: x[0]):
            oprd_name = str(oprd[0])
            oprd_code = oprd[1]

            is_decoder = False

            if oprd_name.startswith('decode_'):
                is_decoder = True

            if is_decoder:
                res += '/* decoder %s */\n' % oprd_code
                res += 'Expression::SPType %sArchitecture::%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n' % (self.GetArchName(), oprd_name[0].upper() + oprd_name[1:])
            else:
                res += '/* operand %s */\n' % oprd_code
                res += 'bool %sArchitecture::Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n' % (self.GetArchName(), oprd_name)

            v = OprdVisitor(self)
            oprd_no = 0
            body = ''
            for oprd_sem in oprd_code:
                nodes = ast.parse(oprd_sem)
                s = v.visit(nodes)
                body += '// operand%d: %s\n%s' % (oprd_no, oprd_sem, s)

                if not is_decoder and 'Oprd' in s:
                    body += 'rInsn.AddOperand(spOprd%d);\n' % oprd_no

                body += '\n'

                if 'Oprd' in s:
                    oprd_no += 1

            #print oprd_name, oprd_code

            if not is_decoder:
                body += 'return true;\n'
            res += self._GenerateBrace(body) + '\n'

        return res
