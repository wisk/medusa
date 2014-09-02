#ifndef ARCH_GAMEBOY_REGISTER_HPP
#define ARCH_GAMEBOY_REGISTER_HPP

#define GB_Invalid_Reg  0x0000
#define GB_RegA         0x0001
#define GB_RegB         0x0002
#define GB_RegC         0x0004
#define GB_RegD         0x0008
#define GB_RegE         0x0010
#define GB_RegF         0x0020
#define GB_RegH         0x0040
#define GB_RegL         0x0080
#define GB_RegSp        0x0100
#define GB_RegPc        0x0200
#define GB_RegFl        0x0400

#define GB_FlCf         0x1000
#define GB_FlHf         0x2000 /* Half Carry Flag (BCD) */
#define GB_FlNf         0x4000 /* Add-Sub Flag (BCD) */
#define GB_FlZf         0x8000

#define GB_RegAF        (GB_RegA | GB_RegF)
#define GB_RegBC        (GB_RegB | GB_RegC)
#define GB_RegDE        (GB_RegD | GB_RegE)
#define GB_RegHL        (GB_RegH | GB_RegL)

#endif // ARCH_GAMEBOY_REGISTER_HPP