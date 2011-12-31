#!/usr/bin/env python2

from pydusa import *

import sys
import struct
import readline



m    = None # medusa::Medusa
ldr  = None # medusa::Loader
arch = None # medusa::Architecture

def mem_area_list(cmd):
    print '%- 12s| %-24s | %-24s | %-24s | %-24s | %s' % ('Name', 'Physical Base', 'Physical Size', 'Virtual Base', 'Virtual Size', 'Access')
    for mem_area in m.Database:
        ps = mem_area.PhysicalSize
        vs = mem_area.VirtualSize

        if ps == None:
            ps = 'None'
        else:
            ps = hex(ps)

        if vs == None:
            vs = 'None'
        else:
            vs = hex(vs)

        print '%-12s| %-24s | %-24s | %-24s | %-24s | %#010x' %\
                (mem_area.Name, mem_area.PhysicalBase, ps, mem_area.VirtualBase, vs, len(mem_area))

def mem_area_dis(cmd):

    if len(cmd) < 2:
        print 'md <mem_area name> [insn no]'
        return

    mem_area_name = cmd[1]

    insn_off = 0x0
    if len(cmd) > 2 and cmd[2] != '':
        if cmd[2].startswith('0x'):
            insn_off = int(cmd[2], 0x10)
        else:
            insn_off = int(cmd[2], 10)

    insn_no = 0xffffffff
    if len(cmd) > 3 and cmd[3] != '':
        if cmd[3].startswith('0x'):
            insn_no = int(cmd[3], 0x10)
        else:
            insn_no = int(cmd[3], 10)

    for mem_area in m.Database:
        if mem_area_name != mem_area.Name:
            continue
        for cell in mem_area:
            if cell.second == None:
                continue

            if insn_off != 0x0:
                insn_off -= 1
                continue

            if insn_no == 0x0:
                break
            insn_no -= 1

            addr = mem_area.MakeAddress(cell.first)
            raw_offset = cell.first
            raw_byte = ''
            for i in xrange(15):
                if i >= len(cell.second):
                    raw_byte += '  '
                else:
                    byte = struct.unpack('B', mem_area.Read(raw_offset, 1))
                    raw_byte += '%02x' % byte
                raw_byte += ' '
                raw_offset += 1
            print '  %s: %s %s ; %s' % (addr, raw_byte, cell.second, cell.second.Comment)
        break

def handle_command(s):
    cmd_handler = { 'ml' : mem_area_list, 'md' : mem_area_dis }

    lines = s.split(' ')
    try:
        cmd_handler[ lines[0] ](lines)
    except(KeyError):
        print 'Command not found'



if __name__ == "__main__":

    try:

        # Initialization
        Linux_StdOutWorkaround()
        m = Medusa()
        m.Open(sys.argv[1])
        m.LoadModules(sys.argv[2])
        print ''

        # Get all supported loaders and ask to the user which one he/she wants
        ldrs = m.SupportedLoaders
        if len(ldrs) == 0:
            print 'There is no available loader'
            exit()

        i = 0
        print 'Loaders:', len(ldrs)
        for l in ldrs:
            print '[%d] - %s' % (i, l.Name)
            i += 1
        ldr = None
        while ldr == None:
            try:
                r = int(raw_input('Please select a loader: '))
                if r >= 0 and r <= len(ldrs):
                    ldr = ldrs[r]
            except(NameError):
                pass
        print 'Parsing using "%s"...' % ldr.Name + ' ',
        ldr.Map()
        print 'Done'

        # Get all architectures and ask to the user which one he/she wants
        archs = m.Architectures
        i = 0
        print 'Architectures:', len(archs)
        for ar in archs:
            print '[%d] - %s' % (i, ar.Name)
            i += 1
        arch = None
        while arch == None:
            try:
                r = int(raw_input('Please select an architecture: '))
                if r >= 0 and r <= len(archs):
                    arch = archs[r]
            except(NameError):
                pass

        # If the selected architecture has several mode and process mode
        # ask to the user which one he/she wants
        modes = arch.GetMode()
        if len(modes):
            print 'Mode:', len(modes)
            for md in modes:
                print '[%s] - %s' % (md.key(), md.data())
            while True:
                try:
                    r = int(raw_input('Please select a mode: '))
                    if r in modes:
                        arch.SetMode(r)
                        break
                except NameError:
                    pass

        # the same with processor model
        pro_mdl = arch.GetProcessorModel()
        if len(pro_mdl):
            print 'Processor Model:', len(pro_mdl)
            for pm in pro_mdl:
                print '[%s] - %s' % (pm.key(), pm.data())
            while True:
                try:
                    r = int(raw_input('Please select a processor model: '))
                    if r in pro_mdl:
                        arch.SetProcessorModel(r)
                        break
                except(NameError):
                    pass

        print 'Disassembling using %s...' % arch.Name + ' ',
        m.DisassembleAll(ldr, arch)
        print 'Done'

        while True:
            cmd = None
            try:
                cmd = raw_input('pydusa> ')
            except(EOFError):
                break

            if cmd == 'quit' or cmd == 'exit':
                break

            handle_command(cmd)



    except RuntimeError as e:
        print 'Exception occured %s' % e
