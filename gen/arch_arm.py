from arch import ArchConvertion
from helper import *

import re
import compiler
import ast
import string

class ArmArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()

        self.id_mapper = {
        'r0':'ARM_Reg0','r1':'ARM_Reg1','r2':'ARM_Reg2','r3':'ARM_Reg3',
        'r4':'ARM_Reg4','r5':'ARM_Reg5','r6':'ARM_Reg6','r7':'ARM_Reg7',
        'r8':'ARM_Reg8','r9':'ARM_Reg9','r10':'ARM_Reg10','r11':'ARM_Reg11',
        'r12':'ARM_Reg12','r13':'ARM_Reg13','r14':'ARM_Reg14','r15':'ARM_Reg15',
        'sp':'ARM_RegSP', 'lr':'ARM_RegLR', 'pc':'ARM_RegPC',
        }

        all_instructions = self.arch['instruction']

        self.arm_insns = []
        self.thumb_insns = []

        for insn in all_instructions:

            # We need to flatten the encoding array
            insn['encoding'] = compiler.ast.flatten(insn['encoding'])

            # Check if instruction is valid
            self._ARM_VerifyInstruction(insn)

            # Gather all mnemonics
            self.all_mnemo.add(self._ARM_GetMnemonic(insn).capitalize())

            if insn['mode'][0] == 'A':
                self.arm_insns.append(insn)
            elif insn['mode'][0] == 'T':
                self.thumb_insns.append(insn)

        self.arm_insns.sort(key=lambda insn: insn['encoding'])
        self.thumb_insns.sort(key=lambda insn: insn['encoding'])

    def _ARM_VerifyInstruction(self, insn):
        enc = insn['encoding']
        if len(enc) != 16 and len(enc) != 32:
            raise Exception('Invalid instruction "%s", encoding: %s, length: %d' % (insn['format'], insn['encoding'], len(insn['encoding'])))

    def _ARM_Mangle(insn):
        encoding = []
        for bit in insn['encoding']:
            if type(bit) == int:
                encoding.append(str(bit))
            elif bit[0] == '(':
                encoding.append(bit[1])
            else:
                encoding.append(bit)
        return '%s_%s' % (insn['mode'], '_'.join(encoding))

    def _ARM_GetMnemonic(self, insn):
        fmt = insn['format']
        res = ''
        for c in fmt:
            if not c in string.ascii_letters+string.digits:
                break
            res += c
        return res

    def _ARM_GetMask(self, insn):
        enc = insn['encoding']
        res = 0x0
        off = 0x0
        for bit in enc[::-1]:
            if bit in [ 0, 1, '(0)', '(1)' ]:
                res |= (1 << off)
            off += 1
        return res

    def _ARM_GetValue(self, insn):
        enc = insn['encoding']
        res = 0x0
        off = 0x0
        for bit in enc[::-1]:
            if bit in [ 1, '(1)' ]:
                res |= (1 << off)
            off += 1
        return res

    def _ARM_GetSize(self, insn):
        return len(insn['encoding'])

    def _ARM_ExtractField(self, insn):
        fmt = insn['format']
        fields = []

        off = fmt.find(' ')
        if off == -1:
            off = len(fmt)

        full_mnem = fmt[:off]
        all_oprds = fmt[off+1:]

        # mnem{field}
        beg = full_mnem.find('{')
        end = full_mnem.find('}')
        if beg != -1 and end != -1:
            fields.append(full_mnem[beg+1:end])
            full_mnem[end+1:]

        # mnem<field0><field1>
        # TODO handle .field
        while len(full_mnem):
            beg = full_mnem.find('<')
            end = full_mnem.find('>')

            if beg == -1:
                break
            if end == -1:
                raise Exception('Unclose field %s' % insn)
            fields.append(full_mnem[beg+1:end])
            full_mnem = full_mnem[end+1:]

        oprds = re.findall('\[[^\]]*\]|\([^\)]*\)|\"[^\"]*\"|\S+', all_oprds)
        e = len(oprds)
        i = 0
        while i < e:
            # TODO: do this in the regexp...
            oprd = oprds[i].strip(' ,')

            if len(oprd) == 0:
                i += 1
                continue

            if oprd.endswith('{!}'):
                fields.append('{!}')
                oprd = oprd[:-3]
                if len(oprd) == 0:
                    i += 1
                    continue

            if oprd == '!':
                fields.append('!')
            elif oprd.startswith('#<') and oprd[-1] == '>':
                fields.append(oprd[2:-1])
            elif oprd[0] == '<' and oprd[-1] == '>':
                fields.append(oprd[1:-1])
            elif oprd[-1] == '{':
                fields.append(oprds[i].strip() + ',' + oprds[i+1].strip())
            elif oprd[0] == '[' and not ']' in oprd:
                fields.append(oprds[i].strip() + ',' + oprds[i+1].strip())
            else:
                fields.append(oprd)
            i += 1

        return fields

    def _ARM_ExtractBits(self, insn, pattern):
        res = [] # beg, end
        enc = insn['encoding']
        beg = 0
        end = 0
        off = 0

        found = False
        for bitfield in enc[::-1]:
            if bitfield == pattern and not found:
                beg = off
                found = True
            elif bitfield != pattern and found:
                end = off - 1
                found = False
                res.append((beg, end))
            off += 1

        if end == 0 and enc[0] == pattern:
            end = len(enc) - 1
            res.append((beg, end))

        return res

    def _ARM_GenerateExtractBits(self, insn, pattern, scale = 0):
        bits = self._ARM_ExtractBits(insn, pattern)
        res = []

        zx_bit = 0
        off = 0
        for beg, end in bits:

            scale_str = ''
            if zx_bit != 0:
                scale_str = ' << %d' % zx_bit

            if beg == end:
                res.append('ExtractBit<%d>(Opcode)%s' % (beg, scale_str))
            else:
                res.append('ExtractBits<%d, %d>(Opcode)%s' % (beg, end, scale_str))

            zx_bit += end - beg + 1

        scale_str = ''
        if scale != 0:
            scale_str = ' << %d' % scale_str
        return '(%s)%s' % (' | '.join(res), scale_str)

    def _ARM_GenerateExtractBitsSigned(self, insn, pattern, scale = 0):
        bits = self._ARM_ExtractBits(insn, pattern)
        res = []

        sx_bit = 0
        off = 0
        for beg, end in bits:

            scale_str = ''
            if sx_bit != 0:
                scale_str = ' << %d' % sx_bit

            if beg == end:
                res.append('ExtractBit<%d>(Opcode)%s' % (beg, scale_str))
            else:
                res.append('ExtractBits<%d, %d>(Opcode)%s' % (beg, end, scale_str))

            sx_bit += end - beg + 1

        scale_str = ''
        if scale != 0:
            scale_str = ' << %d' % scale_str
        return 'SignExtend<s64, %d>(%s)%s' % (sx_bit, ' | '.join(res), scale_str)

    def _ARM_GenerateInstruction(self, insn):
        res = ''
        res += 'rInsn.SetName("%s");\n' % self._ARM_GetMnemonic(insn)
        res += 'rInsn.SetOpcode(ARM_Opcode_%s);\n' % self._ARM_GetMnemonic(insn).capitalize()
        res += 'rInsn.Length() += %d;\n' % (self._ARM_GetSize(insn) / 8)

        ## Handle SubType (jmp / call)
        map_op_type = { 'jmp' : 'Instruction::JumpType', 'call' : 'Instruction::CallType' }
        attrs = []
        for attr in insn['attribute']:
            if attr in map_op_type:
                attrs.append(map_op_type[attr])

        if len(attrs):
            res += 'rInsn.SubType() |= %s;\n' % ' | '.join(attrs)

        field_mapper = { 'c': ('c', 'rInsn.SetTestedFlags(%s);\n') }
        insn_fields = self._ARM_ExtractField(insn)

        oprd_handler = self.arch['operand']

        oprd_no = 0
        for field in insn_fields:

            # {!} is changed to 'W' (write_back) since '!' is not a valid yaml label
            if field == '{!}':
                field = 'W'

            res += '\n// field: %s\n' % field

            # Test condition bits
            if field == 'c':
                if not 'c' in insn['encoding']:
                    res += '// TODO: unable to find conditional bits\n'
                    continue
                res += Indent('u8 CondField = %s;\n' % self._ARM_GenerateExtractBits(insn, 'c'), 0)
                res += Indent('rInsn.SetTestedFlags(CondField);\n', 0)
                res += Indent(self._GenerateCondition('if', 'CondField != 0xe',
                        'rInsn.SubType() |= Instruction::ConditionalType;'), 0)

            elif field == '!':
                res += 'rInsn.Prefix() |= ARM_Prefix_W;\n'

            elif field == 'W':
                if not 'W' in insn['encoding']:
                    res += Indent(self._GenerateCondition('if', '%s != 15' % self._ARM_GenerateExtractBits(insn, 'm'),
                        'rInsn.Prefix() |= ARM_Prefix_W;'), 0)
                else:
                    res += Indent(self._GenerateCondition('if', self._ARM_GenerateExtractBits(insn, 'W'),
                        'rInsn.Prefix() |= ARM_Prefix_W;'), 0)

            elif field == 'S':
                res += Indent(self._GenerateCondition('if', self._ARM_GenerateExtractBits(insn, 'S'),
                    'rInsn.Prefix() |= ARM_Prefix_S;'), 0)

            elif field[0] == '#' and field[1:].isdigit():
                res += 'auto spOprd%d = Expr::MakeConst(32, %s);\n' % (oprd_no, field[1:])
                res += self._GenerateCondition('if', 'spOprd%d == nullptr' % oprd_no, 'return false;')
                res += 'rInsn.AddOperand(spOprd%d);\n' % oprd_no
                oprd_no += 1

            elif field in oprd_handler:
                class OprdVisitor(ast.NodeVisitor):
                    def __init__(self, parent, insn):
                        self.parent = parent
                        self.insn = insn
                        ast.NodeVisitor.__init__(self)
                        self.var_expr = []

                    def generic_visit(self, node):
                        raise Exception('generic:', type(node).__name__)

                    def visit_Module(self, node):
                        res = ''
                        for b in node.body:
                            res += self.visit(b)
                        return res

                    def visit_Call(self, node):
                        res = ''
                        func_name = self.visit(node.func)
                        func_args = []

                        for arg in node.args:
                            func_args.append(self.visit(arg))

                        if func_name == 'id':
                            assert(len(func_args) == 1)
                            id_expr = 'Expr::MakeId(%s, &m_CpuInfo)' % func_args[0]
                            res += id_expr
                            self.var_expr.append(id_expr)
                            return res

                        if func_name in [ 'arm_expand_imm', 'arm_expand_imm_c' ]:
                            imm_expr = 'Expr::MakeConst(32, UnsignedRotateRight(ExtractBits<0, 7>(Opcode), ExtractBits<8, 11>(Opcode) << 1))'
                            res += imm_expr
                            self.var_expr.append(imm_expr)
                            return res

                        if func_name == 'imm':
                            assert(len(func_args) == 1)
                            imm_expr = 'Expr::MakeConst(32, %s)' % self.parent._ARM_GenerateExtractBits(self.insn, func_args[0])
                            res += imm_expr
                            self.var_expr.append(imm_expr)
                            return res

                        if func_name == 'disp':
                            assert(len(func_args) == 1)
                            imm_expr = 'Expr::MakeConst(32, %s)' % self.parent._ARM_GenerateExtractBitsSigned(self.insn, func_args[0])
                            res += imm_expr
                            self.var_expr.append(imm_expr)
                            return res

                        if func_name == 'arm_branch':
                            assert(len(func_args) == 1)
                            branch_expr = 'Expr::MakeConst(32, %s << 2)' % self.parent._ARM_GenerateExtractBitsSigned(self.insn, func_args[0])
                            res += branch_expr
                            self.var_expr.append(branch_expr)
                            return res

                        if func_name == 'thumb_branch':
                            assert(len(func_args) == 1)
                            branch_expr = 'Expr::MakeConst(32, %s << 1)' % self.parent._ARM_GenerateExtractBitsSigned(self.insn, func_args[0])
                            res += branch_expr
                            self.var_expr.append(branch_expr)
                            return res

                        if func_name == 'reg':
                            assert(len(func_args) == 1)
                            reg_name = func_args[0].capitalize()
                            self.var_expr.append('u32 Reg%s = %s;\n' % (reg_name, self.parent._ARM_GenerateExtractBits(self.insn, func_args[0])))
                            if 'could_jmp' in self.insn['attribute'] and reg_name[0].lower() in [ 'd', 't' ]:
                                self.var_expr.append(self.parent._GenerateCondition('if', 'Reg%s + 1 == ARM_RegPC' % reg_name, 'rInsn.SubType() |= Instruction::JumpType;'))
                            if 'could_ret' in self.insn['attribute'] and reg_name[0].lower() in [ 'd', 't' ]:
                                self.var_expr.append(self.parent._GenerateCondition('if', 'Reg%s + 1 == ARM_RegPC' % reg_name, 'rInsn.SubType() |= Instruction::ReturnType;'))
                            reg_expr = 'Expr::MakeId(Reg%s + 1, &m_CpuInfo)' % reg_name
                            self.var_expr.append(reg_expr)
                            return reg_expr

                        if func_name == 'reg_list':
                            assert(len(func_args) == 1)
                            self.var_expr.append('u32 RegList = %s;\n' % self.parent._ARM_GenerateExtractBits(self.insn, func_args[0]))
                            self.var_expr.append('Expression::LSPType IdExprs;\n')
                            self.var_expr.append('std::vector<u32> VecId;\n')
                            self.var_expr.append('for (u8 RegIdx = 0; RegIdx < 16; ++RegIdx)\n')
                            self.var_expr.append('{\n')
                            if_stmt = [ 'VecId.push_back(RegIdx + 1);' ]
                            if 'could_jmp' in self.insn['attribute'] and func_args[0][0] in [ 'd', 'r' ]:
                                if_stmt.append(self.parent._GenerateCondition('if', 'RegIdx + 1 == ARM_RegPC', 'rInsn.SubType() |= Instruction::JumpType;'))
                            if 'could_ret' in self.insn['attribute'] and func_args[0][0] in [ 'd', 'r' ]:
                                if_stmt.append(self.parent._GenerateCondition('if', 'RegIdx + 1 == ARM_RegPC', 'rInsn.SubType() |= Instruction::ReturnType;'))
                            self.var_expr.append(Indent(self.parent._GenerateCondition('if', 'RegList & (1 << RegIdx)', '\n'.join(if_stmt))))
                            self.var_expr.append('}\n')
                            self.var_expr.append('Expr::MakeVecId(VecId, &m_CpuInfo);\n')
                            res += self.var_expr[-1]
                            return res

                        if func_name == 'mem':
                            assert(len(func_args) == 1)
                            mem_expr = 'Expr::MakeMem(%d, %s, %s, %s)' %\
                            (32, 'nullptr', func_args[0], 'true')
                            self.var_expr[-1] = mem_expr
                            return mem_expr

                        if func_name == 'u_add_sub':
                            assert(len(func_args) == 2)

                            left = func_args[0]
                            right = func_args[1]
                            self.var_expr.pop()
                            self.var_expr.pop()

                            self.var_expr.append('auto OpType = (%s) ? OperationExpression::OpAdd : OperationExpression::OpSub;\n' % self.parent._ARM_GenerateExtractBits(insn, 'U'))
                            op_expr = 'Expr::MakeBinOp(OpType,\n%s,\n%s)' % (Indent(left), Indent(right))
                            self.var_expr.append(op_expr)
                            return op_expr

                        raise Exception('call %s' % func_name)

                    def visit_Attribute(self, node):
                        attr_name  = node.attr
                        value_name = self.visit(node.value)

                        raise Exception('attr %s' % attr_name)

                    def visit_Name(self, node):
                        node_name = node.id

                        if node_name in [ 'id', 'imm', 'disp', 'reg', 'reg_list', 'mem', 'u_add_sub' ]:
                            return node_name

                        if node_name.endswith('_branch'):
                            return node_name

                        if node_name.startswith('check_'):
                            return node_name

                        if node_name == 'sp':
                            return 'ARM_RegSP'
                        if node_name == 'pc':
                            return 'ARM_RegPC'

                        if node_name.startswith('arm_'):
                            return node_name

                        raise Exception('name: %s' % node_name)

                    def visit_Str(self, node):
                        return node.s

                    def visit_Num(self, node):
                        return str(node.n)

                    def visit_Assign(self, node):
                        assert(len(node.targets) == 1)
                        target_name = self.visit(node.targets[0])
                        value_name  = self.visit(node.value)

                        raise Exception('assign %s' % target_name)

                    def visit_AugAssign(self, node):
                        oper_name   = self.visit(node.op)
                        target_name = self.visit(node.target)
                        value_name  = self.visit(node.value)
                        raise Exception('AugAssign', oprd_name, target_name, value_name)

                    def visit_BinOp(self, node):
                        op = self.visit(node.op)
                        left = self.visit(node.left)
                        self.var_expr.pop()
                        right = self.visit(node.right)
                        self.var_expr.pop()

                        op_expr = 'Expr::MakeBinOp(%s,\n%s,\n%s)' % (op, Indent(left), Indent(right))
                        self.var_expr.append(op_expr)
                        return op_expr

                    def visit_Add(self, node):
                        return 'OperationExpression::OpAdd'

                    def visit_Expr(self, node):
                        return self.visit(node.value)

                v = OprdVisitor(self, insn)
                oprd_code = oprd_handler[field]
                for oprd_sem in oprd_code:
                    nodes = ast.parse(oprd_sem)
                    v.visit(nodes)

                    for expr in v.var_expr[:-1]:
                        res += expr
                res += 'auto spOprd%s = %s;\n' % (oprd_no, v.var_expr[-1])
                res += self._GenerateCondition('if', 'spOprd%d == nullptr' % oprd_no, 'return false;')
                res += 'rInsn.AddOperand(spOprd%d);\n' % oprd_no

                oprd_no += 1

            else:
                res += '/* unhandled field %s */\n' % field


        if 'semantic' in insn:
            res += self._ConvertSemanticToCode(insn, insn['semantic'], self.id_mapper)

        res += 'return true;\n'

        return self._ARM_GenerateMethodPrototype(insn, False) + '\n' + self._GenerateBrace(res)

    def _ARM_GenerateInstructionComment(self, insn):
        return '// %s - %s - %s\n' % (insn['format'], insn['attribute'], insn['encoding'])

    def _ARM_GenerateMethodName(self, insn):
        mnem  = self._ARM_GetMnemonic(insn)
        mode  = insn['mode']
        mask  = self._ARM_GetMask(insn)
        value = self._ARM_GetValue(insn)
        return 'Instruction_%s_%s_%08x_%08x' % (mnem, mode, mask, value)

    def _ARM_GenerateMethodPrototype(self, insn, in_class = False):
        mnem = self._ARM_GetMnemonic(insn)
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)' % self.GetArchName()

        return meth_fmt % self._ARM_GenerateMethodName(insn)

    def GenerateHeader(self):
        res = ''
        res += 'static char const *m_Mnemonic[%#x];\n' % (len(self.all_mnemo) + 1)

        for insn in sorted(self.arch['instruction'], key=lambda a:self._ARM_GetMnemonic(a)):
            res += self._ARM_GenerateMethodPrototype(insn, True) + ';\n'

        res += 'bool DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n'
        res += 'bool DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n'

        return res

    def GenerateSource(self):
        res = ''

        res += 'bool ArmArchitecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n'
        res += self._GenerateBrace(
                'rInsn.GetData()->ArchitectureTag() = GetTag();\n'+
                'rInsn.Mode() = Mode;\n\n'+
                self._GenerateSwitch('Mode',
                    [('ARM_ModeArm',   'return DisassembleArm(rBinStrm, Offset, rInsn);\n',   False),
                     ('ARM_ModeThumb', 'return DisassembleThumb(rBinStrm, Offset, rInsn);\n', False)],
                    'return false;\n')
                )

        def _ARM_GenerateDispatcher(arm, insns):
            res = ''
            insns_dict = {}

            # regroup instructions with the same mask
            for insn in insns:
                mask = arm._ARM_GetMask(insn)
                if not mask in insns_dict:
                    insns_dict[mask] = []
                insns_dict[mask].append(insn)

            # order by number of 0 and 1 in order to handle alias correctly
            def get_bit_count(elem):
                return 32 - bin(elem[0]).count('1')
            insns_list = sorted(insns_dict.items(), key=get_bit_count)

            for mask, insn_list in insns_list:
                bit = len(insn_list[0]['encoding'])
                if len(insn_list) == 1:
                    value = arm._ARM_GetValue(insn_list[0])
                    res += arm._GenerateCondition('if', '(Opcode%d & %#010x) == %#010x' % (bit, mask, value), self._ARM_GenerateInstructionComment(insn_list[0]) + 'return %s(rBinStrm, Offset, Opcode%d, rInsn);' % (arm._ARM_GenerateMethodName(insn_list[0]), bit))
                else:
                    cases = []
                    for insn in insn_list:
                        value = arm._ARM_GetValue(insn)
                        cases.append( ('%#010x' % value, self._ARM_GenerateInstructionComment(insn) + 'return %s(rBinStrm, Offset, Opcode%d, rInsn);\n' % (arm._ARM_GenerateMethodName(insn), bit), False) )
                    res += arm._GenerateSwitch('Opcode%d & %#010x' % (bit, mask), cases, 'break;\n')

            return res

        res += 'bool ArmArchitecture::DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res += self._GenerateBrace(
                self._GenerateRead('Opcode32', 'Offset', 32)+
                _ARM_GenerateDispatcher(self, self.arm_insns)+
                'return false;\n'
                )

        res += 'bool ArmArchitecture::DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res += self._GenerateBrace(
                self._GenerateRead('Opcode16Low', 'Offset & ~1', 16)+
                self._GenerateRead('Opcode16High', '(Offset + 2) & ~1', 16)+
                Indent('u16 Opcode16 = Opcode16Low;\n', 0)+
                Indent('u32 Opcode32 = ((Opcode16Low << 16) | Opcode16High);\n', 0)+
                _ARM_GenerateDispatcher(self, self.thumb_insns)+
                'return false;\n'
                )

        for insn in self.arm_insns + self.thumb_insns:
            res += self._ARM_GenerateInstructionComment(insn)
            res += self._ARM_GenerateInstruction(insn)

        return res

    def GenerateOpcodeEnum(self):
        res = ',\n'.join('ARM_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum ARM_Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x.lower() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.GetArchName(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''
        return res

    def GenerateOperandCode(self):
        res = ''
        return res
