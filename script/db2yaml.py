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

def GenYamlTable(tbl, indent = 0):
    res = ''
    opcd = 0
    for insn in tbl.insn:
        if type(insn) == type([]):
            res += Indent('- opcode: %#04x\n' % opcd, indent)
            res += Indent('sub_opcodes:\n', indent + 1)
            for sub_insn in insn:
                sub_insn_str = str(sub_insn).split('\n')
                res += Indent('- ' + sub_insn_str[0] + '\n', indent + 2)
                for l in sub_insn_str[1:]:
                    res += Indent(l + '\n', indent + 3)
        else:
            res += Indent('- opcode: %#04x\n' % opcd, indent)
            res += Indent(str(insn), indent + 1) + '\n'
        opcd += 1
    return res

def GenYaml(tbls, grps, fpus):
    res = ''
    for tbl_name, tbl_data in sorted(tbls.items(), key=lambda a:a[0]):
        res += '%s:\n' % tbl_name
        res += GenYamlTable(tbl_data, 1)
    for grp_name, grp_data in sorted(grps.items(), key=lambda a:a[0]):
        res += '%s:\n' % grp_name
        res += GenYamlTable(grp_data)
    #for fpu_name, fpu_data in sorted(fpus.items(), key=lambda a:a[0]):
    #    res += '%s:\n' % fpu_name
    #    res += GenYamlTable(fpu_data)
    return res

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
                flag_data = i[1:]
                fmt_fl = [ [], [], [] ]
                fl_c = [ 'Carry' ]
                fl_p = [ 'Parity' ]
                fl_a = [ 'AuxCarry' ]
                fl_z = [ 'Zero' ]
                fl_s = [ 'Sign' ]
                fl_t = [ 'Trap' ]
                fl_i = [ 'Interrupt' ]
                fl_d = [ 'Direction' ]
                fl_o = [ 'Overflow' ]
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
                for fl in range(len(flag_data)):
                    if   flag_data[fl] == 'T': idx = 0
                    elif flag_data[fl] == 'D': idx = 1
                    elif flag_data[fl] == 'U': idx = 2
                    else: fmt_fl[idx] += map_fl[flag_data[fl]]

                self.test_flags = fmt_fl[0]
                self.update_flags = fmt_fl[1]
                self.clear_flags = fmt_fl[2]
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
            return 'invalid:\n'

        res = ''
        if self.mnemo == '':
            self.mnemo = 'none'
        res += 'mnemonic: "%s"\n' % self.mnemo

        if hasattr(self, 'oprd'):
            res += 'operand: [ %s ]\n' % ', '.join(['"%s"' % x for x in self.oprd])

        if hasattr(self, 'ref'):
            res += 'reference: "%s"\n' % self.ref.lower()

        if hasattr(self, 'constraint'):
            res += 'constraint: %s\n' % ', '.join(['"%s"' % x for x in self.constraint])

        if hasattr(self, 'op_type') and len(self.op_type):
            res += 'operation_type: %s\n' % self.op_type

        if hasattr(self, 'test_flags'):
            res += 'test_flags: [ %s ]\n' % ', '.join(['"%s"' % x for x in self.test_flags])
        if hasattr(self, 'update_flags'):
            res += 'update_flags: [ %s ]\n' % ', '.join(['"%s"' % x for x in self.update_flags])
        if hasattr(self, 'clear_flags'):
            res += 'clear_flags: [ %s ]\n' % ', '.join(['"%s"' % x for x in self.clear_flags])

        if len(self.attr) != 0:
            res += 'attr: [ %s ]\n' % ', '.join(['"%s"' % x for x in self.attr])

        if len(self.suffix) != 0:
            res += 'suffix: %s\n' % ', '.join(self.suffix)

        if len(self.prefix) != 0:
            res += 'prefix: %s\n' % ', '.join(self.prefix)

        if len(self.cpu_mdl) != 0:
            res += 'cpu_model: "%s"\n' % ', '.join(self.cpu_mdl)

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

    yaml = GenYaml(tables, groups, fpus)
    print '#\n\n' + yaml

if __name__ == "__main__":
    main()
