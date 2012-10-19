#!/usr/bin/env python

import sys, time

all_mnemo = set()
all_oprd  = set()
all_dec   = set()

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

def GenMethodName(type_name, opcd_no, in_class = False):
    meth_fmt = 'bool X86Architecture::%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)'
    if in_class == True:
         meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)'

    if opcd_no == None:
        return meth_fmt % 'Invalid'

    return meth_fmt % ('%s_%02x' % (type_name.capitalize().replace('Table', 'Opcode'), opcd_no))

def GenInstructionComment(insn, opcd, grps):
    res = '/*\n'
    res += 'Opcode: %02x\n' % opcd
    if type(insn) == list:
        for i in insn:
            res += str(i)
    elif hasattr(insn, 'name') and insn.name.startswith('FPU'):
        res += 'Group:\n'
        for g in grps[insn.grp.ref].insn:
            res += str(g)
        res += str(insn.tbl)
    else:
        res += str(insn)
        if hasattr(insn, 'ref') and insn.ref.startswith('GROUP'):
            grp_no = 0
            for grp_insn in grps[insn.ref].insn:
                res += '/%d\n' % grp_no
                grp_no += 1
                if type(grp_insn) == list:
                    for arr_insn in grp_insn:
                        res += Indent(str(arr_insn))
                else:
                    res += str(grp_insn)
    res += '*/\n'

    return res

def GenRead(var_name, addr, sz):
    res = ''

    res += 'u%d %s;\n' % (sz, var_name)
    res += 'rBinStrm.Read(%s, %s);\n' % (addr, var_name)
    res += '\n'
    return res

def GenBrace(code):
    res = ''

    res += '{\n'
    res += Indent(code)
    res += '}\n'
    return res

def GenCond(cond_type, cond, statm):
    res = ''

    if cond != None:
        res += '%s (%s)\n' % (cond_type, cond)
    else:
        res += '%s\n' % (cond_type)
    if statm[-1] != '\n':
        res += Indent(statm + '\n')
    else:
        res += GenBrace(statm)
    return res

def GenSwitch(cond, cases, default):
    res = ''

    res += 'switch (%s)\n' % cond
    res += '{\n'
    for case in cases:
        res += 'case %s:\n' % case[0]
        res += Indent(case[1])
        if case[2]:
            res += Indent('break;\n')
    res += 'default:\n'
    res += Indent('%s;\n' % default)
    res += '}\n'
    return res

def GenInstructionCond(insn):
    cond = []

    if len(insn.cpu_mdl):
        cond.append(' || '.join('m_CpuModel %s' % cm for cm in insn.cpu_mdl))

    for f in insn.attr:
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

    if len(insn.prefix):
        cond.append('Prefix == %s' % insn.prefix[0])

    if len(insn.suffix):
        cond.append('Suffix == %s' % insn.suffix[0])

    if len(cond) == 0:
        return None

    return ' && '.join(cond)

def GenInstruction(insn, grps, fpus):
    if type(insn) == list:
        return GenInstructionArray(insn, grps, fpus)
    else:
        return GenInstructionAlone(insn, grps, fpus)

def GenInstructionArray(insns, grps, fpus):
    res = ''
    cond_type = 'if'
    has_pref = False
    has_suff = False

    for insn in insns:
        if not has_pref and len(insn.prefix):
            res += GenRead('Prefix', 'Offset - 2', 8)
            has_pref = True
        if not has_suff and len(insn.suffix):
            res += GenRead('Suffix', 'Offset + 0', 8)
            has_suff = True

    for insn in reversed(insns):
        if insn.HasCond():
            cond = GenInstructionCond(insn)
            if cond_type == 'if' and cond == None:
                if insn != insns[0]:
                    raise Exception('No condition for if\n%s', str(insn))
                cond_type = 'else'
            res += GenCond(cond_type, cond, GenInstructionBody(insn, grps, fpus))
            cond_type = 'else if'
        else:
            if cond_type == 'if':
                raise Exception('No condition for if\n%s', str(insn))
            res += GenCond('else', None, GenInstructionBody(insn, grps, fpus))
            return res

    res += 'return false;\n'
    return res

