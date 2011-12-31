#!/usr/bin/env python2

from pydusa import *

import sys
import struct

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
                r = int(input('Please select a loader: '))
                if r >= 0 and r <= len(ldrs):
                    ldr = ldrs[r]
            except(NameError):
                pass
        print 'Parsing using "%s"... ' % ldr.Name + '',
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
                r = int(input('Please select an architecture: '))
                if r >= 0 and r <= len(archs):
                    arch = archs[r]
            except(NameError):
                pass
        print 'You choose "%s"' % arch.Name

        # If the selected architecture has several mode and process mode
        # ask to the user which one he/she wants
        modes = arch.GetMode()
        if len(modes):
            print 'Mode:', len(modes)
            for md in modes:
                print '[%s] - %s' % (md.key(), md.data())
            while True:
                try:
                    r = int(input('Please select a mode: '))
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
                    r = int(input('Please select a processor model: '))
                    if r in pro_mdl:
                        arch.SetProcessorModel(r)
                        break
                except(NameError):
                    pass

        m.DisassembleAll(ldr, arch)

        for mem_area in m.Database:
            print 'MemoryArea: %s' % mem_area.Name
            for cell in mem_area:
                if cell.second == None:
                    continue
                addr = mem_area.MakeAddress(cell.first)
                raw_byte = ''
                raw_offset = cell.first
                for i in xrange(15):
                    if i >= len(cell.second):
                        raw_byte += '  '
                    else:
                        byte = struct.unpack('B', mem_area.Read(raw_offset, 1))
                        raw_byte += '%02x' % byte
                    raw_byte += ' '
                    raw_offset += 1
                print '  %s: %s %s ; %s' % (addr, raw_byte, cell.second, cell.second.Comment)


    except RuntimeError as e:
        print 'Exception occured %s' % e
