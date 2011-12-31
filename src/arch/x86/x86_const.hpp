#ifndef _X86_CONST_
#define _X86_CONST_

enum X86_Reg
{
  X86_Reg_Unknown,
  X86_Reg_Al,
  X86_Reg_Cl,
  X86_Reg_Dl,
  X86_Reg_Bl,
  X86_Reg_Ah,
  X86_Reg_Ch,
  X86_Reg_Dh,
  X86_Reg_Bh,
  X86_Reg_Spl,
  X86_Reg_Bpl,
  X86_Reg_Sil,
  X86_Reg_Dil,
  X86_Reg_R8b,
  X86_Reg_R9b,
  X86_Reg_R10b,
  X86_Reg_R11b,
  X86_Reg_R12b,
  X86_Reg_R13b,
  X86_Reg_R14b,
  X86_Reg_R15b,
  X86_Reg_Ax,
  X86_Reg_Cx,
  X86_Reg_Dx,
  X86_Reg_Bx,
  X86_Reg_Sp,
  X86_Reg_Bp,
  X86_Reg_Si,
  X86_Reg_Di,
  X86_Reg_R8w,
  X86_Reg_R9w,
  X86_Reg_R10w,
  X86_Reg_R11w,
  X86_Reg_R12w,
  X86_Reg_R13w,
  X86_Reg_R14w,
  X86_Reg_R15w,
  X86_Reg_Es,
  X86_Reg_Cs,
  X86_Reg_Ss,
  X86_Reg_Ds,
  X86_Reg_Fs,
  X86_Reg_Gs,
  X86_Reg_Seg6,
  X86_Reg_Seg7,
  X86_Reg_Eax,
  X86_Reg_Ecx,
  X86_Reg_Edx,
  X86_Reg_Ebx,
  X86_Reg_Esp,
  X86_Reg_Ebp,
  X86_Reg_Esi,
  X86_Reg_Edi,
  X86_Reg_R8d,
  X86_Reg_R9d,
  X86_Reg_R10d,
  X86_Reg_R11d,
  X86_Reg_R12d,
  X86_Reg_R13d,
  X86_Reg_R14d,
  X86_Reg_R15d,
  X86_Reg_Eip,
  X86_Reg_Cr0,
  X86_Reg_Cr1,
  X86_Reg_Cr2,
  X86_Reg_Cr3,
  X86_Reg_Cr4,
  X86_Reg_Cr5,
  X86_Reg_Cr6,
  X86_Reg_Cr7,
  X86_Reg_Cr8,
  X86_Reg_Cr9,
  X86_Reg_Cr10,
  X86_Reg_Cr11,
  X86_Reg_Cr12,
  X86_Reg_Cr13,
  X86_Reg_Cr14,
  X86_Reg_Cr15,
  X86_Reg_Dr0,
  X86_Reg_Dr1,
  X86_Reg_Dr2,
  X86_Reg_Dr3,
  X86_Reg_Dr4,
  X86_Reg_Dr5,
  X86_Reg_Dr6,
  X86_Reg_Dr7,
  X86_Reg_Dr8,
  X86_Reg_Dr9,
  X86_Reg_Dr10,
  X86_Reg_Dr11,
  X86_Reg_Dr12,
  X86_Reg_Dr13,
  X86_Reg_Dr14,
  X86_Reg_Dr15,
  X86_Reg_Tr0,
  X86_Reg_Tr1,
  X86_Reg_Tr2,
  X86_Reg_Tr3,
  X86_Reg_Tr4,
  X86_Reg_Tr5,
  X86_Reg_Tr6,
  X86_Reg_Tr7,
  X86_Reg_Rax,
  X86_Reg_Rcx,
  X86_Reg_Rdx,
  X86_Reg_Rbx,
  X86_Reg_Rsp,
  X86_Reg_Rbp,
  X86_Reg_Rsi,
  X86_Reg_Rdi,
  X86_Reg_R8,
  X86_Reg_R9,
  X86_Reg_R10,
  X86_Reg_R11,
  X86_Reg_R12,
  X86_Reg_R13,
  X86_Reg_R14,
  X86_Reg_R15,
  X86_Reg_Rip,
  X86_Reg_St0,
  X86_Reg_St1,
  X86_Reg_St2,
  X86_Reg_St3,
  X86_Reg_St4,
  X86_Reg_St5,
  X86_Reg_St6,
  X86_Reg_St7,
  X86_Reg_Mm0,
  X86_Reg_Mm1,
  X86_Reg_Mm2,
  X86_Reg_Mm3,
  X86_Reg_Mm4,
  X86_Reg_Mm5,
  X86_Reg_Mm6,
  X86_Reg_Mm7,
  X86_Reg_Xmm0,
  X86_Reg_Xmm1,
  X86_Reg_Xmm2,
  X86_Reg_Xmm3,
  X86_Reg_Xmm4,
  X86_Reg_Xmm5,
  X86_Reg_Xmm6,
  X86_Reg_Xmm7,
  X86_Reg_Xmm8,
  X86_Reg_Xmm9,
  X86_Reg_Xmm10,
  X86_Reg_Xmm11,
  X86_Reg_Xmm12,
  X86_Reg_Xmm13,
  X86_Reg_Xmm14,
  X86_Reg_Xmm15
};