def GenInstructionAlone(insn, grps, fpus):
    res = ''

    if len(insn.prefix):
        res += GenRead('Prefix', 'Offset - 2', 8)

    if len(insn.suffix):
        res += GenRead('Suffix', 'Offset + 0', 8)

    if hasattr(insn, 'ref'):
        return GenRef(insn, grps, fpus)

    elif insn.HasCond():
        cond = GenInstructionCond(insn)
        if cond == None:
            res += GenInstructionBody(insn, grps)
        else:
            res += GenCond('if', cond, GenInstructionBody(insn, grps, fpus))
            res += GenCond('else', None, 'return false;')

    else:
        res += GenInstructionBody(insn, grps, fpus)

    return res

def GenRef(insn, grps, fpus):
    res = ''

    if insn.ref.startswith('TABLEFP'):
        res += GenRead('Opcode', 'Offset', 8)
        res += 'rInsn.Length()++;\n'
        res += 'return (this->*m_%s[Opcode - 0xc0])(rBinStrm, Offset + 1, rInsn);\n' % insn.ref.capitalize()

    elif insn.ref.startswith('TABLE'):
        res += GenRead('Opcode', 'Offset', 8)
        res += 'rInsn.Length()++;\n'
        res += GenCond('if', 'Opcode + 1 > sizeof(m_%s)' % insn.ref.capitalize(), 'return false;')
        res += 'return (this->*m_%s[Opcode])(rBinStrm, Offset + 1, rInsn);\n' % insn.ref.capitalize()

    elif insn.ref.startswith('GROUP'):
        grp = grps[insn.ref]
        refs = []
        grp_no = 0
        for insn_g in grp.insn:
            case_statm = ''

            if type(insn_g) == list:
                case_statm += GenInstructionArray(insn_g, grps, fpus)

            elif hasattr(insn, 'oprd'):
                tmp_insn = insn_g
                tmp_insn.oprd = insn.oprd
                case_statm += GenInstruction(tmp_insn, grps, fpus)

            elif insn_g.mnemo == 'invalid':
                case_statm += 'return false;\n'

            else:
                case_statm += GenInstruction(insn_g, grps, fpus)

            # HACK: To generate scope for reading (prefix/suffix)
            if case_statm.find('Read') != -1:
                case_statm = GenBrace(case_statm)

            refs.append( ('%#x' % grp_no, case_statm, False) )
            grp_no += 1

        res += GenRead('ModRmByte', 'Offset', 8)
        res += 'x86::ModRM ModRm(ModRmByte);\n'
        res += GenSwitch('ModRm.Reg()', refs, 'return false')

    elif insn.ref.startswith('FPU'):
        fpu = fpus[insn.ref]
        res += GenRead('ModRmByte', 'Offset', 8)+GenCond(
                'if', 'ModRmByte < 0xc0',
                        GenRef(fpu.grp, grps, fpus))+GenCond(
                'else', None,
                        GenRef(fpu.tbl, grps, fpus)
                        )

    else:
        raise Exception('Unkown reference %s' % insn.ref)

    return res

def GenOperandMethod(oprd):
    res = 'Operand__%s(rBinStrm, Offset, rInsn)' % '_'.join(oprd)

    all_oprd.add('_'.join(oprd))

    return res

