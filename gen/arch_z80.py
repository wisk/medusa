from arch import ArchConvertion
from helper import *

import ast

class Z80ArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.id_mapper = {
        'cf':'Z80_Flg_C',  'zf':'Z80_Flg_Z',
        'a':'Z80_Reg_A',   'b':'Z80_Reg_B',   'c':'Z80_Reg_C',   'd':'Z80_Reg_D',
        'e':'Z80_Reg_E',   'f':'Z80_Reg_F',
        'h':'Z80_Reg_H',   'l':'Z80_Reg_L',
        'af':'Z80_Reg_AF', 'bc':'Z80_Reg_BC', 'de':'Z80_Reg_DE', 'hl':'Z80_Reg_HL',
        'sp':'Z80_Reg_Sp', 'pc':'Z80_Reg_Pc' }

    def _Z80_GenerateOperandCode(self, insn):
        class OprdVst(ast.NodeVisitor):
            def __init__(self, parent, insn):
                self.parent = parent
                self.insn = insn
                ast.NodeVisitor.__init__(self)
                self.var_expr = []

            def visit_Call(self, node):
                func_name = self.visit(node.func)
                func_args = []

                for arg in node.args:
                    v_arg = self.visit(arg)
                    assert(v_arg is not None)
                    func_args.append(v_arg)

                if func_name == 'reg':
                    assert(len(func_args) == 1)
                    id_expr = 'Expr::MakeId(%s, &m_CpuInfo)' % func_args[0]
                    self.var_expr.append(id_expr)
                    return id_expr

                # FIXME: handle access size correctly
                if func_name == 'mem':
                    assert(len(func_args) == 1)
                    mem_expr = 'Expr::MakeMem(%d, %s, %s, %s)' %\
                    (8, 'nullptr', func_args[0], 'true')
                    self.var_expr[-1] = mem_expr
                    return mem_expr

                if func_name == 'read':
                    assert(len(func_args) == 1 or len(func_args) == 2)
                    read_size = func_args[0]
                    self.var_expr.append(self.parent._GenerateRead('Imm', 'Offset + rInsn.GetLength()', read_size))
                    self.var_expr.append('rInsn.Length() += sizeof(Imm);\n')

                    if len(func_args) == 1:
                        self.var_expr.append('Expr::MakeConst(%d, Imm)' % read_size)

                    else:
                        self.var_expr.append('Expr::MakeConst(%d, SignExtend<s64, %d>(Imm))' % (func_args[1], read_size))
                    return self.var_expr[-1]

                if func_name == 'cst':
                    assert(len(func_args) == 2)
                    cst_size = func_args[0]
                    cst_value = func_args[1]
                    self.var_expr.append('Expr::MakeConst(%d, %#x)' % (cst_size, cst_value))
                    return self.var_expr[-1]

                if func_name == 'sign_extend':
                    assert(len(func_args) == 2)
                    val = func_args[0]
                    sign_bit = func_args[1]
                    self.var_expr.pop()
                    self.var_expr.append('SignExtend<s64, %d>(%s)' % (sign_bit, val))
                    return self.var_expr[-1]

                raise Exception('call %s' % func_name)

            def visit_Name(self, node):
                return node.id


                return node_name

            def visit_Num(self, node):
                return node.n

            def visit_Str(self, node):
                if node.s in self.parent.id_mapper:
                    return self.parent.id_mapper[node.s]

                return node.s

            def visit_BinOp(self, node):
                op = self.visit(node.op)
                if not op:
                    raise Exception('unknown operation %s' % node.op)
                left = self.visit(node.left)
                self.var_expr.pop()
                right = self.visit(node.right)
                self.var_expr.pop()

                op_expr = 'Expr::MakeOp(%s,\n%s,\n%s)' % (op, Indent(left), Indent(right))
                self.var_expr.append(op_expr)
                return op_expr

            def visit_Add(self, node):
                return 'OperationExpression::OpAdd'

            def visit_BitXor(self, node):
                return 'OperationExpression::OpXor'

        res = ''
        oprd_no = 0
        for oprd in insn['operand']:
            v = OprdVst(self, insn)
            oprd_code = self.arch['operand'][oprd]
            for oprd_sem in oprd_code:
                nodes = ast.parse(oprd_sem)
                v.visit(nodes)

                for expr in v.var_expr[:-1]:
                    res += expr
            res += 'auto spOprd%d = %s;\n' % (oprd_no, v.var_expr[-1])
            res += self._GenerateCondition('if', 'spOprd%d == nullptr' % oprd_no, 'return false;')
            res += 'rInsn.AddOperand(spOprd%d);\n' % oprd_no

            oprd_no += 1
        return res


    def _Z80_GenerateDispatcher(self, tbl, depth):
        res = '++rInsn.Length();\n'
        res += self._GenerateRead('Opcode%d' % depth, 'Offset + %d' % depth, 8)
        insn_cases = []
        for insn in sorted(tbl, key = lambda i: i['opcode']):
            case_stmt = ''
            if 'reference' in insn:
                case_stmt += 'rInsn.Length() += %d;\n' % (depth + 1)
                case_stmt += self._GenerateBrace(self._Z80_GenerateDispatcher(self.arch['instruction'][insn['reference']], depth + 1))
            else:
                case_stmt += 'rInsn.SetName("%s");\n' % insn['mnemonic']
                cond = { 'jmp':'Instruction::JumpType', 'call':'Instruction::CallType', 'ret':'Instruction::ReturnType', 'cond':'Instruction::ConditionalType' }
                insn_sub_type = []
                for insn_attr in insn['attribute']:
                    # HACK: since medusa doesn't support unary operator,
                    # we need to encode the not on flag using instruction prefix
                    if insn_attr == 'not_flag':
                        case_stmt += 'rInsn.Prefix() |= Z80_Insn_Prefix_NotFlag;\n'
                        continue

                    insn_sub_type.append(cond[insn_attr])
                if len(insn_sub_type) != 0:
                    case_stmt += 'rInsn.SubType() |= %s;\n' % ' | '.join(insn_sub_type)

                tested_flag = { 'cf' }

                if len(insn['operand']) != 0:
                    case_stmt += self._GenerateBrace(self._Z80_GenerateOperandCode(insn))

            case_stmt += 'return true;\n'

            insn_cases.append(('0x%02x' % insn['opcode'], case_stmt, False))

        res += self._GenerateSwitch('Opcode%d' % depth, insn_cases, 'return false;')

        return res


    def GenerateHeader(self):
        return ''

    def GenerateSource(self):
        dispatcher = self._Z80_GenerateDispatcher(self.arch['instruction']['table'], 0)

        return 'bool Z80Architecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n{\n%s}\n' % Indent(dispatcher)

    def GenerateOpcodeEnum(self):
        return ''

    def GenerateOpcodeString(self):
        return ''

    def GenerateOperandDefinition(self):
        return ''

    def GenerateOperandCode(self):
        return ''
