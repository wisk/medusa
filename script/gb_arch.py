#!/usr/env/bin python

# Give http://imrannazar.com/Gameboy-Z80-Opcode-Map source code as argument

import sys
import string

f = open(sys.argv[1])

insn = []

for line in f:
    if line.find("<td><abbr title=\"") > -1:
        beg = line.find('>')
        beg = line.find('>', beg + 1)
        beg += 1
        end = line.find(' ', beg)
        if end == -1:
            end = line.find('<', beg)
        if line[beg:end] == "XX" or line[beg:end] == "Ext":
            insn.append("Invalid")
        else:
            insn.append(string.capitalize(line[beg:end]))

insn = ["&Insn_%s, " % i for i in insn]

count = 0;
step = 8
new_insn = []
for i in range(0, len(insn), step):
    new_insn.append(insn[i:i+step])

for l in new_insn:
    for i in l:
        print '%-*s' % (15, i),
    print '/* %02x */' % count
    count += 8
    if count > 0xff:
        count = 0