extern const char *X86_RegName[151];

enum X86_Prefix
{
  X86_Prefix_Wait         = 0x1,
  X86_Prefix_GS           = 0x2,
  X86_Prefix_FS           = 0x4,
  X86_Prefix_OpSize       = 0x8,
  X86_Prefix_ES           = 0x10,
  X86_Prefix_AdSize       = 0x20,
  X86_Prefix_CS           = 0x40,
  X86_Prefix_Lock         = 0x80,
  X86_Prefix_RepNz        = 0x100,
  X86_Prefix_Rep          = 0x200,
  X86_Prefix_SS           = 0x400,
  X86_Prefix_DS           = 0x800,
  X86_Prefix_HintTaken    = 0x1000,
  X86_Prefix_HintNotTaken = 0x2000,
  X86_Prefix_HintAltTaken = 0x4000,
  X86_Prefix_REX          = 0x8000,
  X86_Prefix_REX_b        = 0x10000 | X86_Prefix_REX,
  X86_Prefix_REX_x        = 0x20000 | X86_Prefix_REX,
  X86_Prefix_REX_r        = 0x40000 | X86_Prefix_REX,
  X86_Prefix_REX_w        = 0x80000 | X86_Prefix_REX,
  X86_Prefix_REX_xb       = X86_Prefix_REX_x | X86_Prefix_REX_b,
  X86_Prefix_REX_rb       = X86_Prefix_REX_r | X86_Prefix_REX_b,
  X86_Prefix_REX_rx       = X86_Prefix_REX_r | X86_Prefix_REX_x,
  X86_Prefix_REX_rxb      = X86_Prefix_REX_r | X86_Prefix_REX_x | X86_Prefix_REX_b,
  X86_Prefix_REX_wb       = X86_Prefix_REX_w | X86_Prefix_REX_b,
  X86_Prefix_REX_wx       = X86_Prefix_REX_w | X86_Prefix_REX_x,
  X86_Prefix_REX_wxb      = X86_Prefix_REX_w | X86_Prefix_REX_x | X86_Prefix_REX_b,
  X86_Prefix_REX_wr       = X86_Prefix_REX_w | X86_Prefix_REX_r,
  X86_Prefix_REX_wrb      = X86_Prefix_REX_w | X86_Prefix_REX_r | X86_Prefix_REX_b,
  X86_Prefix_REX_wrx      = X86_Prefix_REX_w | X86_Prefix_REX_r | X86_Prefix_REX_x,
  X86_Prefix_REX_wrxb     = X86_Prefix_REX_w | X86_Prefix_REX_r | X86_Prefix_REX_x | X86_Prefix_REX_b,
  X86_Prefix_Xop          = 0x100000,
  X86_Prefix_VEX2         = 0x200000,
  X86_Prefix_VEX3         = 0x400000
};

