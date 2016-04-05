#ifndef ARCH_ARM_DEF_HPP
#define ARCH_ARM_DEF_HPP

enum ARM_Mode
{
  ARM_ModeUnknown,
  ARM_ModeArm,
  ARM_ModeThumb,
};

enum ARM_FeatureThumb
{
  ARM_Thumb1,
  ARM_Thumb2,
  ARM_ThumbEE,
};

// src: https://community.arm.com/groups/processors/blog/2010/07/16/condition-codes-1-condition-flags-and-codes
enum ARM_Condition
{
  ARM_Cond_Eq, // EQual
  ARM_Cond_Ne, // Not Equal
  ARM_Cond_Cs, // Carry Set (unsigned >=)
  ARM_Cond_Hs = ARM_Cond_Cs, // Higher or Same (unsigned >=)
  ARM_Cond_Cc, // Carry Clear (unsigned <)
  ARM_Cond_Lo = ARM_Cond_Cc, // LOwer (unsigned <)
  ARM_Cond_Mi, // MInus (negative)
  ARM_Cond_Pl, // PLus (positive)
  ARM_Cond_Vs, // oVerflow Set
  ARM_Cond_Vc, // oVerflow Clear
  ARM_Cond_Hi, // HIgher (unsigned >)
  ARM_Cond_Ls, // Lower or Same (unsigned <=)
  ARM_Cond_Ge, // Greater or Equal (signed >=)
  ARM_Cond_Lt, // Lesser Than (signed <)
  ARM_Cond_Gt, // Greater Than (signed >)
  ARM_Cond_Le, // Lesser or Equal (signed <=)
  ARM_Cond_Al, // ALways
  ARM_Cond_Unknown,
};

enum ARM_Identifier
{
  ARM_Id_Unknown,

  ARM_Fl_Nf,
  ARM_Fl_Cf,
  ARM_Fl_Vf,
  ARM_Fl_Zf,

  ARM_Reg_R0,
  ARM_Reg_R1,
  ARM_Reg_R2,
  ARM_Reg_R3,
  ARM_Reg_R4,
  ARM_Reg_R5,
  ARM_Reg_R6,
  ARM_Reg_R7,
  ARM_Reg_R8,
  ARM_Reg_R9,
  ARM_Reg_R10,
  ARM_Reg_R11,
  ARM_Reg_Fp = ARM_Reg_R11,
  ARM_Reg_R12,
  ARM_Reg_Ip = ARM_Reg_R12,
  ARM_Reg_R13,
  ARM_Reg_Sp = ARM_Reg_R13,
  ARM_Reg_R14,
  ARM_Reg_Lr = ARM_Reg_R14,
  ARM_Reg_R15,
  ARM_Reg_Pc = ARM_Reg_R15,
};

enum ARM_CSPR
{
  ARM_CSPR_T = 1 << 5,  //! State bit (Thumb)
  ARM_CSPR_V = 1 << 28, //! Overflow
  ARM_CSPR_C = 1 << 29, //! Carry or borrow or extend
  ARM_CSPR_Z = 1 << 30, //! Zero
  ARM_CSPR_N = 1 << 31, //! Negative or less than
};

enum ARM_Prefix
{
  ARM_Prefix_None,
  ARM_Prefix_L = 1 << 0, /* Load/Write (unused) */
  ARM_Prefix_W = 1 << 1, /* Write-back */
  ARM_Prefix_S = 1 << 2, /* Set condition */
  ARM_Prefix_U = 1 << 3, /* Up/Down */
  ARM_Prefix_P = 1 << 4  /* Pre/Post indexing */
};

enum ARM_Attributes
{
  ARM_Attribute_None,
  ARM_Attribute_SupportItBlock = 1 << 0,
  ARM_Attribute_WriteBack = 1 << 1,

};

#endif // !ARM_DEF_HPP