def GenInstructionBody(insn, grps, fpus):
    res = ''
    pfx_n = insn.IsPrefix()

    if insn.mnemo == 'invalid':
        res += 'return false;\n'

    elif pfx_n != None:
        res += 'rInsn.Length()++;\n'
        res += 'rInsn.Prefix() |= X86_Prefix_%s;\n' % insn.mnemo
        res += 'return Disassemble(rBinStrm, Offset + %s - 1, rInsn);\n' % pfx_n

    elif hasattr(insn, 'ref'):
        res += GenRef(insn, grps, fpus)

    else:
        res += 'rInsn.Length()++;\n'
        res += 'rInsn.SetOpcode(X86_Opcode_%s);\n' % insn.mnemo.capitalize()
        all_mnemo.add(insn.mnemo)

        if len(insn.flags) != 0:
            fmt_fl = [ [], [], [] ]
            fl_c = [ 'Instruction::FlCarry' ]
            fl_p = [ 'Instruction::FlParity' ]
            fl_a = [ 'Instruction::FlAuxCarry' ]
            fl_z = [ 'Instruction::FlZero' ]
            fl_s = [ 'Instruction::FlSign' ]
            fl_t = [ 'Instruction::FlTrap' ]
            fl_i = [ 'Instruction::FlInterrupt' ]
            fl_d = [ 'Instruction::FlDirection' ]
            fl_o = [ 'Instruction::FlOverflow' ]
            fl_0 = fl_c + fl_p + fl_a + fl_z + fl_s + fl_o
            fl_1 = fl_c + fl_p + fl_z + fl_s + fl_o
            fl_2 = fl_p + fl_a + fl_z + fl_s + fl_o
            map_fl = {
                    'c' : fl_c, 'p' : fl_p,
                    'a' : fl_a, 'z' : fl_z,
                    's' : fl_s, 't' : fl_t,
                    'i' : fl_i, 'd' : fl_d,
                    'o' : fl_o,
                    '*' : fl_0, '+' : fl_1, '-' : fl_2 }
            idx = None
            for i in range(len(insn.flags)):
                if   insn.flags[i] == 'T': idx = 0
                elif insn.flags[i] == 'D': idx = 1
                elif insn.flags[i] == 'U': idx = 2
                else: fmt_fl[idx] += map_fl[insn.flags[i]]

            if len(fmt_fl[0]) != 0:
                res += 'rInsn.SetTestedFlags(%s);\n' % ' | '.join(fmt_fl[0])
            if len(fmt_fl[1]) != 0:
                res += 'rInsn.SetUpdatedFlags(%s);\n' % ' | '.join(fmt_fl[1])
            if len(fmt_fl[2]) != 0:
                res += 'rInsn.SetClearedFlags(%s);\n' % ' | '.join(fmt_fl[2])

        if len(insn.op_type):
            all_op = []
            for op in insn.op_type:
                if op == 'jmp':
                    all_op.append('Instruction::OpJump')
                elif op == 'call':
                    all_op.append('Instruction::OpCall')
                elif op == 'ret':
                    all_op.append('Instruction::OpRet')
                elif op == 'cond':
                    all_op.append('Instruction::OpCond')
                else:
                    raise Exception('Unknown operation type %s' % insn.op_type)
            res += 'rInsn.SetOperationType(%s);\n' % (' | '.join(all_op))

        if hasattr(insn, 'oprd'):
                res += 'return %s;\n' % GenOperandMethod(insn.oprd)
        else:
            res += 'return true;\n'

    return res

def GenHeader(tables, groups, fpus):
    res = ''

    res += 'private:\n'
    res += Indent('typedef bool (X86Architecture:: *TDisassembler)(BinaryStream const&, TOffset, Instruction&);\n')

    for name, tbl in sorted(tables.items(), key=lambda a:a[0]):
        if 'FP' in name:
            opcd_no = 0xc0
            res += Indent('static const TDisassembler m_%s[0x40];\n' % name.capitalize())
            for opcd in tbl.insn:
                res += Indent('%s;\n' % GenMethodName(name, opcd_no, True))
                opcd_no += 1
            res += '\n'


        else:
            opcd_no = 0x0
            res += Indent('static const TDisassembler m_%s[0x100];\n' % name.capitalize())
            for opcd in tbl.insn:
                res += Indent('%s;\n' % GenMethodName(name, opcd_no, True))
                opcd_no += 1
            res += '\n'

    return res

