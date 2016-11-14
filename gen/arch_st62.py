from arch import ArchConvertion
from helper import *

import ast

class St62ArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()
        self.all_oprd = set()
        self.all_dec = set()

        self.id_mapper = {
        'cf':'ST62_Flg_C',   'zf':'ST62_Flg_Z',
        'A':'ST62_Reg_A',   'X':'ST62_Reg_X',   'Y':'ST62_Reg_Y',
        'V':'ST62_Reg_V',   'W':'ST62_Reg_W',
        'pc':'ST62_Reg_Pc',
        'stk0':'ST62_Reg_Stk0', 'stk1':'ST62_Reg_Stk1',
        'stk2':'ST62_Reg_Stk2', 'stk3':'ST62_Reg_Stk3',
        'stk4':'ST62_Reg_Stk4', 'stk5':'ST62_Reg_Stk5',
        }

        self.id_mem_mapper = {
        'A':'0xFF',   'X':'0x80',   'Y':'0x81',
        'V':'0x82',   'W':'0x83',
        }


    def _ST62_GenerateOperandMethod(self, oprd):
        res = 'Operand__%s(rBinStrm, Offset, rInsn, Mode)' % oprd
        self.all_oprd.add('_'.join(oprd))
        return res

    def GenerateHeader(self):
        res = ''

        res += 'private:\n'
        res += Indent('typedef bool (%sArchitecture:: *TDisassembler)(BinaryStream const&, OffsetType, Instruction&, u8);\n' % self.GetArchName())

        for name in sorted(self.arch['instruction']['table']):
            opcd_no = 0x00
            res += Indent('static const TDisassembler m_%s[%#x];\n' % (name.capitalize(), 0x10 - opcd_no))
            for opcd in self.arch['instruction']['table'][name]:
                res += Indent('%s;\n' % self._ST62_GenerateMethodName(name, opcd_no, True))
                opcd_no += 1
            res += '\n'

        return res

    def GenerateSource(self):
        res = ''
        arch_name = self.GetArchName()

        for name in sorted(self.arch['instruction']['table']):
            opcd_no = 0x00

            res += 'const %sArchitecture::TDisassembler %sArchitecture::m_%s[%#x] =\n' % (arch_name, arch_name, name.capitalize(), 0x10 - opcd_no)
            res += '{\n'
            tbl_elm = []
            for opcd in self.arch['instruction']['table'][name]:
                tbl_elm.append(Indent('&%sArchitecture::%s_%02x\n' % (arch_name, name.capitalize(), opcd_no))[:-1])
                opcd_no += 1
            res += ',\n'.join(tbl_elm)
            res += '\n};\n\n'

        for name in sorted(self.arch['instruction']['table']):
            opcd_no = 0x00

            for opcd in self.arch['instruction']['table'][name]:
                res += self._ST62_GenerateInstructionComment(opcd)
                res += '%s\n' % self._ST62_GenerateMethodName(name, opcd_no, False)
                res += self._GenerateBrace(Indent(self._ST62_GenerateInstruction(opcd)))
                res += '\n'
                opcd_no += 1

        return res

    # Architecture dependant methods
    def _ST62_GenerateMethodName(self, type_name, opcd_no, in_class = False):
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, OffsetType Offset, Instruction& rInsn, u8 Mode)' % self.GetArchName()

        if opcd_no == None:
            return meth_fmt % 'Invalid'

        return meth_fmt % ('%s_%02x' % (type_name.capitalize(), opcd_no))


    def _ST62_GenerateReference(self, ref, oprd = None):
        res = ''
        tbl_off = ''

        if ref.startswith('table_'):
            res += self._GenerateRead('High', 'Offset', 8)
            res += 'High >>= 4;\n'
            res += self._GenerateCondition('if', 'High + 1 > sizeof(m_%s)' % ref.capitalize(), 'return false;')
            res += 'return (this->*m_%s[High%s])(rBinStrm, Offset, rInsn, Mode);\n' % (ref.capitalize(), tbl_off)

        return res

    def _ST62_GenerateInstruction(self, opcd):
        res = ''
        pfx_n = None

        if 'reference' in opcd:
            ref_oprd = None
            if 'operand' in opcd:
                ref_oprd = opcd['operand']
            res += self._ST62_GenerateReference(opcd['reference'], ref_oprd)
            return res

        if 'invalid' in opcd:
            return 'return false; /* INVALID */\n'

        res += 'rInsn.Size()++;\n'

        if 'alternate' in opcd:
            res += self._GenerateRead('Value', 'Offset', 8)
            res += 'if (Value & 0x10) {\n'
            res += '  rInsn.SetOpcode(ST62_Opcode_%s);\n' % opcd['alternate'].capitalize()
            res += '} else {\n'
            res += '  rInsn.SetOpcode(ST62_Opcode_%s);\n' % opcd['mnemonic'].capitalize()
            res += '}\n'
            self.all_mnemo.add(opcd['alternate'].capitalize())
            self.all_mnemo.add(opcd['mnemonic'].capitalize())
        elif 'mnemonic' in opcd:
            res += 'rInsn.SetOpcode(ST62_Opcode_%s);\n' % opcd['mnemonic'].capitalize()
            self.all_mnemo.add(opcd['mnemonic'].capitalize())

        conv_optype = { 'jmp':'Instruction::JumpType', 'call':'Instruction::CallType', 'ret':'Instruction::ReturnType', 'cond':'Instruction::ConditionalType' }
        if 'operation_type' in opcd:
            res += 'rInsn.SubType() |= %s;\n' % ' | '.join(conv_optype[x] for x in opcd['operation_type'])

        if 'operand' in opcd:
            for oprd in opcd['operand']:
                res += self._GenerateCondition(
                    'if',
                    '%s == false' % self._ST62_GenerateOperandMethod(oprd),
                    'return false;\n')

        if 'semantic' in opcd:
            if 'semantic_alt' in opcd:
                res += 'if (Value & 0x10)\n'
                res += self._ConvertSemanticToCode(opcd, opcd['semantic_alt'], self.id_mapper)
                res += 'else\n'
            res += self._ConvertSemanticToCode(opcd, opcd['semantic'], self.id_mapper)

        else:
            res += self._ConvertSemanticToCode(opcd, None, self.id_mapper)
        res += 'return true;\n'
        return res

    def _ST62_GenerateInstructionComment(self, opcd_info):
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


    def GenerateOpcodeEnum(self):
        res = ',\n'.join('ST62_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum ST62Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x.lower() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.GetArchName(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''

        for oprd_name in self.arch['operand']:
            oprd_name = str(oprd_name)
            if oprd_name.startswith('decode_'):
                res += Indent('Expression::SPType %s(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);\n' % (oprd_name[0].upper() + oprd_name[1:]))
            else:
                res += Indent('bool Operand__%s(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode);\n' % (oprd_name))
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

            def visit_Name(self, node):
                node_name = node.id
                #print 'Name', node_name

                if node_name == 'const':
                    return node_name

                if node_name.startswith('reg'):
                    return node_name

                if node_name.startswith('ireg'):
                    return node_name

                if node_name == 'pc':
                    return 'Expr::MakeId(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, Mode), &m_CpuInfo)'

                if node_name.startswith('op'):
                    return node_name

                if node_name.startswith('decode_'):
                    return '%s(rBinStrm, Offset, rInsn, Mode)' % (node_name[0].upper() + node_name[1:])

                if node_name == 'call':
                    return node_name

                print "Name: %s" % node_name
                assert(0)

            def visit_Num(self, node):
                return str(node.n)

            def visit_Str(self, node):
                #print 'Str', node.s
                return node.s

            def visit_Expr(self, node):
                return self.visit(node.value)

            def visit_Call(self, node):
                func_name = self.visit(node.func)
                func_args = []

                #print "CALL: %s" % func_name
                for arg in node.args:
                    func_args.append(self.visit(arg))

                if func_name == 'reg':
                    assert(len(func_args) == 1)
                    return 'return Expr::MakeId(%s, &m_CpuInfo);' % self.parent.id_mapper[func_args[0]]

                if func_name == 'ireg':
                    assert(len(func_args) == 1)
                    return 'return Expr::MakeMem(8, Expr::MakeBitVector(16, 0x1000), Expr::MakeId(%s, &m_CpuInfo), true);' % self.parent.id_mapper[func_args[0]]

                if func_name.startswith('const'):
                    assert(len(func_args) == 2)
                    return 'return Expr::MakeBitVector(%s, %s);' % tuple(func_args)

                if func_name == 'call':
                    assert(len(func_args) == 1)
                    return 'return __%s(rBinStrm, Offset, rInsn, Mode);' % func_args[0]

                print('Unknown func name: %s' % func_name)
                assert(0)


        for oprd in sorted(self.arch['operand'].items(), key = lambda x: x[0]):
            oprd_name = str(oprd[0])
            oprd_code = oprd[1]

            is_decoder = False

            if oprd_name.startswith('decode_'):
                is_decoder = True

            if is_decoder:
                res += '/* decoder %s */\n' % oprd_code
                res += 'Expression::SPType %sArchitecture::%s(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)\n' % (self.GetArchName(), oprd_name[0].upper() + oprd_name[1:])
            else:
                res += '/* operand %s */\n' % oprd_code
                res += 'bool %sArchitecture::Operand__%s(BinaryStream const& rBinStrm, OffsetType& Offset, Instruction& rInsn, u8 Mode)\n' % (self.GetArchName(), oprd_name)

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
