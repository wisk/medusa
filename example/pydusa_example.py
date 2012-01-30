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
        print 'Interpreting file using "%s"... ' % ldr.Name + '',
        ldr.Map()
        print 'Done'

        # Get all architectures and ask to the user which one he/she wants
        archs = m.Architectures

        arch = ldr.GetMainArchitecture(archs)

        if arch == None:

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

        print 'Disassembling file using "%s"...' % arch.Name

        cfg_mdl = ConfigurationModel()

        arch.FillConfigurationModel(cfg_mdl)
        ldr.Configure(cfg_mdl.Configuration)
        arch.UseConfiguration(cfg_mdl.Configuration)

        print '%s' % cfg_mdl.Configuration

        print 'EntryPoint: %s' % ldr.EntryPoint

        m.Disassemble(ldr, arch)

        for mem_area in m.Database:
            print 'MemoryArea: %s' % mem_area.Name
            for cell in mem_area:
                if cell.second == None:
                    continue

                addr = mem_area.MakeAddress(cell.first)

                xrefs = m.Database.XRefs
                xr_ad = xrefs.From(addr)
                for ad in xr_ad:
                    print('; XREF: %s' % ad)


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
