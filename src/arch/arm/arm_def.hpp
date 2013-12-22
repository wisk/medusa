#ifndef __ARM_DEF_HPP__
#define __ARM_DEF_HPP__

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

enum ARM_Condition
{
  ARM_Cond_Eq,
  ARM_Cond_Ne,
  ARM_Cond_Cs,
  ARM_Cond_Cc,
  ARM_Cond_Mi,
  ARM_Cond_Pl,
  ARM_Cond_Vs,
  ARM_Cond_Vc,
  ARM_Cond_Hi,
  ARM_Cond_Ls,
  ARM_Cond_Ge,
  ARM_Cond_Lt,
  ARM_Cond_Gt,
  ARM_Cond_Le
};

enum ARM_Register
{
  ARM_RegR0  = 1 << 0,
  ARM_RegR1  = 1 << 1,
  ARM_RegR2  = 1 << 2,
  ARM_RegR3  = 1 << 3,
  ARM_RegR4  = 1 << 4,
  ARM_RegR5  = 1 << 5,
  ARM_RegR6  = 1 << 6,
  ARM_RegR7  = 1 << 7,
  ARM_RegR8  = 1 << 8,
  ARM_RegR9  = 1 << 9,
  ARM_RegR10 = 1 << 10,
  ARM_RegR11 = 1 << 11,
  ARM_RegR12 = 1 << 12,
  ARM_RegR13 = 1 << 13,
  ARM_RegR14 = 1 << 14,
  ARM_RegSP  = 1 << 14,
  ARM_RegR15 = 1 << 15,
  ARM_RegPC  = 1 << 15
};

enum ARM_Flag
{
  ARM_FlNone,
  ARM_FlNf = 1 << 0,
  ARM_FlCf = 1 << 1,
  ARM_FlVf = 1 << 2,
  ARM_FlZf = 1 << 3
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

#endif // !__ARM_DEF_HPP__