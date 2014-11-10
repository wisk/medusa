from arch import ArchConvertion
from helper import *

from compiler.ast import flatten

import string

class ArmArchConvertion(ArchConvertion):
    def __init__(self, arch):
        ArchConvertion.__init__(self, arch)
        self.all_mnemo = set()

        all_instructions = self.arch['instruction']

        self.arm_insns = []
        self.thumb_insns = []

        for insn in all_instructions:

            # We need to flatten the encoding array
            insn['encoding'] = flatten(insn['encoding'])

            # Check if instruction is valid
            self.__ARM_VerifyInstruction(insn)

            # Gather all mnemonics
            self.all_mnemo.add(self.__ARM_GetMnemonic(insn).capitalize())

            if insn['mode'][0] == 'A':
                self.arm_insns.append(insn)
            elif insn['mode'][0] == 'T':
                self.thumb_insns.append(insn)

        self.arm_insns.sort(key=lambda insn: insn['encoding'])
        self.thumb_insns.sort(key=lambda insn: insn['encoding'])

    def __ARM_VerifyInstruction(self, insn):
        enc = insn['encoding']
        if len(enc) != 16 and len(enc) != 32:
            print 'Invalid instruction "%s", encoding: %s, length: %d' % (insn['format'], insn['encoding'], len(insn['encoding']))

    def __ARM_Mangle(insn):
        encoding = []
        for bit in insn['encoding']:
            if type(bit) == int:
                encoding.append(str(bit))
            elif bit[0] == '(':
                encoding.append(bit[1])
            else:
                encoding.append(bit)
        return '%s_%s' % (insn['mode'], '_'.join(encoding))

    def __ARM_GetMnemonic(self, insn):
        fmt = insn['format']
        res = ''
        for c in fmt:
            if not c in string.ascii_letters+string.digits:
                break
            res += c
        return res

    def __ARM_GetMask(self, insn):
        enc = insn['encoding']
        res = 0x0
        off = 0x0
        for bit in enc[::-1]:
            if bit in [ 0, 1, '(0)', '(1)' ]:
                res |= (1 << off)
            off += 1
        return res

    def __ARM_GetValue(self, insn):
        enc = insn['encoding']
        res = 0x0
        off = 0x0
        for bit in enc[::-1]:
            if bit in [ 1, '(1)' ]:
                res |= (1 << off)
            off += 1
        return res

    def __ARM_GetSize(self, insn):
        return len(insn['encoding'])

    def __ARM_ExtractField(self, insn):
        fmt = insn['format']
        fields = []

        #print fmt

        off = fmt.find(' ')
        if off == -1:
            off = len(fmt)

        full_mnem = fmt[:off]
        all_oprds = fmt[off+1:]

        # mnem{field}
        beg = full_mnem.find('{')
        end = full_mnem.find('}')
        if beg != -1 and end != -1:
            fields.append(full_mnem[beg+1:end])
            full_mnem[end+1:]

        # mnem<field0><field1>
        # TODO handle .field
        while len(full_mnem):
            beg = full_mnem.find('<')
            end = full_mnem.find('>')

            if beg == -1:
                break
            if end == -1:
                raise Exception('Unclose field %s' % insn)
            fields.append(full_mnem[beg+1:end])
            full_mnem = full_mnem[end+1:]

        oprds = all_oprds.split(',')
        e = len(oprds)
        i = 0
        while i < e:
            oprd = oprds[i].strip()
            if len(oprd) == 0:
                i += 1
                continue

            if oprd[-1] == '{':
                fields.append(oprds[i].strip() + ',' + oprds[i+1].strip())
                i += 1
            elif oprd[0] == '[' and not ']' in oprd:
                fields.append(oprds[i].strip() + ',' + oprds[i+1].strip())
                i += 1
            else:
                fields.append(oprd)
            i += 1

        return fields

    def __ARM_GenerateExtractBits(self, insn, pattern):
        enc = insn['encoding']
        beg = 0
        end = 0
        off = 0
        found = False
        for bf in enc[::-1]:
            if bf == pattern and not found:
                beg = off
                found = True

            elif bf != pattern and found:
                end = off - 1
                break
            off += 1

        if not found:
            raise Exception('Unable to generate extract bits for %s -  %s' % (pattern, insn))

        #if end != 0 and end <= beg:
        #    raise Exception('Invalid bit %d - %d' % (beg, end))

        if end == 0 and enc[0] == pattern:
            end = len(enc) - 1

        if beg == end:
            end = 0

        if end == 0:
            return 'ExtractBit<%d>(Opcode)' % beg

        return 'ExtractBits<%d, %d>(Opcode)' % (beg, end)

    def __ARM_GenerateExtractBitsSigned(self, insn, pattern, scale):
        enc = insn['encoding']
        beg = 0
        end = 0
        off = 0
        found = False
        for bf in enc[::-1]:
            if bf == pattern and not found:
                beg = off
                found = True

            if bf != pattern and found:
                end = off - 1
                break
            off += 1

        if not found:
            raise Exception('Unable to generate extract bits for %s -  %s' % (pattern, insn))

        if end != 0 and end <= beg:
            raise Exception('Invalid bit %d - %d' % (beg, end))

        if end == 0 and enc[0] == pattern:
            end = len(enc) - 1

        if end == 0:
            return 'SignExtend<s64, %d>(ExtractBit<%d>(Opcode) << %d)' % (beg + scale + 1, beg, scale)

        return 'SignExtend<s64, %d>(ExtractBits<%d, %d>(Opcode) << %d)' % (end + scale + 1, beg, end, scale)

    def __ARM_GenerateInstruction(self, insn):
        res = ''
        res += 'rInsn.SetName("%s");\n' % self.__ARM_GetMnemonic(insn)
        res += 'rInsn.SetOpcode(ARM_Opcode_%s);\n' % self.__ARM_GetMnemonic(insn).capitalize()
        res += 'rInsn.Length() += %d;\n' % (self.__ARM_GetSize(insn) / 8)

        map_op_type = { 'jmp' : 'Instruction::JumpType', 'call' : 'Instruction::CallType' }
        attrs = []
        for attr in insn['attribute']:
            if attr in map_op_type:
                attrs.append(map_op_type[attr])

        if len(attrs):
            res += 'rInsn.SubType() |= %s;\n' % ' | '.join(attrs)

        field_mapper = { 'c': ('c', 'rInsn.SetTestedFlags(%s);\n') }
        insn_fields = self.__ARM_ExtractField(insn)

        #print insn_fields

        # Test condition bits
        if 'c' in insn_fields and 'c' in insn['encoding']:
            res += Indent('u8 CondField = %s;\n' % self.__ARM_GenerateExtractBits(insn, 'c'), 0)
            res += Indent('rInsn.SetTestedFlags(CondField);\n', 0)
            res += Indent(self._GenerateCondition('if', 'CondField != 0xe',
                    'rInsn.SubType() |= Instruction::ConditionalType;'), 0)

        if 'S' in insn_fields and 'S' in insn['encoding']:
            res += Indent(self._GenerateCondition('if', self.__ARM_GenerateExtractBits(insn, 'S'), 'rInsn.Prefix() |= ARM_Prefix_S;'), 0)

        def SetOperand(oprd_idx, oprd_type, oprd_value, oprd_reg, oprd_secreg):
            oprd_str = ''
            oprd_str += Indent('auto pOprd%d = rInsn.Operand(%d);\n' % (oprd_idx, oprd_idx), 0)
            oprd_str += Indent('pOprd%d->SetType(%s);\n' % (oprd_idx, oprd_type), 0)
            if oprd_value:
                oprd_str += Indent('pOprd%d->SetValue(%s);\n' % (oprd_idx, oprd_value), 0)
            if oprd_reg:
                oprd_str += Indent('pOprd%d->SetReg(%s);\n' % (oprd_idx, oprd_reg), 0)
            if oprd_secreg:
                oprd_str += Indent('pOprd%d->SetSecReg(%s);\n' % (oprd_idx, oprd_secreg), 0)
            return oprd_str

        oprd_cnt = 0
        for field in insn_fields:
            if oprd_cnt > 4:
                break

            if field[-1] == '!':
                res += Indent('rInsn.Prefix() |= ARM_Prefix_W; /* TODO: Handle writeback (!) */\n', 0)
                field = field[:-1]

            if field.endswith('{!}'):
                res += Indent('rInsn.Prefix() |= ARM_Prefix_W; /* TODO: Handle writeback ({!}) */\n', 0)
                field = field[:-3]

            if field[0] == '<' and field[-1] == '>':
                field = field[1:-1]

            # Instruction attributes
            if len(field) == 1:
                pass

            # Operand deref
            elif field[0] == '[' and field[-1] == ']':
                field = field[1:-1]

                if ',' in field:
                    first, second = field.split(',')

                    reg = ''
                    imm = None
                    secreg = None
                    types = [ 'O_MEM32' ]

                    if first == 'PC' or first == 'SP':
                        reg = 'ARM_Reg%s' % first
                        types.append('O_REG32')
                    elif first == '<Rn>':
                        reg = '1 << %s' % self.__ARM_GenerateExtractBits(insn, 'n')
                        types.append('O_REG32')

                    if second[0] == '#':

                        # We need a special handling for [PC,#imm]
                        if first == 'PC':
                            res += self._GenerateRead('DstVal', 'Offset + 8 + %s' % self.__ARM_GenerateExtractBits(insn, 'i'), 32)
                            types = [] # We empty le types list because we want it to be treated as immediate
                            types.append('O_ABS32')
                            imm = 'DstVal'
                        else:
                            imm = self.__ARM_GenerateExtractBits(insn, 'i')
                            types.append('O_DISP32')
                    elif second == '<Rm>':
                        secreg = '1 << %s' % self.__ARM_GenerateExtractBits(insn, 'm')
                        types.append('O_SREG')

                    res += SetOperand(oprd_cnt, ' | '.join(types), imm, reg, secreg)
                    oprd_cnt += 1

                else:
                    if field[0] == '<' and field[-1] == '>':
                        field = field[1:-1]

                    # deref pc/sp
                    if field == 'PC' or field == 'SP':
                        res += SetOperand(oprd_cnt, 'O_MEM32 | O_REG32', None, 'ARM_Reg%s' % field, None)

                        if field == 'PC':
                            res += Indent(self._GenerateCondition('if', 'pOprd%d->GetReg() & ARM_RegPC' % oprd_cnt, 'rInsn.SubType() |= Instruction::JumpType;'), 0)

                        oprd_cnt += 1

                    elif field[0] == 'R':
                        pattern = field[1:].lower()
                        res += SetOperand(oprd_cnt, 'O_MEM32 | O_REG32', None, '1 << %s' % self.__ARM_GenerateExtractBits(insn, pattern), None)

                        if field == 'Rd':
                            res += Indent(self._GenerateCondition('if', 'pOprd%d->GetReg() & ARM_RegPC' % oprd_cnt, 'rInsn.SubType() |= Instruction::JumpType;'), 0)

                        oprd_cnt += 1

            # Operand defined register
            elif field == 'PC' or field == 'SP':
                res += SetOperand(oprd_cnt, 'O_REG32', None, 'ARM_Reg%s' % field, None)
                oprd_cnt += 1

            # Operand register
            elif field[0] == 'R':

                rp = field[1:].lower()
                if field == 'Rt2' and not rp in insn['encoding']:
                    res += Indent('/* TODO: Handle Rt2 without encoding */\n', 0)
                    continue

                res += SetOperand(oprd_cnt, 'O_REG32', None, '1 << %s' % self.__ARM_GenerateExtractBits(insn, rp), None)

                if field == 'Rd' or field == 'Rt':
                    res += Indent(self._GenerateCondition('if', 'pOprd%d->GetReg() & ARM_RegPC' % oprd_cnt, 'rInsn.SubType() |= Instruction::JumpType;'), 0)
                oprd_cnt += 1

            # Operand branch
            elif field == 'label':
                if insn['mode'][0] == 'A':
                    scale = 2
                elif insn['mode'][0] == 'T':
                    scale = 1
                else:
                    raise Exception('Unknown scale')

                if 'J1' in insn['encoding'] and 'J2' in insn['encoding']:
                    res += Indent('/* TODO: Handle ih:J1:J2:il operand */\n', 0)
                else:
                    # HACK: ARM add 4 bytes
                    res += SetOperand(oprd_cnt, 'O_REL32', self.__ARM_GenerateExtractBitsSigned(insn, 'i', scale) + ' + 4', None, None)
                    oprd_cnt += 1

            # Operand registers
            elif field == 'registers':

                if 'r' in insn['encoding']:
                    pattern = 'r'
                else:
                    pattern = 't'
                res += SetOperand(oprd_cnt, 'O_REG32', None, '1 << %s' % self.__ARM_GenerateExtractBits(insn, pattern), None)
                oprd_cnt += 1

            # Operand constant (ARMExpandImm TODO ThumbExpandImm!)
            elif field == '#<const>' or field.startswith('#<imm') or field.startswith('#+/-<imm'):

                pattern = 'i'

                if not 'i' in insn['encoding']:
                    pattern = 'size'
                    res += SetOperand(oprd_cnt, 'O_IMM32', self.__ARM_GenerateExtractBits(insn, pattern), None, None)
                else:
                    res += Indent('u32 RawImm = %s;\n' % self.__ARM_GenerateExtractBits(insn, pattern), 0)
                    res += Indent('u32 Imm = UnsignedRotateRight(ExtractBits<0, 7>(RawImm), (2 * ExtractBits<8, 11>(RawImm)) % 32);\n', 0)

                    res += SetOperand(oprd_cnt, 'O_IMM32', 'Imm', None, None)
                oprd_cnt += 1

            # Operand unhandled
            else:
                print 'Unable to handle field: "%s"' % field

        id_mapper = {
                'r0':'ARM_Reg0','r1':'ARM_Reg1','r2':'ARM_Reg2','r3':'ARM_Reg3',
                'r4':'ARM_Reg4','r5':'ARM_Reg5','r6':'ARM_Reg6','r7':'ARM_Reg7',
                'r8':'ARM_Reg8','r9':'ARM_Reg9','r10':'ARM_Reg10','r11':'ARM_Reg11',
                'r12':'ARM_Reg12','r13':'ARM_Reg13','r14':'ARM_Reg14','r15':'ARM_Reg15',
                'pc':'ARM_RegPC','sp':'ARM_RegSP'

                }


        if 'semantic' in insn:
            res += self._ConvertSemanticToCode(insn, insn['semantic'], id_mapper)

        res += 'return true;\n'

        return self.__ARM_GenerateMethodPrototype(insn, False) + '\n' + self._GenerateBrace(res)

    def __ARM_GenerateInstructionComment(self, insn):
        return '// %s - %s\n' % (insn['format'], insn['encoding'])

    def __ARM_GenerateMethodName(self, insn):
        mnem  = self.__ARM_GetMnemonic(insn)
        mode  = insn['mode']
        mask  = self.__ARM_GetMask(insn)
        value = self.__ARM_GetValue(insn)
        return 'Instruction_%s_%s_%08x_%08x' % (mnem, mode, mask, value)

    def __ARM_GenerateMethodPrototype(self, insn, in_class = False):
        mnem = self.__ARM_GetMnemonic(insn)
        meth_fmt = 'bool %s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)'
        if in_class == False:
            meth_fmt = 'bool %sArchitecture::%%s(BinaryStream const& rBinStrm, TOffset Offset, u32 Opcode, Instruction& rInsn)' % self.GetArchName()

        return meth_fmt % self.__ARM_GenerateMethodName(insn)

    def GenerateHeader(self):
        res = ''
        res += 'static char const *m_Mnemonic[%#x];\n' % (len(self.all_mnemo) + 1)

        for insn in sorted(self.arch['instruction'], key=lambda a:self.__ARM_GetMnemonic(a)):
            res += self.__ARM_GenerateMethodPrototype(insn, True) + ';\n'

        res += 'bool DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n'
        res += 'bool DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn);\n'

        return res

    def GenerateSource(self):
        res = ''

        res += 'bool ArmArchitecture::Disassemble(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn, u8 Mode)\n'
        res += self._GenerateBrace(
                self._GenerateSwitch('Mode',
                    [('ARM_ModeArm',   'return DisassembleArm(rBinStrm, Offset, rInsn);\n',   False),
                     ('ARM_ModeThumb', 'return DisassembleThumb(rBinStrm, Offset, rInsn);\n', False)],
                    'return false;\n')
                )

        def __ARM_GenerateDispatcher(arm, insns):
            res = ''
            insns_dict = {}

            for insn in insns:
                mask = arm.__ARM_GetMask(insn)
                if not mask in insns_dict:
                    insns_dict[mask] = []
                insns_dict[mask].append(insn)

            for mask, insn_list in insns_dict.items():
                bit = len(insn_list[0]['encoding'])
                if len(insn_list) == 1:
                    value = arm.__ARM_GetValue(insn_list[0])
                    res += arm._GenerateCondition('if', '(Opcode%d & %#010x) == %#010x' % (bit, mask, value), self.__ARM_GenerateInstructionComment(insn) + 'return %s(rBinStrm, Offset, Opcode%d, rInsn);' % (arm.__ARM_GenerateMethodName(insn_list[0]), bit))
                else:
                    cases = []
                    for insn in insn_list:
                        value = arm.__ARM_GetValue(insn)
                        cases.append( ('%#010x' % value, self.__ARM_GenerateInstructionComment(insn) + 'return %s(rBinStrm, Offset, Opcode%d, rInsn);\n' % (arm.__ARM_GenerateMethodName(insn), bit), False) )
                    res += arm._GenerateSwitch('Opcode%d & %#010x' % (bit, mask), cases, 'break;\n')

            return res

        res += 'bool ArmArchitecture::DisassembleArm(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res += self._GenerateBrace(
                self._GenerateRead('Opcode32', 'Offset', 32)+
                __ARM_GenerateDispatcher(self, self.arm_insns)+
                'return false;\n'
                )

        res += 'bool ArmArchitecture::DisassembleThumb(BinaryStream const& rBinStrm, TOffset Offset, Instruction& rInsn)\n'
        res += self._GenerateBrace(
                self._GenerateRead('Opcode16Low', 'Offset & ~1', 16)+
                self._GenerateRead('Opcode16High', '(Offset + 2) & ~1', 16)+
                Indent('u16 Opcode16 = Opcode16Low;\n', 0)+
                Indent('u32 Opcode32 = ((Opcode16Low << 16) | Opcode16High);\n', 0)+
                __ARM_GenerateDispatcher(self, self.thumb_insns)+
                'return false;\n'
                )

        for insn in self.arm_insns + self.thumb_insns:
            res += self.__ARM_GenerateInstructionComment(insn)
            res += self.__ARM_GenerateInstruction(insn)

        return res

    def GenerateOpcodeEnum(self):
        res = ',\n'.join('ARM_Opcode_%s' % x.capitalize() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'enum ARM_Opcode\n' + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOpcodeString(self):
        res = ',\n'.join('"%s"' % x.lower() for x in ['unknown'] + sorted(self.all_mnemo)) + '\n'
        return 'const char *%sArchitecture::m_Mnemonic[%#x] =\n' % (self.GetArchName(), len(self.all_mnemo) + 1) + self._GenerateBrace(res)[:-1] + ';\n'

    def GenerateOperandDefinition(self):
        res = ''
        return res

    def GenerateOperandCode(self):
        res = ''
        return res