enum X86_Feature_Integer
{
  // Integer instructions
  X86_FeatInt_8086,
  X86_FeatInt_80186,
  X86_FeatInt_80286,
  X86_FeatInt_80386,
  X86_FeatInt_80486,
  X86_FeatInt_80586,
  X86_FeatInt_80686,
  X86_FeatInt_80786,
  X86_FeatInt_Sse,
  X86_FeatInt_Sse2,
  X86_FeatInt_Sse3,
  X86_FeatInt_Svm,
  X86_FeatInt_Vmx,
  X86_FeatInt_Sse4a
};

enum X86_Feature_FloatPoint
{
  // FP instructions
  X86_FeatFp_8087,
  X86_FeatFp_80287,
  X86_FeatFp_80387,
  X86_FeatFp_80487,
  X86_FeatFp_80587
};

enum X86_Feature_Simd
{
  // SIMD instructions
  X86_FeatSimd_Mmx,
  X86_FeatSimd_MmxPlus,
  X86_FeatSimd_Emmx,
  X86_FeatSimd_3dNow,
  X86_FeatSimd_Sse,
  X86_FeatSimd_Sse2,
  X86_FeatSimd_Sse3,
  X86_FeatSimd_Ssse3,
  X86_FeatSimd_Sse41,
  X86_FeatSimd_Sse4a,
  X86_FeatSimd_Sse42,
  X86_FeatSimd_Aes,
  X86_FeatSimd_Avx,
};

enum X86_Arch
{
  X86_Arch_8087,
  X86_Arch_8088,

  X86_Arch_80186,

  X86_Arch_80286,
  X86_Arch_80287,
  X86_Arch_80287xl,

  X86_Arch_80386,
  X86_Arch_80386sl,
  X86_Arch_80387,
  X86_Arch_80387sl,

  X86_Arch_80486,

  X86_Arch_Centaur_ace,
  X86_Arch_Centaur_ais,
  X86_Arch_Centaur_he,
  X86_Arch_Centaur_mm,
  X86_Arch_Centaur_rng,
  X86_Arch_Cyrix_gx1,
  X86_Arch_Geode_lx,
  X86_Arch_K6_2,
  X86_Arch_Mmx,
  X86_Arch_Mmx_sse,
  X86_Arch_Mmx_sse2,
  X86_Arch_Pentium_4,
  X86_Arch_Pentium_pro,
  X86_Arch_Sse,
  X86_Arch_Sse2,
  X86_Arch_Sse2_mmx,
  X86_Arch_Sse3,
  X86_Arch_Ssse3,
  X86_Arch_Sse4a,
  X86_Arch_Sse41,
  X86_Arch_Sse42,

  X86_Arch_Vmx,
  X86_Arch_Invpcid, // ?
  X86_Arch_Fma,

  X86_Arch_Aes,

  X86_Arch_Movbe, // ?
  X86_Arch_Bmi,
  X86_Arch_Bmi2,
  X86_Arch_Clmul, // ?
  X86_Arch_Avx,
  X86_Arch_Avx2,
  X86_Arch_F16c
};

enum X86_Mode
{
  X86_Mode_Real,
  X86_Mode_Protected,
  X86_Mode_Long
};

enum X86_ProcessorType
{
  X86_ProcType_AMD,
  X86_ProcType_INTEL,
  X86_ProcType_IA64,
  X86_ProcType_CYRIX,
  X86_ProcType_IIT
};

enum X86_Bit
{
  X86_Bit_16 = 16,
  X86_Bit_32 = 32,
  X86_Bit_64 = 64
};

enum X86_Condition
{
  X86_Cond_None,
  X86_Cond_B,
  X86_Cond_Be,
  X86_Cond_L,
  X86_Cond_Le,
  X86_Cond_Nb,
  X86_Cond_Nbe,
  X86_Cond_Nl,
  X86_Cond_Nle,
  X86_Cond_No,
  X86_Cond_Np,
  X86_Cond_Ns,
  X86_Cond_Nz,
  X86_Cond_O,
  X86_Cond_P,
  X86_Cond_S,
  X86_Cond_Z
};

#endif // !_X86_CONST_
