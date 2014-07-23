#!/usr/bin/env python

import re
import ast
import sys
import yaml
import time
import string
import itertools
from compiler.ast import flatten

def Indent(text, indent = 1):
    if text == None:
        return ''
    res = ''
    strip = False
    if text[-1] != '\n':
        text += '\n'
        strip = True
    lines = text.split('\n')[:-1]
    if len(lines) == 0:
        return '  ' * indent + text
    for l in lines:
        if l == '':
            res += '\n'
            continue
        res += '  ' * indent + l + '\n'
    if strip == True and res[-1] == '\n':
        return res[:-1]
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
        res += Indent('%s' % default)
        res += '}\n'
        return res

    def _GenerateRead(self, var_name, addr, sz):
        return 'u%d %s;\nif (!rBinStrm.Read(%s, %s))\n  return false;\n\n' % (sz, var_name, addr, var_name)

    def _ConvertSemanticToCode(self, opcd, sem, id_mapper):
        class SemVisitor(ast.NodeVisitor):
            def __init__(self, id_mapper):
                ast.NodeVisitor.__init__(self)
                self.id_mapper = id_mapper
                self.var_expr = []

            def reset(self):
                self.res = ''

            def generic_visit(self, node):
                print('generic:', type(node).__name__)
                assert(0)

            def visit_Module(self, node):
                for b in node.body:
                    self.res += self.visit(b)

            def visit_If(self, node):
                assert(len(node.body) == 1)
                test_name = self.visit(node.test)
                body_name = self.visit(node.body[0])

                if len(node.orelse) == 0:
                    return 'new IfConditionExpression(\n%s,\n%s)\n' % (Indent(test_name), Indent(body_name))

                assert(len(node.orelse) == 1)
                else_name = self.visit(node.orelse[0])
                return 'new IfElseConditionExpression(\n%s,\n%s,\n%s)\n' % (Indent(test_name), Indent(body_name), Indent(else_name))

            def visit_IfExp(self, node):
                assert(0)

            def visit_Compare(self, node):
                assert(len(node.ops) == 1)
                assert(len(node.comparators) == 1)
                oper_name = self.visit(node.ops[0])
                left_name = self.visit(node.left)
                comp_name = self.visit(node.comparators[0])
                return '%s,\n%s,\n%s' % (oper_name, left_name, comp_name)

            def visit_Eq(self, node):
                return 'ConditionExpression::CondEq'

            def visit_NotEq(self, node):
                return 'ConditionExpression::CondNe'

            def visit_Add(self, node):
                return 'OperationExpression::OpAdd'

            def visit_Sub(self, node):
                return 'OperationExpression::OpSub'

            def visit_BitOr(self, node):
                return 'OperationExpression::OpOr'

            def visit_BitAnd(self, node):
                return 'OperationExpression::OpAnd'

            def visit_BitXor(self, node):
                return 'OperationExpression::OpXor'

            def visit_LShift(self, node):
                return 'OperationExpression::OpLls'

            def visit_RShift(self, node):
                return 'OperationExpression::OpLrs'

            def visit_Mult(self, node):
                return 'OperationExpression::OpMul'

            def visit_BinOp(self, node):
                oper_name  = self.visit(node.op)
                left_name  = self.visit(node.left)
                right_name = self.visit(node.right)
                return 'new OperationExpression(\n%s,\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(left_name), Indent(right_name))

            def visit_Call(self, node):
                func_name = self.visit(node.func)
                if type(node.func).__name__ == 'Attribute':
                    print func_name
                    return func_name

                args_name = []
                for arg in node.args:
                    args_name.append(self.visit(arg))

                if 'OperationExpression::OpXchg' in func_name:
                    if len(args_name) != 2:
                        assert(0)
                    return 'new OperationExpression(\n%s,\n%s,\n%s);'\
                            % (Indent(func_name), Indent(args_name[0]), Indent(args_name[1]))

                if 'VariableExpression' in func_name:
                    self.var_expr.append(args_name[1].replace('"', ''))

                return func_name % tuple(args_name)

            def visit_Attribute(self, node):
                attr_name  = node.attr
                value_name = self.visit(node.value)

                if attr_name == 'id':
                    return 'new IdentifierExpression(%s, &m_CpuInfo)' % value_name

                elif attr_name == 'val':

                    if value_name.startswith('rInsn.Operand'):
                        return '%s->GetSemantic(rInsn.GetMode(), &m_CpuInfo, static_cast<u8>(rInsn.GetLength()), true)' % value_name

                    else: assert(0)

                elif attr_name == 'addr':

                    if value_name.startswith('rInsn.Operand'):
                        return '%s->GetSemantic(rInsn.GetMode(), &m_CpuInfo, static_cast<u8>(rInsn.GetLength()), false)' % value_name

                    else: assert(0)

                elif attr_name == 'size':

                    if value_name == 'rInsn':
                        get_pc_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()))'
                        return 'new ConstantExpression(\n%s,\n%s)'\
                                % (Indent(get_pc_size_bit), Indent('rInsn.GetLength()'))

                    elif value_name.startswith('rInsn.Operand'):
                        get_insn_size_bit = '%s->GetLength()' % value_name
                        return 'new ConstantExpression(\n%s,\n%s)'\
                                % (Indent('32'), Indent(get_insn_size_bit))

                    else:
                        get_reg_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name
                        return 'new ConstantExpression(\n%s,\n%s / 8)'\
                                % (Indent(get_reg_size_bit), Indent(get_reg_size_bit))

                elif attr_name == 'bit':
                    if value_name.startswith('rInsn.Operand'):
                        return '%s->GetLength() * 8' % value_name
                    else:
                        return 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name

                elif attr_name == 'mem':
                    get_reg_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name
                    return 'new MemoryExpression(%s, nullptr, new IdentifierExpression(%s, &m_CpuInfo))' % (get_reg_size_bit, value_name)

                assert(0)

            def visit_Name(self, node):
                node_name = node.id

                if node_name in self.id_mapper:
                    return self.id_mapper[node_name]

                if node_name in self.var_expr:
                    return 'new VariableExpression(0, "%s")' % node_name

                # Operand
                if node_name.startswith('op'):
                    return 'rInsn.Operand(%d)' % int(node_name[2:])

                # Identifier (register)
                elif node_name == 'id':
                    return 'new IdentifierExpression(%s, &m_CpuInfo)'

                # Integer
                elif node_name == 'int':
                    return 'new ConstantExpression(%s, %s)'
                elif node_name.startswith('int'):
                    int_size = int(node_name[3:])
                    return 'new ConstantExpression(%d, %%s)' % int_size

                # Variable
                elif node_name == 'var':
                    return 'new VariableExpression(%s, %s)'
                elif node_name.startswith('var'):
                    var_size = int(node_name[3:])
                    return 'new VariableExpression(%d, %%s)' % var_size

                # Flags
                elif node_name == 'update_flags':
                    return 'UpdateFlags(rInsn, %s)'

                if node_name == 'stack':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::StackPointerRegister, rInsn.GetMode())'
                elif node_name == 'frame':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::StackFrameRegister, rInsn.GetMode())'
                elif node_name == 'program':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode())'
                elif node_name == 'acc':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::AccumulatorRegister, rInsn.GetMode())'
                elif node_name == 'cnt':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::CounterRegister, rInsn.GetMode())'
                elif node_name == 'flag':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::FlagRegister, rInsn.GetMode())'
                elif node_name == 'insn':
                    return 'rInsn'

                # Fonction name
                elif node_name == 'swap':
                    return 'OperationExpression::OpXchg'
                elif node_name == 'sign_extend':
                    return 'new OperationExpression(OperationExpression::OpSext, %s, %s)'
                elif node_name == 'system':
                    return 'HandleSystemExpression(%s, rInsn)'

                assert(0)

            def visit_Num(self, node):
                return '%#x' % node.n
                return 'new ConstantExpression(0, %#x)' % node.n

            def visit_Str(self, node):
                return '"%s"' % node.s

            def visit_Print(self, node):
                assert(0)

            def visit_Assign(self, node):
                assert(len(node.targets) == 1)
                target_name = self.visit(node.targets[0])
                value_name  = self.visit(node.value)
                return 'new OperationExpression(OperationExpression::OpAff,\n%s,\n%s)\n'\
                        % (Indent(target_name), Indent(value_name))

            def visit_AugAssign(self, node):
                oper_name   = self.visit(node.op)
                target_name = self.visit(node.target)
                value_name  = self.visit(node.value)
                sub_expr = 'new OperationExpression(\n%s,\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(target_name), Indent(value_name))
                return 'new OperationExpression(\n  OperationExpression::OpAff,\n%s,\n%s)\n'\
                        % (Indent(target_name), Indent(sub_expr))

            def visit_Expr(self, node):
                return self.visit(node.value)

            def __str__(self):
                return self.res

        res = ''

        if sem != None:
            all_expr = []
            need_flat = False
            for x in sem:
                if type(x) == list:
                    need_flat = True
                    break
            if need_flat:
                sem = itertools.chain(*sem)

            v = SemVisitor(id_mapper)
            for expr in sem:
                v.reset()
                nodes = ast.parse(expr)
                v.visit(nodes)
                expr_res = v.res
                if expr_res[-1] == '\n':
                    expr_res = expr_res[:-1]
                all_expr.append('/* Semantic: %s */\n' % expr + expr_res + ';\n')
            sem_no = 0
            for expr in all_expr:
                res += 'auto pExpr%d = %s' % (sem_no, expr)
                res += 'AllExpr.push_back(pExpr%d);\n' % sem_no
                sem_no += 1

        if 'clear_flags' in opcd:
            for f in opcd['clear_flags']:
                res += 'AllExpr.push_back(Expr::MakeOp(OperationExpression::OpAff, Expr::MakeId(%s, &m_CpuInfo), Expr::MakeConst(ConstantExpression::Const1Bit, 0)));\n' % ('X86_Fl' + f.capitalize())

        if 'set_flags' in opcd:
            for f in opcd['set_flags']:
                res += 'AllExpr.push_back(Expr::MakeOp(OperationExpression::OpAff, Expr::MakeId(%s, &m_CpuInfo), Expr::MakeConst(ConstantExpression::Const1Bit, 1)));\n' % ('X86_Fl' + f.capitalize())

        if len(res) == 0:
            return ''

        var = 'Expression::List AllExpr;\n'
        res += 'rInsn.SetSemantic(AllExpr);\n'

        return self._GenerateBrace(var + res)

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
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)' % self.arch['arch_info']['name'].capitalize()

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
            if opcd['constraint'] == 'd64' or opcd['constraint'] == 'df64':
                res += self._GenerateCondition('if', '!(rInsn.Prefix() & X86_Prefix_OpSize)', 'rInsn.Prefix() |= X86_Prefix_REX_w; /* d64/df64 constraint */\n')

        if 'invalid' in opcd:
            return 'return false; /* INVALID */\n'

        if pfx_n != None:
            res += 'rInsn.Length()++;\n'
            if 'mnemonic' in opcd:
                res += 'rInsn.Prefix() |= X86_Prefix_%s;\n' % opcd['mnemonic']
            res += 'return Disassemble(rBinStrm, Offset + %d, rInsn, Mode);\n' % (pfx_n - 1)
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
                    '%s == false' % self.__X86_GenerateOperandMethod(opcd['operand']),
                    'return false;\n')
        id_mapper = {
                'cf':'X86_FlCf', 'pf':'X86_FlPf', 'af':'X86_FlAf', 'zf':'X86_FlZf',
                'sf':'X86_FlSf', 'tf':'X86_FlTf', 'if':'X86_FlIf', 'df':'X86_FlDf', 'of':'X86_FlOf',
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
                'rax':'X86_Reg_Rax', 'rbx':'X86_Reg_Rbx', 'rcx':'X86_Reg_Rcx', 'rdx':'X86_Reg_Rdx',
                'rsi':'X86_Reg_Rsi', 'rdi':'X86_Reg_Rdi', 'rsp':'X86_Reg_Rsp', 'rbp':'X86_Reg_Rbp',
                'r8':'X86_Reg_R8', 'r9':'X86_Reg_R9', 'r10':'X86_Reg_R10', 'r11':'X86_Reg_R11',
                'r12':'X86_Reg_R12', 'r13':'X86_Reg_R13', 'r14':'X86_Reg_R14', 'r15':'X86_Reg_R15' }

        if 'semantic' in opcd:
            res += self._ConvertSemanticToCode(opcd, opcd['semantic'], id_mapper)
        else:
            res += self._ConvertSemanticToCode(opcd, None, id_mapper)
        res += 'return true;\n'
        return res


    def __X86_GenerateInstructionCondition(self, opcd):
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

    def __X86_GenerateOperandMethod(self, oprd):
        res = 'Operand__%s(rBinStrm, Offset, rInsn, Mode)' % '_'.join(oprd)
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
            res += 'return (this->*m_%s[Opcode%s])(rBinStrm, Offset + 1, rInsn, Mode);\n' % (ref.capitalize(), tbl_off)

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
            res += self._GenerateSwitch('ModRm.Reg()', refs, 'return false;\n')

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
        res += Indent('typedef bool (%sArchitecture:: *TDisassembler)(BinaryStream const&, TOffset, Instruction&, u8);\n' % self.arch['arch_info']['name'].capitalize())

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
        res = ',\n'.join('"%s"' % x.lower() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.arch['arch_info']['name'].capitalize(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''
        for oprd in self.all_oprd:
            if oprd == '': continue
            res += Indent('bool Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode);\n' % oprd)
        return res

    def GenerateOperandCode(self):
        res = ''
        for oprd in self.all_oprd:
            if oprd == '': continue
            res += 'bool %sArchitecture::Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n' % (self.arch['arch_info']['name'].capitalize(), oprd)
            dec_op = []
            op_no = 0
            oprd = oprd.split('_')

            seg = ''
            for i in range(len(oprd)):
                seg += 'ApplySegmentOverridePrefix(rInsn, rInsn.Operand(%d));\n' % i

            # HACK: Handling case with E?_I? is tedious, we have to figure out the size of ModR/M
            # (which can have SIB and/or ImmXX)
            ei_hack = ''
            for o in oprd:
                if o[0] == 'I' and (oprd[0][0] == 'E' or (len(oprd) > 1 and oprd[1][0] == 'E')):
                    ei_hack += 'size_t PrefixOpcodeLength = rInsn.GetLength();\n'

                    for o in oprd:
                        if o[0] == 'I' and (oprd[0][0] == 'E' or (len(oprd) > 1 and oprd[1][0] == 'E')):
                            ei_hack += self._GenerateCondition('if', '!Decode_%s(rBinStrm, Offset + (rInsn.GetLength() - PrefixOpcodeLength), rInsn, rInsn.Operand(%d), Mode)' % (o, op_no),\
                                    'return false;')
                        else:
                            ei_hack += self._GenerateCondition('if', '!Decode_%s(rBinStrm, Offset, rInsn, rInsn.Operand(%d), Mode)' % (o, op_no),\
                                    'return false;')
                        self.all_dec.add('Decode_%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)' % o)
                        op_no += 1

                    ei_hack += seg
                    ei_hack += 'return true;\n'
                    break

            if len(ei_hack):
                res += self._GenerateBrace(ei_hack)
                continue

            for o in oprd:
                dec_op.append('Decode_%s(rBinStrm, Offset, rInsn, rInsn.Operand(%d), Mode)' % (o, op_no))
                op_no += 1
                self.all_dec.add('Decode_%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd, u8 Mode)' % o)
            res += self._GenerateBrace('bool Res =\n' + Indent(' &&\n'.join(dec_op) + ';\n') + seg + 'return Res;\n')
        return res

class ArmArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()

        all_instructions = self.arch['insn']

        self.arm_insns = []
        self.thumb_insns = []

        for insn in all_instructions:

            # We need to flatten the encoding array
            insn['encoding'] = flatten(insn['encoding'])

            # Check if instruction is valid
            self.__ARM_VerifyInstruction(insn)

            # Gather all mnemonics
            self.all_mnemo.add(self.__ARM_GetMnemonic(insn).capitalize())

            if insn['mode'][0] == 'A':
                self.arm_insns.append(insn)
            elif insn['mode'][0] == 'T':
                self.thumb_insns.append(insn)

        self.arm_insns.sort(key=lambda insn: insn['encoding'])
        self.thumb_insns.sort(key=lambda insn: insn['encoding'])

    def __ARM_VerifyInstruction(self, insn):
        enc = insn['encoding']
        if len(enc) != 16 and len(enc) != 32:
            print 'Invalid instruction "%s", encoding: %s, length: %d' % (insn['format'], insn['encoding'], len(insn['encoding']))

    def __ARM_Mangle(insn):
        encoding = []
        for bit in insn['encoding']:
            if type(bit) == int:
                encoding.append(str(bit))
            elif bit[0] == '(':
                encoding.append(bit[1])
            else:
                encoding.append(bit)
        return '%s_%s' % (insn['mode'], '_'.join(encoding))

    def __ARM_GetMnemonic(self, insn):
        fmt = insn['format']
        res = ''
        for c in fmt:
            if not c in string.ascii_letters+string.digits:
                break
            res += c
        return res

    def __ARM_GetMask(self, insn):
        enc = insn['encoding']
        res = 0x0
        off = 0x0
        for bit in enc[::-1]:
            if bit in [ 0, 1, '(0)', '(1)' ]:
                res |= (1 << off)
            off += 1
        return res

    def __ARM_GetValue(self, insn):
        enc = insn['encoding']
        res = 0x0
        off = 0x0
        for bit in enc[::-1]:
            if bit in [ 1, '(1)' ]:
                res |= (1 << off)
            off += 1
        return res

    def __ARM_GetSize(self, insn):
        return len(insn['encoding'])

    def __ARM_ExtractField(self, insn):
        fmt = insn['format']
        fields = []

        #print fmt

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

        oprds = all_oprds.split(',')
        e = len(oprds)
        i = 0
        while i < e:
            oprd = oprds[i].strip()
            if len(oprd) == 0:
                i += 1
                continue

            if oprd[-1] == '{':
                fields.append(oprds[i].strip() + ',' + oprds[i+1].strip())
                i += 1
            elif oprd[0] == '[' and not ']' in oprd:
                fields.append(oprds[i].strip() + ',' + oprds[i+1].strip())
                i += 1
            else:
                fields.append(oprd)
            i += 1

        return fields

    def __ARM_GenerateExtractBits(self, insn, pattern):
        enc = insn['encoding']
        beg = 0
        end = 0
        off = 0
        found = False
        for bf in enc[::-1]:
            if bf == pattern and not found:
                beg = off
                found = True

            elif bf != pattern and found:
                end = off - 1
                break
            off += 1

        if not found:
            raise Exception('Unable to generate extract bits for %s -  %s' % (pattern, insn))

        #if end != 0 and end <= beg:
        #    raise Exception('Invalid bit %d - %d' % (beg, end))

        if end == 0 and enc[0] == pattern:
            end = len(enc) - 1

        if beg == end:
            end = 0

        if end == 0:
            return 'ExtractBit<%d>(Opcode)' % beg

        return 'ExtractBits<%d, %d>(Opcode)' % (beg, end)

    def __ARM_GenerateExtractBitsSigned(self, insn, pattern, scale):
        enc = insn['encoding']
        beg = 0
        end = 0
        off = 0
        found = False
        for bf in enc[::-1]:
            if bf == pattern and not found:
                beg = off
                found = True

            if bf != pattern and found:
                end = off - 1
                break
            off += 1

        if not found:
            raise Exception('Unable to generate extract bits for %s -  %s' % (pattern, insn))

        if end != 0 and end <= beg:
            raise Exception('Invalid bit %d - %d' % (beg, end))

        if end == 0 and enc[0] == pattern:
            end = len(enc) - 1

        if end == 0:
            return 'SignExtend<s64, %d>(ExtractBit<%d>(Opcode) << %d)' % (beg + scale + 1, beg, scale)

        return 'SignExtend<s64, %d>(ExtractBits<%d, %d>(Opcode) << %d)' % (end + scale + 1, beg, end, scale)

    def __ARM_GenerateInstruction(self, insn):
        res = ''
        res += 'rInsn.SetName("%s");\n' % self.__ARM_GetMnemonic(insn)
        res += 'rInsn.SetOpcode(ARM_Opcode_%s);\n' % self.__ARM_GetMnemonic(insn).capitalize()
        res += 'rInsn.Length() += %d;\n' % (self.__ARM_GetSize(insn) / 8)

        map_op_type = { 'jmp' : 'Instruction::JumpType', 'call' : 'Instruction::CallType' }
        attrs = []
        for attr in insn['attribute']:
            if attr in map_op_type:
                attrs.append(map_op_type[attr])

        if len(attrs):
            res += 'rInsn.SubType() |= %s;\n' % ' | '.join(attrs)

        field_mapper = { 'c': ('c', 'rInsn.SetTestedFlags(%s);\n') }
        insn_fields = self.__ARM_ExtractField(insn)

        #print insn_fields

        # Test condition bits
        if 'c' in insn_fields and 'c' in insn['encoding']:
            res += Indent('u8 CondField = %s;\n' % self.__ARM_GenerateExtractBits(insn, 'c'), 0)
            res += Indent('rInsn.SetTestedFlags(CondField);\n', 0)
            res += Indent(self._GenerateCondition('if', 'CondField != 0xe',
                    'rInsn.SubType() |= Instruction::ConditionalType;'), 0)

        if 'S' in insn_fields and 'S' in insn['encoding']:
            res += Indent(self._GenerateCondition('if', self.__ARM_GenerateExtractBits(insn, 'S'), 'rInsn.Prefix() |= ARM_Prefix_S;'), 0)

        def SetOperand(oprd_idx, oprd_type, oprd_value, oprd_reg, oprd_secreg):
            oprd_str = ''
            oprd_str += Indent('auto pOprd%d = rInsn.Operand(%d);\n' % (oprd_idx, oprd_idx), 0)
            oprd_str += Indent('pOprd%d->SetType(%s);\n' % (oprd_idx, oprd_type), 0)
            if oprd_value:
                oprd_str += Indent('pOprd%d->SetValue(%s);\n' % (oprd_idx, oprd_value), 0)
            if oprd_reg:
                oprd_str += Indent('pOprd%d->SetReg(%s);\n' % (oprd_idx, oprd_reg), 0)
            if oprd_secreg:
                oprd_str += Indent('pOprd%d->SetSecReg(%s);\n' % (oprd_idx, oprd_secreg), 0)
            return oprd_str

        oprd_cnt = 0
        for field in insn_fields:
            if oprd_cnt > 4:
                break

            if field[-1] == '!':
                res += Indent('rInsn.Prefix() |= ARM_Prefix_W; /* TODO: Handle writeback (!) */\n', 0)
                field = field[:-1]

            if field.endswith('{!}'):
                res += Indent('rInsn.Prefix() |= ARM_Prefix_W; /* TODO: Handle writeback ({!}) */\n', 0)
                field = field[:-3]

            if field[0] == '<' and field[-1] == '>':
                field = field[1:-1]

            # Instruction attributes
            if len(field) == 1:
                pass

            # Operand deref
            elif field[0] == '[' and field[-1] == ']':
                field = field[1:-1]

                if ',' in field:
                    first, second = field.split(',')

                    reg = ''
                    imm = None
                    secreg = None
                    types = [ 'O_MEM32' ]

                    if first == 'PC' or first == 'SP':
                        reg = 'ARM_Reg%s' % first
                        types.append('O_REG32')
                    elif first == '<Rn>':
                        reg = '1 << %s' % self.__ARM_GenerateExtractBits(insn, 'n')
                        types.append('O_REG32')

                    if second[0] == '#':

                        # We need a special handling for [PC,#imm]
                        if first == 'PC':
                            res += self._GenerateRead('DstVal', 'Offset + 8 + %s' % self.__ARM_GenerateExtractBits(insn, 'i'), 32)
                            types = [] # We empty le types list because we want it to be treated as immediate
                            types.append('O_ABS32')
                            imm = 'DstVal'
                        else:
                            imm = self.__ARM_GenerateExtractBits(insn, 'i')
                            types.append('O_DISP32')
                    elif second == '<Rm>':
                        secreg = '1 << %s' % self.__ARM_GenerateExtractBits(insn, 'm')
                        types.append('O_SREG')

                    res += SetOperand(oprd_cnt, ' | '.join(types), imm, reg, secreg)
                    oprd_cnt += 1

                else:
                    if field[0] == '<' and field[-1] == '>':
                        field = field[1:-1]

                    # deref pc/sp
                    if field == 'PC' or field == 'SP':
                        res += SetOperand(oprd_cnt, 'O_MEM32 | O_REG32', None, 'ARM_Reg%s' % field, None)

                        if field == 'PC':
                            res += Indent(self._GenerateCondition('if', 'pOprd%d->GetReg() & ARM_RegPC' % oprd_cnt, 'rInsn.SubType() |= Instruction::JumpType;'), 0)

                        oprd_cnt += 1

                    elif field[0] == 'R':
                        pattern = field[1:].lower()
                        res += SetOperand(oprd_cnt, 'O_MEM32 | O_REG32', None, '1 << %s' % self.__ARM_GenerateExtractBits(insn, pattern), None)

                        if field == 'Rd':
                            res += Indent(self._GenerateCondition('if', 'pOprd%d->GetReg() & ARM_RegPC' % oprd_cnt, 'rInsn.SubType() |= Instruction::JumpType;'), 0)

                        oprd_cnt += 1

            # Operand defined register
            elif field == 'PC' or field == 'SP':
                res += SetOperand(oprd_cnt, 'O_REG32', None, 'ARM_Reg%s' % field, None)
                oprd_cnt += 1

            # Operand register
            elif field[0] == 'R':

                rp = field[1:].lower()
                if field == 'Rt2' and not rp in insn['encoding']:
                    res += Indent('/* TODO: Handle Rt2 without encoding */\n', 0)
                    continue

                res += SetOperand(oprd_cnt, 'O_REG32', None, '1 << %s' % self.__ARM_GenerateExtractBits(insn, rp), None)

                if field == 'Rd' or field == 'Rt':
                    res += Indent(self._GenerateCondition('if', 'pOprd%d->GetReg() & ARM_RegPC' % oprd_cnt, 'rInsn.SubType() |= Instruction::JumpType;'), 0)
                oprd_cnt += 1

            # Operand branch
            elif field == 'label':
                if insn['mode'][0] == 'A':
                    scale = 2
                elif insn['mode'][0] == 'T':
                    scale = 1
                else:
                    raise Exception('Unknown scale')

                if 'J1' in insn['encoding'] and 'J2' in insn['encoding']:
                    res += Indent('/* TODO: Handle ih:J1:J2:il operand */\n', 0)
                else:
                    # HACK: ARM add 4 bytes
                    res += SetOperand(oprd_cnt, 'O_REL32', self.__ARM_GenerateExtractBitsSigned(insn, 'i', scale) + ' + 4', None, None)
                    oprd_cnt += 1

            # Operand registers
            elif field == 'registers':

                if 'r' in insn['encoding']:
                    pattern = 'r'
                else:
                    pattern = 't'
                res += SetOperand(oprd_cnt, 'O_REG32', None, self.__ARM_GenerateExtractBits(insn, pattern), None)
                oprd_cnt += 1

            # Operand constant (ARMExpandImm TODO ThumbExpandImm!)
            elif field == '#<const>' or field.startswith('#<imm') or field.startswith('#+/-<imm'):

                pattern = 'i'

                if not 'i' in insn['encoding']:
                    pattern = 'size'
                    res += SetOperand(oprd_cnt, 'O_IMM32', self.__ARM_GenerateExtractBits(insn, pattern), None, None)
                else:
                    res += Indent('u32 RawImm = %s;\n' % self.__ARM_GenerateExtractBits(insn, pattern), 0)
                    res += Indent('u32 Imm = UnsignedRotateRight(ExtractBits<0, 7>(RawImm), (2 * ExtractBits<8, 11>(RawImm)) % 32);\n', 0)

                    res += SetOperand(oprd_cnt, 'O_IMM32', 'Imm', None, None)
                oprd_cnt += 1

            # Operand unhandled
            else:
                print 'Unable to handle field: "%s"' % field

        id_mapper = {
                'r0':'ARM_Reg0','r1':'ARM_Reg1','r2':'ARM_Reg2','r3':'ARM_Reg3',
                'r4':'ARM_Reg4','r5':'ARM_Reg5','r6':'ARM_Reg6','r7':'ARM_Reg7',
                'r8':'ARM_Reg8','r9':'ARM_Reg9','r10':'ARM_Reg10','r11':'ARM_Reg11',
                'r12':'ARM_Reg12','r13':'ARM_Reg13','r14':'ARM_Reg14','r15':'ARM_Reg15',
                'pc':'ARM_RegPC','sp':'ARM_RegSP'

                }


        if 'semantic' in insn:
            res += self._ConvertSemanticToCode(insn, insn['semantic'], id_mapper)

        res += 'return true;\n'

        return self.__ARM_GenerateMethodPrototype(insn, False) + '\n' + self._GenerateBrace(res)

    def __ARM_GenerateInstructionComment(self, insn):
        return '// %s - %s\n' % (insn['format'], insn['encoding'])

    def __ARM_GenerateMethodName(self, insn):
        mnem  = self.__ARM_GetMnemonic(insn)
        mode  = insn['mode']
        mask  = self.__ARM_GetMask(insn)
        value = self.__ARM_GetValue(insn)
        return 'Instruction_%s_%s_%08x_%08x' % (mnem, mode, mask, value)

    def __ARM_GenerateMethodPrototype(self, insn, in_class = False):
        mnem = self.__ARM_GetMnemonic(insn)
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)' % self.arch['arch_info']['name'].capitalize()

        return meth_fmt % self.__ARM_GenerateMethodName(insn)

    def GenerateHeader(self):
        res = ''
        res += 'static char const *m_Mnemonic[%#x];\n' % (len(self.all_mnemo) + 1)

        for insn in sorted(self.arch['insn'], key=lambda a:self.__ARM_GetMnemonic(a)):
            res += self.__ARM_GenerateMethodPrototype(insn, True) + ';\n'

        res += 'bool DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n'
        res += 'bool DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n'

        return res

    def GenerateSource(self):
        res = ''

        res += 'bool ArmArchitecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n'
        res += self._GenerateBrace(
                self._GenerateSwitch('Mode',
                    [('ARM_ModeArm',   'return DisassembleArm(rBinStrm, Offset, rInsn);\n',   False),
                     ('ARM_ModeThumb', 'return DisassembleThumb(rBinStrm, Offset, rInsn);\n', False)],
                    'return false;\n')
                )

        def __ARM_GenerateDispatcher(arm, insns):
            res = ''
            insns_dict = {}

            for insn in insns:
                mask = arm.__ARM_GetMask(insn)
                if not mask in insns_dict:
                    insns_dict[mask] = []
                insns_dict[mask].append(insn)

            for mask, insn_list in insns_dict.items():
                bit = len(insn_list[0]['encoding'])
                if len(insn_list) == 1:
                    value = arm.__ARM_GetValue(insn_list[0])
                    res += arm._GenerateCondition('if', '(Opcode%d & %#010x) == %#010x' % (bit, mask, value), self.__ARM_GenerateInstructionComment(insn) + 'return %s(rBinStrm, Offset, Opcode%d, rInsn);' % (arm.__ARM_GenerateMethodName(insn_list[0]), bit))
                else:
                    cases = []
                    for insn in insn_list:
                        value = arm.__ARM_GetValue(insn)
                        cases.append( ('%#010x' % value, self.__ARM_GenerateInstructionComment(insn) + 'return %s(rBinStrm, Offset, Opcode%d, rInsn);\n' % (arm.__ARM_GenerateMethodName(insn), bit), False) )
                    res += arm._GenerateSwitch('Opcode%d & %#010x' % (bit, mask), cases, 'break;\n')

            return res

        res += 'bool ArmArchitecture::DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res += self._GenerateBrace(
                self._GenerateRead('Opcode32', 'Offset', 32)+
                __ARM_GenerateDispatcher(self, self.arm_insns)+
                'return false;\n'
                )

        res += 'bool ArmArchitecture::DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res += self._GenerateBrace(
                self._GenerateRead('Opcode16Low', 'Offset & ~1', 16)+
                self._GenerateRead('Opcode16High', '(Offset + 2) & ~1', 16)+
                Indent('u16 Opcode16 = Opcode16Low;\n', 0)+
                Indent('u32 Opcode32 = ((Opcode16Low << 16) | Opcode16High);\n', 0)+
                __ARM_GenerateDispatcher(self, self.thumb_insns)+
                'return false;\n'
                )

        for insn in self.arm_insns + self.thumb_insns:
            res += self.__ARM_GenerateInstructionComment(insn)
            res += self.__ARM_GenerateInstruction(insn)

        return res

    def GenerateOpcodeEnum(self):
        res = ',\n'.join('ARM_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum ARM_Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x.lower() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.arch['arch_info']['name'].capitalize(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''
        return res

    def GenerateOperandCode(self):
        res = ''
        return res

def main():

    for arch_file in sys.argv[1:]:
        f = open(arch_file)
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
