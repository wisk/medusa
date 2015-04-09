from helper import *

import ast
import time

class ArchConvertion:
    def __init__(self, arch):
        self.arch = arch

    def GetArchName(self):
        return self.arch['architecture_information']['name'].capitalize()

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

        res += 'switch (%s)\n' % cond
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
                self.var_pool = []

            def reset(self):
                self.var_expr = []
                self.res = ''

            def generic_visit(self, node):
                print('generic:', type(node).__name__)
                assert(0)

            def visit_Module(self, node):
                for b in node.body:
                    self.res += self.visit(b)

            def visit_If(self, node):
                test_name = self.visit(node.test)

                body_name = None
                if len(node.body) == 1:
                    body_name = self.visit(node.body[0])
                else:
                    self.var_expr.append('Expression::LSPType BodyExprs;\n')
                    for statm in node.body:
                        self.var_expr.append('BodyExprs.push_back(%s);\n' % (self.visit(statm)))
                    statm_i = 0;
                    self.var_expr.append('auto spBody = Expr::MakeBind(BodyExprs);\n')
                    body_name = 'spBody'

                if len(node.orelse) == 0:
                    return 'Expr::MakeIfElseCond(\n%s,\n%s, nullptr)\n' % (Indent(test_name), Indent(body_name))

                # TODO: handle multiple statment for else
                assert(len(node.orelse) == 1)
                else_name = self.visit(node.orelse[0])
                return 'Expr::MakeIfElseCond(\n%s,\n%s,\n%s)\n' % (Indent(test_name), Indent(body_name), Indent(else_name))

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

            def visit_Div(self, mode):
                return 'OperationExpression::OpUDiv'

            def visit_Mod(self, mode):
                return 'OperationExpression::OpUMod'

            def visit_BinOp(self, node):
                oper_name  = self.visit(node.op)
                left_name  = self.visit(node.left)
                right_name = self.visit(node.right)
                return 'Expr::MakeBinOp(\n%s,\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(left_name), Indent(right_name))

            def visit_Call(self, node):
                func_name = self.visit(node.func)
                if type(node.func).__name__ == 'Attribute':
                    print func_name
                    return func_name

                args_name = []
                for arg in node.args:
                    args_name.append(self.visit(arg))

                if func_name == 'ignore':
                    return args_name[0][1:-1]

                if func_name == 'concat':
                    return ''.join(args_name)

                if 'VariableExpression::Alloc' in func_name:
                    var_name = args_name[0][1:-1]
                    assert(not var_name in self.var_pool)
                    self.var_pool.append(var_name)
                if 'VariableExpression::Free' in func_name:
                    var_name = args_name[0][1:-1]
                    assert(var_name in self.var_pool)
                    self.var_pool.remove(args_name[0][1:-1])

                if 'OperationExpression::OpXchg' in func_name:
                    if len(args_name) != 2:
                        assert(0)
                    return 'Expr::MakeBinOp(\n%s,\n%s,\n%s);'\
                            % (Indent(func_name), Indent(args_name[0]), Indent(args_name[1]))

                return func_name % tuple(args_name)

            def visit_Attribute(self, node):
                attr_name  = node.attr
                value_name = self.visit(node.value)

                if attr_name == 'id':
                    return 'Expr::MakeId(%s, &m_CpuInfo)' % value_name

                elif attr_name == 'val':
                    return value_name

                elif attr_name == 'addr':
                    return '%s->ToAddress()' % value_name

                elif attr_name == 'size':

                    if value_name == 'rInsn':
                        get_pc_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()))'
                        return 'Expr::MakeConst(\n%s,\n%s)'\
                                % (Indent(get_pc_size_bit), Indent('rInsn.GetLength()'))

                    elif value_name.startswith('rInsn.GetOperand'):
                        get_insn_size_bit = '(%s->GetBitSize() / 8)' % value_name
                        return 'Expr::MakeConst(\n%s,\n%s)'\
                                % (Indent('32'), Indent(get_insn_size_bit))

                    else:
                        get_reg_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name
                        return 'Expr::MakeConst(\n%s,\n%s / 8)'\
                                % (Indent(get_reg_size_bit), Indent(get_reg_size_bit))

                elif attr_name == 'bit':
                    if value_name.startswith('rInsn.GetOperand'):
                        return '%s->GetBitSize()' % value_name
                    else:
                        return 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name

                elif attr_name == 'mem':
                    get_reg_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name
                    return 'Expr::MakeMem(%s, nullptr, Expr::MakeId(%s, &m_CpuInfo))' % (get_reg_size_bit, value_name)

                assert(0)

            def visit_Name(self, node):
                node_name = node.id

                if node_name in [ 'ignore', 'concat' ]:
                    return node_name

                if node_name in self.id_mapper:
                    return self.id_mapper[node_name]

                if node_name in self.var_pool:
                    return 'Expr::MakeVar("%s", VariableExpression::Use)' % node_name

                # Operand
                if node_name.startswith('op'):
                    return 'rInsn.GetOperand(%d)' % int(node_name[2:])

                # Identifier (register)
                elif node_name == 'id':
                    return 'Expr::MakeId(%s, &m_CpuInfo)'

                # Integer
                elif node_name == 'int':
                    return 'Expr::MakeConst(%s, %s)'
                elif node_name.startswith('int'):
                    int_size = int(node_name[3:])
                    return 'Expr::MakeConst(%d, %%s)' % int_size

                # Variable
                elif node_name == 'alloc_var':
                    return 'Expr::MakeVar(%s, VariableExpression::Alloc, %s)'
                elif node_name.startswith('alloc_var'):
                    var_size = int(node_name[9:])
                    return 'Expr::MakeVar(%%s, VariableExpression::Alloc, %d)' % var_size
                elif node_name == 'free_var':
                    return 'Expr::MakeVar(%s, VariableExpression::Free)'
                elif node_name == 'var':
                    return 'Expr::MakeVar(%s, VariableExpression::Use)'

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
                elif node_name == 'div':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::DivisorRegister, rInsn.GetMode())'
                elif node_name == 'rem':
                    return 'm_CpuInfo.GetRegisterByType(CpuInformation::RemainderRegister, rInsn.GetMode())'
                elif node_name == 'insn':
                    return 'rInsn'
                elif node_name == 'res':
                    return 'spResExpr->Clone()'

                # Fonction name
                elif node_name == 'exchange':
                    return 'OperationExpression::OpXchg'
                elif node_name == 'swap':
                    return 'Expr::MakeUnOp(OperationExpression::OpSwap, %s)'
                elif node_name == 'bsf':
                    return 'Expr::MakeUnOp(OperationExpression::OpBsf, %s)'
                elif node_name == 'bsr':
                    return 'Expr::MakeUnOp(OperationExpression::OpBsr, %s)'
                elif node_name == 'sign_extend':
                    return 'Expr::MakeBinOp(OperationExpression::OpSext, %s, %s)'
                elif node_name == 'zero_extend':
                    return 'Expr::MakeBinOp(OperationExpression::OpZext, %s, %s)'
                elif node_name == 'ars':
                    return 'Expr::MakeBinOp(OperationExpression::OpArs, %s, %s)'
                elif node_name == 'sdiv':
                    return 'Expr::MakeBinOp(OperationExpression::OpSDiv, %s, %s)'
                elif node_name == 'smod':
                    return 'Expr::MakeBinOp(OperationExpression::OpSMod, %s, %s)'
                elif node_name == 'bit_cast':
                    return 'Expr::MakeBinOp(OperationExpression::OpBcast, %s, %s)'
                elif node_name == 'expr':
                    return 'HandleExpression(AllExpr, %s, rInsn, spResExpr)'

                assert(0)

            def visit_Num(self, node):
                return '%#x' % node.n
                return 'Expr::MakeConst(0, %#x)' % node.n

            def visit_Str(self, node):
                return '"%s"' % node.s

            def visit_Print(self, node):
                assert(0)

            def visit_Assign(self, node):
                assert(len(node.targets) == 1)

                value_name  = self.visit(node.value)

                if hasattr(node.targets[0], 'id') and node.targets[0].id == 'res':
                    return 'spResExpr = %s' % value_name                
                target_name = self.visit(node.targets[0])

                return 'Expr::MakeAssign(\n%s,\n%s)\n'\
                        % (Indent(target_name), Indent(value_name))

            def visit_AugAssign(self, node):
                oper_name   = self.visit(node.op)
                target_name = self.visit(node.target)
                value_name  = self.visit(node.value)
                sub_expr = 'Expr::MakeBinOp(\n%s,\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(target_name), Indent(value_name))
                return 'Expr::MakeAssign(\n%s,\n%s)\n'\
                        % (Indent(target_name), Indent(sub_expr))

            def visit_Expr(self, node):
                return self.visit(node.value)

            def __str__(self):
                return self.res

        res = 'Expression::SPType spResExpr;\n'

        if 'test_flags' in opcd:
            res += 'rInsn.SetTestedFlags(%s);\n' % ' | '.join(self.id_mapper[x] for x in opcd['test_flags'])
        if 'update_flags' in opcd:
            res += 'rInsn.SetUpdatedFlags(%s);\n' % ' | '.join(self.id_mapper[x] for x in opcd['update_flags'])
        if 'clear_flags' in opcd:
            res += 'rInsn.SetClearedFlags(%s);\n' % ' | '.join(self.id_mapper[x] for x in opcd['clear_flags'])
            for f in opcd['clear_flags']:
                res += 'AllExpr.push_back(Expr::MakeAssign(Expr::MakeId(%s, &m_CpuInfo), Expr::MakeBoolean(false)));\n' % ('X86_Fl' + f.capitalize())
        if 'set_flags' in opcd:
            res += 'rInsn.SetFixedFlags(%s);\n' % ' | '.join(self.id_mapper[x] for x in opcd['set_flags'])
            for f in opcd['set_flags']:
                res += 'AllExpr.push_back(Expr::MakeAssign(Expr::MakeId(%s, &m_CpuInfo), Expr::MakeBoolean(true)));\n' % ('X86_Fl' + f.capitalize())

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
                #print 'DEBUG: %r' % expr
                v.reset()
                nodes = ast.parse(expr)
                v.visit(nodes)
                if len(v.var_expr): res += '/* Var Expr */\n'
                for var in v.var_expr:
                    res += var
                if len(v.var_expr): res += '\n'
                expr_res = v.res
                if expr_res[-1] == '\n':
                    expr_res = expr_res[:-1]
                all_expr.append('/* Semantic: %s */\n' % expr + expr_res + ';\n')
            sem_no = 0
            for expr in all_expr:
                if 'HandleExpression' in expr:
                    res += expr
                    continue
                elif 'spResExpr = ' in expr:
                    res += expr
                else:
                    res += 'auto pExpr%d = %s' % (sem_no, expr)
                    res += 'AllExpr.push_back(pExpr%d);\n' % sem_no
                    sem_no += 1

        if len(res) == 0:
            return ''

        var = 'Expression::LSPType AllExpr;\n'
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
