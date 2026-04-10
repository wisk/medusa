#!/usr/bin/env python

from helper import *

import sys
import yaml

def main():

    for arch_file in sys.argv[1:]:
        with open('../arch/%s.yaml' % arch_file) as f:
            d = yaml.safe_load(f)

        arch_name = d['architecture_information']['name']
        mod = __import__('arch_%s' % arch_name)
        conv_class = getattr(mod, '%sArchConvertion' % arch_name.capitalize())
        conv = conv_class(d)

        hdr = conv.GenerateHeader()
        src = conv.GenerateSource()
        enm = conv.GenerateOpcodeEnum()
        mns = conv.GenerateOpcodeString()
        opd = conv.GenerateOperandDefinition()
        opc = conv.GenerateOperandCode()

        with open('%s_opcode.ipp' % arch_name, 'w') as arch_hpp:
            arch_hpp.write(conv.GenerateBanner())
            arch_hpp.write(enm)
            arch_hpp.write(hdr)
            arch_hpp.write(opd)

        with open('%s_mnemonic.cpp' % arch_name, 'w') as arch_cpp:
            arch_cpp.write(conv.GenerateBanner())
            arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
            arch_cpp.write(mns)

        with open('%s_operand.cpp' % arch_name, 'w') as arch_cpp:
            arch_cpp.write(conv.GenerateBanner())
            arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
            arch_cpp.write(opc)

        if isinstance(src, dict):
            for name, code in src.items():
                with open('%s_opcode_%s.cpp' % (arch_name, name), 'w') as arch_cpp:
                    arch_cpp.write(conv.GenerateBanner())
                    arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
                    arch_cpp.write(code)
        else:
            with open('%s_opcode.cpp' % arch_name, 'w') as arch_cpp:
                arch_cpp.write(conv.GenerateBanner())
                arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
                arch_cpp.write(src)

if __name__ == "__main__":
    main()
