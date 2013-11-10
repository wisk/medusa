#!/usr/bin/env python

txt_file = 'arm.txt'

def is_format(s):
    if 'ARM' in s:
        return False
    return s[0] >= 'A' and s[0] <= 'Z' and (s[1] >= 'A' and s[1] <= 'Z') or s[1] == '<' and not 'Special case' in s

def is_bitfield(s):
    return '15 14 13 12' in s

all_insn = []

with open(txt_file, 'r+') as f:
    lines = f.readlines()

    i = 0

    while i < len(lines):
        if 'Alphabetical list of instructions' in lines[i]:
            break
        i += 1

    while i < len(lines):
        line = lines[i].strip()

        if len(line) == 0:
            i += 1
            continue

        insn = None
        if line.startswith('Encoding '):

            words = line.split()[1:]

            if '/' in words:
                insn = [ { 'format': None, 'encoding' : None, 'attribute': [], 'mode': [], 'processor' : [], 'semantic' : [] }, { 'format': None, 'encoding' : None, 'attribute': [], 'mode': [], 'processor' : [], 'semantic' : [] }
]
            else:
                insn = { 'format': None, 'encoding' : None, 'attribute': [], 'mode': [], 'processor': [], 'semantic' : [] }

            procs = []
            for m in words:
                #print(m)
                if m.startswith('ARM'):
                    if m[-1] == ',':
                        m = m[:-1]
                    procs.append(m)

            if type(insn) == list:
                insn[0]['mode'] = words[0]
                insn[1]['mode'] = words[2]
                insn[0]['processor'] = procs
                insn[1]['processor'] = procs
            else:
                insn['mode'] = words[0]
                insn['processor'] = procs

            i += 1
            while i < len(lines):
                line = lines[i].strip()

                if line.startswith('Encoding'):
                    i -= 1
                    break

                if len(line) <= 1:
                    i += 1
                    continue

                if is_format(line):
                    it_blk = False
                    if 'Outside' in line:
                        it_blk = False
                    elif 'Inside' in line:
                        it_blk = True
                    off = line.find('    ')
                    if off != -1:
                        line = line[:off]

                    fmt = "'%s'" % line

                    if not it_blk:
                        if type(insn) == list:
                            if len(insn[0]['attribute']):
                                insn[1]['attribute'].append('support_it_block')
                            else:
                                insn[0]['attribute'].append('support_it_block')

                            if insn[0]['format']:
                                insn[1]['format'] = fmt
                            else:
                                insn[0]['format'] = fmt
                        else:
                            insn['format'] = fmt
                            if it_blk:
                                insn['attribute'].append('support_it_block')

                if is_bitfield(line):
                    enc = lines[i + 1].strip()
                    if len(enc) == 0:
                        enc = lines[i + 2].strip()
                    enc_fmt = []
                    for elem in enc.split():
                        if elem == '0' or elem == '1' or elem[0] == '(':
                            enc_fmt.append(elem)
                        else:
                            enc_fmt.append('*%s' % elem)
                    enc = '[ %s ]' % ','.join(enc_fmt)

                    if type(insn) == list:
                        if insn[0]['encoding']:
                            insn[1]['encoding'] = enc
                        else:
                            insn[0]['encoding'] = enc
                    else:
                        insn['encoding'] = enc

                if line == 'Assembler syntax':
                    break

                i += 1

        if insn and type(insn) != list:
            if insn['encoding']:
                all_insn.append(insn)

        if type(insn) == list and (len(insn[0]) + len(insn[1])):
            if not insn[1]['format']:
                insn[1]['format'] = insn[0]['format']
            if insn[0]['encoding']:
                all_insn.append(insn[0])
            if insn[1]['encoding']:
                all_insn.append(insn[1])

        if line == "The System Level Programmers' Model":
            break

        i += 1

print('arch_info:')
print('  name: arm')
print('')
print('insn:')
print('')
para = '- '
for insn in all_insn:
    for k,v in insn.items():
        print('  %s%s: %s' % (para,k,v))
        para = '  '
    print('')
    para = '- '

print('# %d' % len(all_insn))
