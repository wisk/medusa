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
        class CodeBlock:
            CMT_TYPE = 0
            EXPR_TYPE = 1
            IF_CODE_TYPE = 2
            IF_EXPR_TYPE = 3
            WHILE_EXPR_TYPE = 4

            def __init__(self, blk_type):
                self.blk_type = blk_type
                self.cmt = None
                self.cond = None
                self.expr_decl = []
                self.sub_exprs = []
                self.glb_exprs = []

            def SetComment(self, cmt):
                self.cmt = cmt

            def SetCondition(self, cond):
                self.cond = cond

            def AddDeclaration(self, expr):
                self.expr_decl.append(expr)

            def AddSubExpr(self, expr):
                self.sub_exprs.append(expr)

            def AddGlbExpr(self, expr):
                self.glb_exprs.append(expr)

            def Format(self, expr_container):
                if self.blk_type == self.CMT_TYPE:
                    if not self.cmt:
                        raise Exception('invalid code block comment')
                    return self.cmt

                if self.blk_type == self.EXPR_TYPE or self.blk_type == self.IF_EXPR_TYPE or self.blk_type == self.WHILE_EXPR_TYPE:
                    res = ''
                    if len(self.expr_decl):
                        # res += '/* block declarations */\n'
                        res += ';\n'.join(self.expr_decl)
                    if len(self.sub_exprs):
                        # res += '/* block sub expressions */\n'
                        res += ';\n'.join(self.sub_exprs)
                    if len(self.glb_exprs):
                        # res += '/* block glb expressions */\n'
                        if expr_container:
                            for expr in self.glb_exprs:
                                res += '%s.push_back(%s);\n' % (expr_container, expr)
                        else:
                            res += ',\n'.join(self.glb_exprs)
                    return res

                if self.blk_type == self.IF_CODE_TYPE:
                    res = ''
                    res += 'if (%s)\n' % self.cond
                    res += '{\n'
                    if len(self.expr_decl):
                        res += Indent('/* block declarations */\n')
                        res += Indent(';\n'.join(self.expr_decl))
                    if len(self.sub_exprs):
                        res += Indent('/* block sub expressions */\n')
                        res += Indent(';\n'.join(self.sub_exprs))
                    if len(self.glb_exprs):
                        res += Indent('/* block glb expressions */\n')
                        for expr in self.glb_exprs:
                            res += Indent('%s.push_back(%s);\n' % (expr_container, expr))
                    res += '}\n'
                    return res

                raise Exception('unknown code block type')

        class SemVisitor(ast.NodeVisitor):
            def __init__(self, arch, id_mapper):
                ast.NodeVisitor.__init__(self)
                self.arch = arch
                self.id_mapper = id_mapper
                self.cnt = 0
                self.var_pool = []
                self.expr_container = 'AllExpr' # TODO(KS): don't hardcode this
                self.res = []

            def reset(self):
                self.res = []

            def generic_visit(self, node):
                print('generic:', type(node).__name__)
                assert(0)

            # Main visitors

            def visit_Module(self, node):
                res = []
                for b in node.body:
                    cb = self.visit(b)
                    if not cb: continue
                    if isinstance(cb, list):
                        res += cb
                    else:
                        res.append(cb)
                return res

            def visit_Assign(self, node):
                assert(len(node.targets) == 1)

                value_name  = self.visit(node.value)
                target_name = self.visit(node.targets[0])

                cb = CodeBlock(CodeBlock.EXPR_TYPE)
                cb.AddGlbExpr('Expr::MakeAssign(\n%s,\n%s)'\
                        % (Indent(target_name), Indent(value_name)))
                return cb

            def visit_AugAssign(self, node):
                oper_name   = self.visit(node.op)
                target_name = self.visit(node.target)
                value_name  = self.visit(node.value)
                sub_expr = 'Expr::MakeBinOp(\n%s,\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(target_name), Indent(value_name))

                cb = CodeBlock(CodeBlock.EXPR_TYPE)
                cb.AddGlbExpr('Expr::MakeAssign(\n%s,\n%s)\n'\
                        % (Indent(target_name), Indent(sub_expr)))
                return cb

            def visit_If(self, node):
                vst_test = self.visit(node.test)
                code_blk_type = vst_test[0]
                test = vst_test[1]
                code_blk = CodeBlock(code_blk_type)

                vst_test = self.visit(test)
                vst_then_stmt = []
                vst_else_stmt = []

                if code_blk_type == CodeBlock.IF_CODE_TYPE:
                    code_blk.SetCondition(vst_test)
                    for statm in node.body:
                        statm_expr = self.visit(statm)
                        if isinstance(statm_expr, list):
                            vst_then_stmt += [ x.Format(None) for x in statm_expr ]
                        else:
                            vst_then_stmt.append(statm_expr.Format(None))
                    for expr in vst_then_stmt:
                        code_blk.AddGlbExpr(expr)
                    return code_blk

                else:
                    then_body_name = None
                    for statm in node.body:
                        statm_expr = self.visit(statm)
                        if isinstance(statm_expr, list):
                            vst_then_stmt += [ x.Format(None) for x in statm_expr ]
                        else:
                            vst_then_stmt.append(statm_expr.Format(None))
                    assert(len(vst_then_stmt) != 0)
                    if len(vst_then_stmt) == 1:
                        then_body_name = vst_then_stmt[0]
                    else:
                        then_body_name = 'Expr::MakeBind({\n%s})\n' % ',\n'.join(vst_then_stmt)

                    if len(node.orelse) == 0:
                        res = 'Expr::MakeIfElseCond(\n%s,\n%s, nullptr)\n' % (Indent(vst_test), Indent(then_body_name))
                        code_blk.AddGlbExpr(res)
                        return code_blk

                    else_body_name = None
                    for statm in node.orelse:
                        statm_expr = self.visit(statm)
                        if isinstance(statm_expr, list):
                            vst_else_stmt += [ x.Format(None) for x in statm_expr ]
                        else:
                            vst_else_stmt.append(statm_expr.Format(None))
                    assert(len(vst_else_stmt) != 0)
                    if len(vst_else_stmt) == 1:
                        else_body_name = vst_else_stmt[0]
                    else:
                        else_body_name = 'Expr::MakeBind({\n%s})\n' % ',\n'.join(vst_else_stmt)

                    res = 'Expr::MakeIfElseCond(\n%s,\n%s,\n%s)' % (Indent(vst_test), Indent(then_body_name), Indent(else_body_name))
                    code_blk.AddGlbExpr(res)
                    return code_blk

            def visit_While(self, node):
                vst_test = self.visit(node.test)
                vst_body = []
                for statm in node.body:
                    statm_expr = self.visit(statm)
                    if isinstance(statm_expr, list):
                        vst_body += [ x.Format(None) for x in statm_expr ]
                    else:
                        vst_body.append(statm_expr.Format(None))

                body_expr = None
                if len(vst_body) == 1:
                    body_expr = vst_body[0]
                else:
                    body_expr = 'Expr::MakeBind({\n%s})\n' % ',\n'.join(vst_body)

                res = 'Expr::MakeWhileCond(\n%s,\n%s)' % (Indent(vst_test), Indent(body_expr))

                code_blk = CodeBlock(CodeBlock.WHILE_EXPR_TYPE)
                code_blk.SetCondition(vst_test)
                code_blk.AddGlbExpr(res)
                return code_blk

            def visit_IfExp(self, node):
                assert(0)

            # Leaf visitors

            # TODO(KS): make sure node.op is _ast.And
            def visit_BoolOp(self, node):
                if_type = node.values[0].id
                if if_type == '__expr':
                    return (CodeBlock.IF_EXPR_TYPE, node.values[1])
                if if_type == '__code':
                    return (CodeBlock.IF_CODE_TYPE, node.values[1])
                raise Exception('Please specify the type of if in first position')

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

            def visit_Gt(self, node):
                return 'ConditionExpression::CondUgt'

            def visit_GtE(self, node):
                return 'ConditionExpression::CondUge'

            def visit_Lt(self, node):
                return 'ConditionExpression::CondUlt'

            def visit_LtE(self, node):
                return 'ConditionExpression::CondUle'

            def visit_UnaryOp(self, node):
                oper_name = self.visit(node.op)
                oprd_name = self.visit(node.operand)

                return 'Expr::MakeUnOp(\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(oprd_name))

            def visit_Invert(self, node):
                return 'OperationExpression::OpNot'

            def visit_USub(self, node):
                return 'OperationExpression::OpNeg'

            def visit_BinOp(self, node):
                oper_name  = self.visit(node.op)
                left_name  = self.visit(node.left)
                right_name = self.visit(node.right)
                return 'Expr::MakeBinOp(\n%s,\n%s,\n%s)'\
                        % (Indent(oper_name), Indent(left_name), Indent(right_name))

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

            def visit_Div(self, node):
                return 'OperationExpression::OpUDiv'

            def visit_Mod(self, node):
                return 'OperationExpression::OpUMod'

            def visit_Call(self, node):
                func_name = self.visit(node.func)
                if type(node.func).__name__ == 'Attribute':
                    return func_name

                args_name = []
                for arg in node.args:
                    args_name.append(self.visit(arg))

                if func_name == 'ignore':
                    return args_name[0][1:-1]

                if func_name == 'concat':
                    return ''.join(args_name)

                if func_name == 'call':
                    res = []
                    sem = self.arch['function'][args_name[0][1:-1]]
                    if isinstance(sem, str):
                        sem = sem.split(";")
                    for expr in sem:
                        if expr[0] == '\n':
                            expr = expr[1:]
                        if len(expr) == 0:
                            continue

                        # cb = CodeBlock(CodeBlock.CMT_TYPE)
                        # cb.SetComment('/* call semantic: %s */\n' % expr)
                        # res.append(cb)

                        nodes = ast.parse(expr)
                        func_expr = self.visit(nodes)
                        if isinstance(func_expr, list):
                            res += func_expr
                        else:
                            res.append(func_expr)

                    return res

                if func_name == 'int_type':
                    assert(len(args_name) == 1)
                    return 'Expr::MakeConstInt(%s, %s)' % (args_name[0], args_name[0])

                if 'VariableExpression::Alloc' in func_name:
                    var_name = args_name[0][1:-1]
                    assert(not var_name in self.var_pool)
                    self.var_pool.append(var_name)
                if 'VariableExpression::Free' in func_name:
                    var_name = args_name[0][1:-1]
                    assert(var_name in self.var_pool)
                    self.var_pool.remove(args_name[0][1:-1])

                if 'VariableExpression' in func_name:
                    cb = CodeBlock(CodeBlock.EXPR_TYPE)
                    cb.AddGlbExpr(func_name % tuple(args_name))
                    return cb

                if 'GetPrefix' in func_name:
                    mask = ' | '.join(args_name)
                    if '!' in func_name:
                        return '!(rInsn.GetPrefix() & (%s))' % mask
                    return '%s & (%s)' % (func_name, mask)

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

                elif attr_name == 'base':
                    return 'expr_cast<MemoryExpression>(%s)->GetBaseExpression()' % value_name

                elif attr_name == 'offset':
                    return 'expr_cast<MemoryExpression>(%s)->GetOffsetExpression()' % value_name

                elif attr_name == 'size':

                    if value_name == 'rInsn':
                        get_pc_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(m_CpuInfo.GetRegisterByType(CpuInformation::ProgramPointerRegister, rInsn.GetMode()))'
                        return 'Expr::MakeConstInt(\n%s,\n%s)'\
                                % (Indent(get_pc_size_bit), Indent('rInsn.GetLength()'))

                    elif value_name.startswith('rInsn.GetOperand') or value_name.startswith('expr_cast<MemoryExpression>(rInsn.GetOperand'):
                        get_insn_size_bit = '(%s->GetBitSize())' % value_name
                        get_insn_size_byte = '(%s->GetBitSize() / 8)' % value_name
                        return 'Expr::MakeConstInt(\n%s,\n%s)'\
                                % (Indent(get_insn_size_bit), Indent(get_insn_size_byte))

                    else:
                        get_reg_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name
                        return 'Expr::MakeConstInt(\n%s,\n%s / 8)'\
                                % (Indent(get_reg_size_bit), Indent(get_reg_size_bit))

                elif attr_name == 'bit':
                    if value_name.startswith('rInsn.GetOperand') or value_name.startswith('expr_cast<MemoryExpression>(rInsn.GetOperand'):
                        return '%s->GetBitSize()' % value_name
                    else:
                        return 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name

                elif attr_name == 'byte':
                    if value_name.startswith('rInsn.GetOperand') or value_name.startswith('expr_cast<MemoryExpression>(rInsn.GetOperand'):
                        return '%s->GetBitSize() / 8' % value_name
                    else:
                        return 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name

                elif attr_name == 'mem':
                    get_reg_size_bit = 'm_CpuInfo.GetSizeOfRegisterInBit(%s)' % value_name
                    return 'Expr::MakeMem(%s, nullptr, Expr::MakeId(%s, &m_CpuInfo))' % (get_reg_size_bit, value_name)

                assert(0)

            def visit_Name(self, node):
                node_name = node.id

                if node_name in [ 'ignore', 'concat', 'call' ]:
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

                # Type
                elif node_name == 'int_type':
                    return node_name
                elif node_name.startswith('int_type'):
                    int_size = int(node_name[8:])
                    return 'Expr::MakeConstInt(%d, %d)' % (int_size, int_size)

                # Integer
                elif node_name == 'int':
                    return 'Expr::MakeConstInt(%s, %s)'
                elif node_name.startswith('int'):
                    int_size = int(node_name[3:])
                    return 'Expr::MakeConstInt(%d, %%s)' % int_size

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

                # Fonction name
                elif node_name == 'ite':
                    return 'Expr::MakeTernaryCond(%s,\n%s, %s)'

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
                elif node_name == 'rol':
                    return 'Expr::MakeBinOp(OperationExpression::OpRol, %s, %s)'
                elif node_name == 'ror':
                    return 'Expr::MakeBinOp(OperationExpression::OpRor, %s, %s)'
                elif node_name == 'sdiv':
                    return 'Expr::MakeBinOp(OperationExpression::OpSDiv, %s, %s)'
                elif node_name == 'smod':
                    return 'Expr::MakeBinOp(OperationExpression::OpSMod, %s, %s)'
                elif node_name == 'bit_cast':
                    return 'Expr::MakeBinOp(OperationExpression::OpBcast, %s, %s)'

                elif node_name == 'sgt': # s>
                    return 'Expr::MakeBinOp(ConditionExpression::CondSgt, %s, %s)'
                elif node_name == 'sge': # s>
                    return 'Expr::MakeBinOp(ConditionExpression::CondSge, %s, %s)'
                elif node_name == 'slt': # s<
                    return 'Expr::MakeBinOp(ConditionExpression::CondSlt, %s, %s)'
                elif node_name == 'sle': # s<=
                    return 'Expr::MakeBinOp(ConditionExpression::CondSle, %s, %s)'

                # TODO(KS): this code is architecture specific, move it to arch_<arch>.py
                elif node_name == 'is_byte_operation':
                    return 'rInsn.GetOperand(0)->GetBitSize() == 8'
                elif node_name == 'is_word_operation':
                    return 'rInsn.GetOperand(0)->GetBitSize() == 16'
                elif node_name == 'is_dword_operation':
                    return 'rInsn.GetOperand(0)->GetBitSize() == 32'
                elif node_name == 'is_qword_operation':
                    return 'rInsn.GetOperand(0)->GetBitSize() == 64'
                elif node_name == 'is_expr_id':
                    return 'Expr::TestKind(Expression::Id, %s)'
                elif node_name == 'is_expr_mem':
                    return 'Expr::TestKind(Expression::Mem, %s)'
                elif node_name == 'is_id_and_mem':
                    return '(Expr::TestKind(Expression::Id, %s) && Expr::TestKind(Expression::Mem, %s))'
                elif node_name == 'is_mem_and_id':
                    return '(Expr::TestKind(Expression::Mem, %s) && Expr::TestKind(Expression::Id, %s))'
                elif node_name == 'is_id_and_id':
                    return '(Expr::TestKind(Expression::Id, %s) && Expr::TestKind(Expression::Id, %s))'

                elif node_name == 'instruction_has_prefix':
                    return 'rInsn.GetPrefix()'
                elif node_name == 'instruction_has_no_prefix':
                    return '!rInsn.GetPrefix()'

                # TODO(KS): prefixes are arch dependent
                pfx = { 'rep': 'X86_Prefix_Rep', 'repz': 'X86_Prefix_Rep', 'repnz': 'X86_Prefix_RepNz' }
                if node_name in pfx:
                    return pfx[node_name]

                assert(0)

            def visit_Num(self, node):
                return '%#x' % node.n
                return 'Expr::MakeConstInt(0, %#x)' % node.n

            def visit_Str(self, node):
                return '"%s"' % node.s

            def visit_Print(self, node):
                assert(0)

            def visit_Expr(self, node):
                return self.visit(node.value)

            def __str__(self):
                return self.res

        res = ''
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

            # Handle new type of instructions semantic definition
            if isinstance(sem, str):
                sem = sem.split(";")
                if sem[-1] == "\n": del sem[-1]

            v = SemVisitor(self.arch, id_mapper)
            expr_res = []
            for expr in sem:
                if len(expr) == 0:
                    continue
                if expr[0] == '\n':
                    expr = expr[1:]
                #print 'DEBUG: %r' % expr
                v.reset()
                nodes = ast.parse(expr)
                cb = CodeBlock(CodeBlock.CMT_TYPE)
                cb.SetComment('/* semantic: %s */\n' % expr)
                expr_res.append(cb)
                expr_res += v.visit(nodes)

            sem_no = 0
            for expr in expr_res:
                res += expr.Format('AllExpr')

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
