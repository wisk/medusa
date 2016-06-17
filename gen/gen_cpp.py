#!/usr/bin/env python

from helper import *

import sys
import yaml

def main():

    for arch_file in sys.argv[1:]:
        f = open('../arch/%s.yaml' % arch_file)
        d = yaml.load(f)

        conv = None

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

        arch_hpp = open('%s_opcode.ipp' % arch_name, 'w')
        arch_hpp.write(conv.GenerateBanner())
        arch_hpp.write(enm)
        arch_hpp.write(hdr)
        arch_hpp.write(opd)

        arch_cpp = open('%s_mnemonic.cpp' % arch_name, 'w')
        arch_cpp.write(conv.GenerateBanner())
        arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
        arch_cpp.write(mns)

	arch_cpp = open('%s_operand.cpp' % arch_name, 'w')
	arch_cpp.write(conv.GenerateBanner())
	arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
        arch_cpp.write(opc)

	if isinstance(src, dict):
		for name, code in src.items():
			arch_cpp = open('%s_opcode_%s.cpp' % (arch_name, name), 'w')
			arch_cpp.write(conv.GenerateBanner())
			arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
			arch_cpp.write(code)
	else:
		arch_cpp = open('%s_opcode.cpp' % arch_name, 'w')
		arch_cpp.write(conv.GenerateBanner())
		arch_cpp.write('#include "%s_architecture.hpp"\n' % arch_name)
		arch_cpp.write(src)

if __name__ == "__main__":
    main()
