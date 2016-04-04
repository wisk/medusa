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

enum ARM_Register
{
  ARM_RegUnknown,

  ARM_FlNf,
  ARM_FlCf,
  ARM_FlVf,
  ARM_FlZf,

  ARM_RegR0,
  ARM_RegR1,
  ARM_RegR2,
  ARM_RegR3,
  ARM_RegR4,
  ARM_RegR5,
  ARM_RegR6,
  ARM_RegR7,
  ARM_RegR8,
  ARM_RegR9,
  ARM_RegR10,
  ARM_RegR11,
  ARM_RegFP = ARM_RegR11,
  ARM_RegR12,
  ARM_RegIP = ARM_RegR12,
  ARM_RegR13,
  ARM_RegSP = ARM_RegR13,
  ARM_RegR14,
  ARM_RegLR = ARM_RegR14,
  ARM_RegR15,
  ARM_RegPC = ARM_RegR15,
};

enum CSPR_Flags
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