def GenSource(tables, groups, fpus):
    res = ''

    for name, tbl in sorted(tables.items(), key=lambda a:a[0]):
        if 'FP' in name:
            opcd_no = 0xc0
            res += 'const X86Architecture::TDisassembler X86Architecture::m_%s[0x40] =\n' % name.capitalize()
            res += '{\n'
            tbl_elm = []
            for opcd in tbl.insn:
                tbl_elm.append(Indent('&X86Architecture::%s_%02x\n' % (name.capitalize().replace('Table', 'Opcode'), opcd_no))[:-1])
                opcd_no += 1
            res += ',\n'.join(tbl_elm)
            res += '\n};\n\n'

        else:
            opcd_no = 0x0
            res += 'const X86Architecture::TDisassembler X86Architecture::m_%s[0x100] =\n' % name.capitalize()
            res += '{\n'
            tbl_elm = []
            for opcd in tbl.insn:
                tbl_elm.append(Indent('&X86Architecture::%s_%02x\n' % (name.capitalize().replace('Table', 'Opcode'), opcd_no))[:-1])
                opcd_no += 1
            res += ',\n'.join(tbl_elm)
            res += '\n};\n\n'

    for name, tbl in sorted(tables.items(), key=lambda a:a[0]):

        # FPU instructions must be handled with care...
        if 'FP' in name:
            opcd_no = 0xc0
            for opcd in tbl.insn:
                res += GenInstructionComment(opcd, opcd_no, groups)
                res += '%s\n' % GenMethodName(name, opcd_no)
                res += GenBrace(Indent(GenInstruction(opcd, groups, fpus)))
                res += '\n'
                opcd_no += 1


        else:
            opcd_no = 0x0
            for opcd in tbl.insn:
                res += GenInstructionComment(opcd, opcd_no, groups)
                res += '%s\n' % GenMethodName(name, opcd_no)
                res += GenBrace(Indent(GenInstruction(opcd, groups, fpus)))
                res += '\n'
                opcd_no += 1

    return res

def GenOpcodeEnum(mnemos):
    res = ',\n'.join('X86_Opcode_%s' % x.capitalize() for x in sorted(mnemos)) + '\n'
    return 'enum X86Opcode\n' + GenBrace(res)[:-1] + ';\n'

def GenOperandDef(oprds):
    res = ''
    for oprd in oprds:
        if oprd == '':
            continue
        res += Indent('bool Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n' % oprd)
    return res

def GenOperandCode(oprds):
    res = ''
    for oprd in oprds:
        if oprd == '':
            continue
        res += 'bool X86Architecture::Operand__%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n' % oprd
        dec_op = []
        op_no = 0
        oprd = oprd.split('_')

        # HACK: Handling case with E?_I?.. is tedious, we have to determine the size
        # of modr/m (which can has sib and/or immXX)
        ei_hack = ''
        for o in oprd:
            if o[0] == 'I' and oprd[0][0] == 'E':
                ei_hack += 'size_t PrefixOpcodeLength = rInsn.GetLength();\n'

                for o in oprd:
                    if o[0] == 'I' and oprd[0][0] == 'E':
                        ei_hack += GenCond('if', '!Decode_%s(rBinStrm, Offset + (rInsn.GetLength() - PrefixOpcodeLength), rInsn, rInsn.Operand(%d))' % (o, op_no),\
                                'return false;')

                    else:
                        ei_hack += GenCond('if', '!Decode_%s(rBinStrm, Offset, rInsn, rInsn.Operand(%d))' % (o, op_no),\
                                'return false;')

                    all_dec.add('Decode_%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)' % o)
                    op_no += 1

                ei_hack += 'return true;\n'
                break
        if len(ei_hack):
            res += GenBrace(ei_hack)
            continue


        for o in oprd:
            dec_op.append('Decode_%s(rBinStrm, Offset, rInsn, rInsn.Operand(%d))' % (o, op_no))
            op_no += 1
            all_dec.add('Decode_%s(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, Operand* pOprd)' % o)

        res += GenBrace('return\n' + Indent(' &&\n'.join(dec_op) + ';\n'))
    return res

def GenOpcodeString(mnemos):
    res = ',\n'.join('"%s"' % x for x in sorted(mnemos)) + '\n'
    return 'const char *X86Architecture::m_Mnemonic[%#x] =\n' % len(mnemos) + GenBrace(res)[:-1] + ';\n'

class Instruction:
    def __init__(self, insn):
        insn = insn.lstrip(' \t')
        info = insn.split()

        self.mnemo = ''
        if not info[0][0] == '#':
            self.mnemo = info[0]
            info = info[1:]
        self.flags   = ''
        self.prefix  = []
        self.suffix  = []
        self.attr    = []
        self.cpu_mdl = []
        self.op_type = []

        for i in info[:]:
            if len(i) == 0:
                continue

            if i[0] == '!':
                self.attr.append(i[1:])
                info.remove(i)

            elif i[0] == '>' or i[0] == '=':
                op_len = 1
                op = i[0]
                if i[1] == '=':
                    op += i[1]
                    op_len += 1

                arch = op + ' X86_Arch_' + i[op_len:].capitalize()
                self.cpu_mdl.append(arch)
                info.remove(i)

            elif i[0] == '+':
                self.suffix.append(i[1:])
                info.remove(i)

            elif i[0] == '-':
                self.prefix.append(i[1:])
                info.remove(i)

            elif i[0] == '#':
                self.ref = i[1:]
                info.remove(i)

            elif i[0] == '?':
                self.flags = i[1:]
                info.remove(i)

            elif i[0] == '&':
                if not hasattr(self, 'constraint'):
                    self.constraint = []
                self.constraint.append(i[1:])
                info.remove(i)

            elif i[0] == '@':
                self.op_type.append(i[1:])
                info.remove(i)

        if len(info) >= 1:
            self.oprd = info[-1].split('_')

    def __str__(self):

        if self.mnemo == 'invalid':
            return '** INVALID **\n'

        res = ''
        res += '- %s\n' % self.mnemo

        if hasattr(self, 'oprd'):
            res += 'operand: %s\n' % self.oprd

        if hasattr(self, 'ref'):
            res += 'reference: %s\n' % self.ref

        if hasattr(self, 'constraint'):
            res += 'constraint: %s\n' % ', '.join(self.constraint)

        if hasattr(self, 'op_type'):
            res += 'OpType: %s\n' % self.op_type

        if len(self.flags) != 0:
            res += 'flags: %s\n' % self.flags

        if len(self.attr) != 0:
            res += 'attr: %s\n' % ' | '.join(self.attr)

        if len(self.suffix) != 0:
            res += 'suffix: %s\n' % ', '.join(self.suffix)

        if len(self.prefix) != 0:
            res += 'prefix: %s\n' % ', '.join(self.prefix)

        if len(self.cpu_mdl) != 0:
            res += 'CPU Model: %s\n' % '|| '.join(self.cpu_mdl)

        return res

    def IsPrefix(self):
        if not hasattr(self, 'constraint'):
            return None
        for c in self.constraint:
            if 'pfx' in c:
                return c[-1]
        return None

    def HasCond(self):
        if len(self.cpu_mdl) or len(self.attr) or len(self.prefix) or len(self.suffix):
            return True
        return False


def ParseInstruction(insn):
    return Instruction(insn)

def ParseArrayOfInstruction(array):
    array = array[1:-1]
    array = array.split(', ')
    res = []
    for element in array:
        res.append(ParseInstruction(element))
    return res

class Table:
    def __init__(self, tbl_name, tbl):
        self.insn = []
        self.name = tbl_name
        for e in tbl:
            if e[0] == '[':
                self.insn.append(ParseArrayOfInstruction(e))
            else:
                self.insn.append(ParseInstruction(e))

    def __str__(self):
        res = 'Table %s len: %x\n' % (self.name, len(self.insn))
        opcd_no = 0
        for e in self.insn:
            res += '%#02x:\n' % opcd_no
            if type(e) == list:
                for i in e:
                    res += '%s\n' % i
            else:
                res += '%s\n' % e
            opcd_no += 1
        return res

class Group:
    def __init__(self, grp_name, grp):
        self.insn = []
        self.name = grp_name
        for e in grp:
            if e[0] == '[':
                self.insn.append(ParseArrayOfInstruction(e))
            else:
                self.insn.append(ParseInstruction(e))

    def __str__(self):
        res = 'Group %s len: %x\n' % (self.name, len(self.insn))
        opcd_no = 0
        for e in self.insn:
            res += '/%d:\n' % opcd_no
            if type(e) == list:
                for i in e:
                    res += '%s\n' % i
            else:
                res += '%s\n' % e
            opcd_no += 1
        return res

class Fpu:
    def __init__(self, fpu_name, fpu):
        self.name = fpu_name
        self.grp  = Instruction(fpu[0])
        self.tbl  = Instruction(fpu[1])

    def __str__(self):
        res =  'FPU "%s"\n' % self.name
        res += 'Group:\n'
        res += '  %s\n' % self.grp
        res += 'Table:\n'
        res += '  %s\n' % self.tbl
        return res

def RemoveComment(line):
    if ';' in line:
        return line[0:line.find(';')].rstrip(' \t')
    return line.rstrip(' \t')

def main():
    data = open(sys.argv[1], 'r').read()

    lines = data.splitlines()
    line_no = 0


    tables = {}
    groups = {}
    fpus   = {}

    while line_no < len(lines):

        line = lines[line_no]
        line_no += 1

        line = RemoveComment(line)

        if len(line) == 0:
            continue

        if line[0] == '=':
            if '=TABLE' in line:
                table_name = line[1:]
                table = []
                while True:
                    if line_no >= len(lines):
                        raise Exception('Missing END for table?')
                    table_line = lines[line_no]
                    line_no += 1
                    table_line = RemoveComment(table_line)
                    if len(table_line) == 0:
                        continue
                    if table_line == 'END':
                        break
                    table.append(table_line)

                tables[table_name] = Table(table_name, table)

            elif '=GROUP' in line:
                group_name = line[1:]
                group = []
                while True:
                    if line_no >= len(lines):
                        raise Exception('Missing END for group?')
                    group_line = lines[line_no]
                    line_no += 1
                    group_line = RemoveComment(group_line)
                    if len(group_line) == 0:
                        continue
                    if group_line == 'END':
                        break
                    group.append(group_line)

                groups[group_name] = Group(group_name, group)

            elif '=FPU' in line:
                fpu_name = line[1:]
                fpu = []
                while True:
                    if line_no >= len(lines):
                        raise Exception('Missing END for fpu?')
                    fpu_line = lines[line_no]
                    line_no += 1
                    fpu_line = RemoveComment(fpu_line)
                    if len(fpu_line) == 0:
                        continue
                    if fpu_line == 'END':
                        break
                    fpu.append(fpu_line)

                fpus[fpu_name] = Fpu(fpu_name, fpu)

    hdr = GenHeader(tables, groups, fpus)
    src = GenSource(tables, groups, fpus)
    enm = GenOpcodeEnum(all_mnemo)
    mns = GenOpcodeString(all_mnemo)
    opd = GenOperandDef(all_oprd)
    opc = GenOperandCode(all_oprd)

    x86_hpp = open('x86_opcode.ipp', 'w')
    x86_cpp = open('x86_opcode.cpp', 'w')

    banner = '/* This file has been automatically generated, you must _NOT_ edit it directly. (%s) */\n' % time.ctime()

    x86_hpp.write(banner)
    x86_hpp.write(enm)
    x86_hpp.write(hdr)
    x86_hpp.write(opd)

    x86_cpp.write(banner)
    x86_cpp.write('#include "x86_architecture.hpp"\n')
    x86_cpp.write(mns)
    x86_cpp.write(src)
    x86_cpp.write(opc)

    print('Remaining operand method to implement')
    for dec in sorted(all_dec):
        print(dec)

if __name__ == "__main__":
    main()
