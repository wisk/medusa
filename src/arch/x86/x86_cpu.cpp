#include "x86_architecture.hpp"

#include <boost/format.hpp>

bool X86Architecture::X86CpuContext::ReadRegister(u32 Register, void* pValue, u32 Size) const
{
  auto RegisterSize = m_rCpuInfo.GetSizeOfRegisterInBit(Register) / 8;
  if (RegisterSize != Size) return false;

#define READ_R_L(reg) *reinterpret_cast<u8  *>(pValue) = m_Context.reg.x.l;
#define READ_R_H(reg) *reinterpret_cast<u8  *>(pValue) = m_Context.reg.x.h;
#define READ_R_W(reg) *reinterpret_cast<u16 *>(pValue) = m_Context.reg.w;
#define READ_S(seg)   *reinterpret_cast<u16 *>(pValue) = m_Context.seg;
#define READ_R_E(reg) *reinterpret_cast<u32 *>(pValue) = m_Context.reg.e;
#define READ_R_R(reg) *reinterpret_cast<u64 *>(pValue) = m_Context.reg.r;
#define READ_F(flg)   *reinterpret_cast<u8  *>(pValue) = (m_Context.flg);

  switch (Register)
  {
  case X86_FlCf:    READ_F(CF); break;
  case X86_FlPf:    READ_F(PF); break;
  case X86_FlAf:    READ_F(AF); break;
  case X86_FlZf:    READ_F(ZF); break;
  case X86_FlSf:    READ_F(SF); break;
  case X86_FlTf:    READ_F(TF); break;
  case X86_FlIf:    READ_F(IF); break;
  case X86_FlDf:    READ_F(DF); break;
  case X86_FlOf:    READ_F(OF); break;

  case X86_Reg_Al:  READ_R_L(a);       break;
  case X86_Reg_Ah:  READ_R_H(a);       break;
  case X86_Reg_Bl:  READ_R_L(b);       break;
  case X86_Reg_Bh:  READ_R_H(b);       break;
  case X86_Reg_Cl:  READ_R_L(c);       break;
  case X86_Reg_Ch:  READ_R_H(c);       break;
  case X86_Reg_Dl:  READ_R_L(d);       break;
  case X86_Reg_Dh:  READ_R_H(d);       break;
  case X86_Reg_Spl: READ_R_L(sp);      break;
  case X86_Reg_Bpl: READ_R_L(bp);      break;
  case X86_Reg_Sil: READ_R_L(si);      break;
  case X86_Reg_Dil: READ_R_L(sp);      break;
  case X86_Reg_R8b: READ_R_L(r8);      break;
  case X86_Reg_R9b: READ_R_L(r9);      break;
  case X86_Reg_R10b:READ_R_L(r10);     break;
  case X86_Reg_R11b:READ_R_L(r11);     break;
  case X86_Reg_R12b:READ_R_L(r12);     break;
  case X86_Reg_R13b:READ_R_L(r13);     break;
  case X86_Reg_R14b:READ_R_L(r14);     break;
  case X86_Reg_R15b:READ_R_L(r15);     break;

  case X86_Reg_Ax:  READ_R_W(a);       break;
  case X86_Reg_Bx:  READ_R_W(b);       break;
  case X86_Reg_Cx:  READ_R_W(c);       break;
  case X86_Reg_Dx:  READ_R_W(d);       break;
  case X86_Reg_Sp:  READ_R_W(sp);      break;
  case X86_Reg_Bp:  READ_R_W(bp);      break;
  case X86_Reg_Si:  READ_R_W(si);      break;
  case X86_Reg_Di:  READ_R_W(di);      break;
  case X86_Reg_R8w: READ_R_W(r8);      break;
  case X86_Reg_R9w: READ_R_W(r9);      break;
  case X86_Reg_R10w:READ_R_W(r10);     break;
  case X86_Reg_R11w:READ_R_W(r11);     break;
  case X86_Reg_R12w:READ_R_W(r12);     break;
  case X86_Reg_R13w:READ_R_W(r13);     break;
  case X86_Reg_R14w:READ_R_W(r14);     break;
  case X86_Reg_R15w:READ_R_W(r15);     break;
  case X86_Reg_Ip:  READ_R_W(ip);      break;
  case X86_Reg_Cs:  READ_S(cs);        break;
  case X86_Reg_Ds:  READ_S(ds);        break;
  case X86_Reg_Es:  READ_S(es);        break;
  case X86_Reg_Ss:  READ_S(ss);        break;
  case X86_Reg_Fs:  READ_S(fs);        break;
  case X86_Reg_Gs:  READ_S(gs);        break;

  case X86_Reg_Eax: READ_R_E(a);       break;
  case X86_Reg_Ebx: READ_R_E(b);       break;
  case X86_Reg_Ecx: READ_R_E(c);       break;
  case X86_Reg_Edx: READ_R_E(d);       break;
  case X86_Reg_Esp: READ_R_E(sp);      break;
  case X86_Reg_Ebp: READ_R_E(bp);      break;
  case X86_Reg_Esi: READ_R_E(si);      break;
  case X86_Reg_Edi: READ_R_E(di);      break;
  case X86_Reg_R8d: READ_R_E(r8);      break;
  case X86_Reg_R9d: READ_R_E(r9);      break;
  case X86_Reg_R10d:READ_R_E(r10);     break;
  case X86_Reg_R11d:READ_R_E(r11);     break;
  case X86_Reg_R12d:READ_R_E(r12);     break;
  case X86_Reg_R13d:READ_R_E(r13);     break;
  case X86_Reg_R14d:READ_R_E(r14);     break;
  case X86_Reg_R15d:READ_R_E(r15);     break;
  case X86_Reg_Eip: READ_R_E(ip);      break;

  case X86_Reg_Rax: READ_R_R(a);       break;
  case X86_Reg_Rbx: READ_R_R(b);       break;
  case X86_Reg_Rcx: READ_R_R(c);       break;
  case X86_Reg_Rdx: READ_R_R(d);       break;
  case X86_Reg_Rsp: READ_R_R(sp);      break;
  case X86_Reg_Rbp: READ_R_R(bp);      break;
  case X86_Reg_Rsi: READ_R_R(si);      break;
  case X86_Reg_Rdi: READ_R_R(di);      break;
  case X86_Reg_R8:  READ_R_R(r8);      break;
  case X86_Reg_R9:  READ_R_R(r9);      break;
  case X86_Reg_R10: READ_R_R(r10);     break;
  case X86_Reg_R11: READ_R_R(r11);     break;
  case X86_Reg_R12: READ_R_R(r12);     break;
  case X86_Reg_R13: READ_R_R(r13);     break;
  case X86_Reg_R14: READ_R_R(r14);     break;
  case X86_Reg_R15: READ_R_R(r15);     break;
  case X86_Reg_Rip: READ_R_R(ip);      break;
  }

#undef READ_R_L
#undef READ_R_H
#undef READ_R_W
#undef READ_S
#undef READ_R_E
#undef READ_R_R
#undef READ_F

  return true;
}

bool X86Architecture::X86CpuContext::WriteRegister(u32 Register, void const* pValue, u32 Size, bool SignExtend)
{
  auto RegisterSize = m_rCpuInfo.GetSizeOfRegisterInBit(Register) / 8;
  if (RegisterSize != Size) return false;

#define WRITE_R_L(reg) m_Context.reg.x.l = *reinterpret_cast<u8  const*>(pValue);
#define WRITE_R_H(reg) m_Context.reg.x.h = *reinterpret_cast<u8  const*>(pValue);
#define WRITE_R_W(reg) m_Context.reg.w   = *reinterpret_cast<u16 const*>(pValue);
#define WRITE_S(seg)   m_Context.seg     = *reinterpret_cast<u16 const*>(pValue);
#define WRITE_R_E(reg) m_Context.reg.r   = *reinterpret_cast<u32 const*>(pValue); /* AMD64 clears 32MSB of register */
#define WRITE_R_R(reg) m_Context.reg.r   = *reinterpret_cast<u64 const*>(pValue);
#define WRITE_F(flg)   m_Context.flg     = *reinterpret_cast<u8  const*>(pValue) ? true : false;

  switch (Register)
  {
  case X86_FlCf:    WRITE_F(CF); break;
  case X86_FlPf:    WRITE_F(PF); break;
  case X86_FlAf:    WRITE_F(AF); break;
  case X86_FlZf:    WRITE_F(ZF); break;
  case X86_FlSf:    WRITE_F(SF); break;
  case X86_FlTf:    WRITE_F(TF); break;
  case X86_FlIf:    WRITE_F(IF); break;
  case X86_FlDf:    WRITE_F(DF); break;
  case X86_FlOf:    WRITE_F(OF); break;

  case X86_Reg_Al:  WRITE_R_L(a);       break;
  case X86_Reg_Ah:  WRITE_R_H(a);       break;
  case X86_Reg_Bl:  WRITE_R_L(b);       break;
  case X86_Reg_Bh:  WRITE_R_H(b);       break;
  case X86_Reg_Cl:  WRITE_R_L(c);       break;
  case X86_Reg_Ch:  WRITE_R_H(c);       break;
  case X86_Reg_Dl:  WRITE_R_L(d);       break;
  case X86_Reg_Dh:  WRITE_R_H(d);       break;
  case X86_Reg_Spl: WRITE_R_L(sp);      break;
  case X86_Reg_Bpl: WRITE_R_L(bp);      break;
  case X86_Reg_Sil: WRITE_R_L(si);      break;
  case X86_Reg_Dil: WRITE_R_L(sp);      break;
  case X86_Reg_R8b: WRITE_R_L(r8);      break;
  case X86_Reg_R9b: WRITE_R_L(r9);      break;
  case X86_Reg_R10b:WRITE_R_L(r10);     break;
  case X86_Reg_R11b:WRITE_R_L(r11);     break;
  case X86_Reg_R12b:WRITE_R_L(r12);     break;
  case X86_Reg_R13b:WRITE_R_L(r13);     break;
  case X86_Reg_R14b:WRITE_R_L(r14);     break;
  case X86_Reg_R15b:WRITE_R_L(r15);     break;

  case X86_Reg_Ax:  WRITE_R_W(a);       break;
  case X86_Reg_Bx:  WRITE_R_W(b);       break;
  case X86_Reg_Cx:  WRITE_R_W(c);       break;
  case X86_Reg_Dx:  WRITE_R_W(d);       break;
  case X86_Reg_Sp:  WRITE_R_W(sp);      break;
  case X86_Reg_Bp:  WRITE_R_W(bp);      break;
  case X86_Reg_Si:  WRITE_R_W(si);      break;
  case X86_Reg_Di:  WRITE_R_W(di);      break;
  case X86_Reg_R8w: WRITE_R_W(r8);      break;
  case X86_Reg_R9w: WRITE_R_W(r9);      break;
  case X86_Reg_R10w:WRITE_R_W(r10);     break;
  case X86_Reg_R11w:WRITE_R_W(r11);     break;
  case X86_Reg_R12w:WRITE_R_W(r12);     break;
  case X86_Reg_R13w:WRITE_R_W(r13);     break;
  case X86_Reg_R14w:WRITE_R_W(r14);     break;
  case X86_Reg_R15w:WRITE_R_W(r15);     break;
  case X86_Reg_Ip:  WRITE_R_W(ip);      break;
  case X86_Reg_Cs:  WRITE_S(cs);        break;
  case X86_Reg_Ds:  WRITE_S(ds);        break;
  case X86_Reg_Es:  WRITE_S(es);        break;
  case X86_Reg_Ss:  WRITE_S(ss);        break;
  case X86_Reg_Fs:  WRITE_S(fs);        break;
  case X86_Reg_Gs:  WRITE_S(gs);        break;

  case X86_Reg_Eax: WRITE_R_E(a);       break;
  case X86_Reg_Ebx: WRITE_R_E(b);       break;
  case X86_Reg_Ecx: WRITE_R_E(c);       break;
  case X86_Reg_Edx: WRITE_R_E(d);       break;
  case X86_Reg_Esp: WRITE_R_E(sp);      break;
  case X86_Reg_Ebp: WRITE_R_E(bp);      break;
  case X86_Reg_Esi: WRITE_R_E(si);      break;
  case X86_Reg_Edi: WRITE_R_E(di);      break;
  case X86_Reg_R8d: WRITE_R_E(r8);      break;
  case X86_Reg_R9d: WRITE_R_E(r9);      break;
  case X86_Reg_R10d:WRITE_R_E(r10);     break;
  case X86_Reg_R11d:WRITE_R_E(r11);     break;
  case X86_Reg_R12d:WRITE_R_E(r12);     break;
  case X86_Reg_R13d:WRITE_R_E(r13);     break;
  case X86_Reg_R14d:WRITE_R_E(r14);     break;
  case X86_Reg_R15d:WRITE_R_E(r15);     break;
  case X86_Reg_Eip: WRITE_R_E(ip);      break;

  case X86_Reg_Rax: WRITE_R_R(a);       break;
  case X86_Reg_Rbx: WRITE_R_R(b);       break;
  case X86_Reg_Rcx: WRITE_R_R(c);       break;
  case X86_Reg_Rdx: WRITE_R_R(d);       break;
  case X86_Reg_Rsp: WRITE_R_R(sp);      break;
  case X86_Reg_Rbp: WRITE_R_R(bp);      break;
  case X86_Reg_Rsi: WRITE_R_R(si);      break;
  case X86_Reg_Rdi: WRITE_R_R(di);      break;
  case X86_Reg_R8:  WRITE_R_R(r8);      break;
  case X86_Reg_R9:  WRITE_R_R(r9);      break;
  case X86_Reg_R10: WRITE_R_R(r10);     break;
  case X86_Reg_R11: WRITE_R_R(r11);     break;
  case X86_Reg_R12: WRITE_R_R(r12);     break;
  case X86_Reg_R13: WRITE_R_R(r13);     break;
  case X86_Reg_R14: WRITE_R_R(r14);     break;
  case X86_Reg_R15: WRITE_R_R(r15);     break;
  case X86_Reg_Rip: WRITE_R_R(ip);      break;
  }

#undef WRITE_R_L
#undef WRITE_R_H
#undef WRITE_R_W
#undef WRITE_S
#undef WRITE_R_E
#undef WRITE_R_R
#undef WRITE_F

  return true;
}

void* X86Architecture::X86CpuContext::GetRegisterAddress(u32 Register)
{
#define ADDR_R_L(reg) &m_Context.reg
#define ADDR_R_H(reg) (&m_Context.reg) + 1
#define ADDR_R_W(reg) &m_Context.reg
#define ADDR_S(seg)   &m_Context.seg
#define ADDR_R_E(reg) &m_Context.reg
#define ADDR_R_R(reg) &m_Context.reg
#define ADDR_F(flg)   &m_Context.flg

  switch (Register)
  {
  case X86_FlCf:     return ADDR_F(CF);
  case X86_FlPf:     return ADDR_F(PF);
  case X86_FlAf:     return ADDR_F(AF);
  case X86_FlZf:     return ADDR_F(ZF);
  case X86_FlSf:     return ADDR_F(SF);
  case X86_FlTf:     return ADDR_F(TF);
  case X86_FlIf:     return ADDR_F(IF);
  case X86_FlDf:     return ADDR_F(DF);
  case X86_FlOf:     return ADDR_F(OF);

  case X86_Reg_Al:   return ADDR_R_L(a);
  case X86_Reg_Ah:   return ADDR_R_H(a);
  case X86_Reg_Bl:   return ADDR_R_L(b);
  case X86_Reg_Bh:   return ADDR_R_H(b);
  case X86_Reg_Cl:   return ADDR_R_L(c);
  case X86_Reg_Ch:   return ADDR_R_H(c);
  case X86_Reg_Dl:   return ADDR_R_L(d);
  case X86_Reg_Dh:   return ADDR_R_H(d);
  case X86_Reg_Spl:  return ADDR_R_L(sp);
  case X86_Reg_Bpl:  return ADDR_R_L(bp);
  case X86_Reg_Sil:  return ADDR_R_L(si);
  case X86_Reg_Dil:  return ADDR_R_L(sp);
  case X86_Reg_R8b:  return ADDR_R_L(r8);
  case X86_Reg_R9b:  return ADDR_R_L(r9);
  case X86_Reg_R10b: return ADDR_R_L(r10);
  case X86_Reg_R11b: return ADDR_R_L(r11);
  case X86_Reg_R12b: return ADDR_R_L(r12);
  case X86_Reg_R13b: return ADDR_R_L(r13);
  case X86_Reg_R14b: return ADDR_R_L(r14);
  case X86_Reg_R15b: return ADDR_R_L(r15);
  case X86_Reg_Ax:   return ADDR_R_W(a);
  case X86_Reg_Bx:   return ADDR_R_W(b);
  case X86_Reg_Cx:   return ADDR_R_W(c);
  case X86_Reg_Dx:   return ADDR_R_W(d);
  case X86_Reg_Sp:   return ADDR_R_W(sp);
  case X86_Reg_Bp:   return ADDR_R_W(bp);
  case X86_Reg_Si:   return ADDR_R_W(si);
  case X86_Reg_Di:   return ADDR_R_W(di);
  case X86_Reg_R8w:  return ADDR_R_W(r8);
  case X86_Reg_R9w:  return ADDR_R_W(r9);
  case X86_Reg_R10w: return ADDR_R_W(r10);
  case X86_Reg_R11w: return ADDR_R_W(r11);
  case X86_Reg_R12w: return ADDR_R_W(r12);
  case X86_Reg_R13w: return ADDR_R_W(r13);
  case X86_Reg_R14w: return ADDR_R_W(r14);
  case X86_Reg_R15w: return ADDR_R_W(r15);
  case X86_Reg_Ip:   return ADDR_R_W(ip);
  case X86_Reg_Cs:   return ADDR_S(cs);
  case X86_Reg_Ds:   return ADDR_S(ds);
  case X86_Reg_Es:   return ADDR_S(es);
  case X86_Reg_Ss:   return ADDR_S(ss);
  case X86_Reg_Fs:   return ADDR_S(fs);
  case X86_Reg_Gs:   return ADDR_S(gs);
  case X86_Reg_Eax:  return ADDR_R_E(a);
  case X86_Reg_Ebx:  return ADDR_R_E(b);
  case X86_Reg_Ecx:  return ADDR_R_E(c);
  case X86_Reg_Edx:  return ADDR_R_E(d);
  case X86_Reg_Esp:  return ADDR_R_E(sp);
  case X86_Reg_Ebp:  return ADDR_R_E(bp);
  case X86_Reg_Esi:  return ADDR_R_E(si);
  case X86_Reg_Edi:  return ADDR_R_E(di);
  case X86_Reg_R8d:  return ADDR_R_E(r8);
  case X86_Reg_R9d:  return ADDR_R_E(r9);
  case X86_Reg_R10d: return ADDR_R_E(r10);
  case X86_Reg_R11d: return ADDR_R_E(r11);
  case X86_Reg_R12d: return ADDR_R_E(r12);
  case X86_Reg_R13d: return ADDR_R_E(r13);
  case X86_Reg_R14d: return ADDR_R_E(r14);
  case X86_Reg_R15d: return ADDR_R_E(r15);
  case X86_Reg_Eip:  return ADDR_R_E(ip);
  case X86_Reg_Rax:  return ADDR_R_R(a);
  case X86_Reg_Rbx:  return ADDR_R_R(b);
  case X86_Reg_Rcx:  return ADDR_R_R(c);
  case X86_Reg_Rdx:  return ADDR_R_R(d);
  case X86_Reg_Rsp:  return ADDR_R_R(sp);
  case X86_Reg_Rbp:  return ADDR_R_R(bp);
  case X86_Reg_Rsi:  return ADDR_R_R(si);
  case X86_Reg_Rdi:  return ADDR_R_R(di);
  case X86_Reg_R8:   return ADDR_R_R(r8);
  case X86_Reg_R9:   return ADDR_R_R(r9);
  case X86_Reg_R10:  return ADDR_R_R(r10);
  case X86_Reg_R11:  return ADDR_R_R(r11);
  case X86_Reg_R12:  return ADDR_R_R(r12);
  case X86_Reg_R13:  return ADDR_R_R(r13);
  case X86_Reg_R14:  return ADDR_R_R(r14);
  case X86_Reg_R15:  return ADDR_R_R(r15);
  case X86_Reg_Rip:  return ADDR_R_R(ip);
  default:           break;
  }

#undef ADDR_R_L
#undef ADDR_R_H
#undef ADDR_R_W
#undef ADDR_S
#undef ADDR_R_E
#undef ADDR_R_R

  return nullptr;
}

u16 X86Architecture::X86CpuContext::GetRegisterOffset(u32 Register)
{
#define OFF_R_L(reg) offsetof(Context, reg)
#define OFF_R_H(reg) (offsetof(Context, reg) + 1)
#define OFF_R_W(reg) offsetof(Context, reg)
#define OFF_S(seg)   offsetof(Context, seg)
#define OFF_R_E(reg) offsetof(Context, reg)
#define OFF_R_R(reg) offsetof(Context, reg)
#define OFF_F(flg)   offsetof(Context, flg)

  switch (Register)
  {
  case X86_FlCf:     return OFF_F(CF);
  case X86_FlPf:     return OFF_F(PF);
  case X86_FlAf:     return OFF_F(AF);
  case X86_FlZf:     return OFF_F(ZF);
  case X86_FlSf:     return OFF_F(SF);
  case X86_FlTf:     return OFF_F(TF);
  case X86_FlIf:     return OFF_F(IF);
  case X86_FlDf:     return OFF_F(DF);
  case X86_FlOf:     return OFF_F(OF);

  case X86_Reg_Al:   return OFF_R_L(a);
  case X86_Reg_Ah:   return OFF_R_H(a);
  case X86_Reg_Bl:   return OFF_R_L(b);
  case X86_Reg_Bh:   return OFF_R_H(b);
  case X86_Reg_Cl:   return OFF_R_L(c);
  case X86_Reg_Ch:   return OFF_R_H(c);
  case X86_Reg_Dl:   return OFF_R_L(d);
  case X86_Reg_Dh:   return OFF_R_H(d);
  case X86_Reg_Spl:  return OFF_R_L(sp);
  case X86_Reg_Bpl:  return OFF_R_L(bp);
  case X86_Reg_Sil:  return OFF_R_L(si);
  case X86_Reg_Dil:  return OFF_R_L(sp);
  case X86_Reg_R8b:  return OFF_R_L(r8);
  case X86_Reg_R9b:  return OFF_R_L(r9);
  case X86_Reg_R10b: return OFF_R_L(r10);
  case X86_Reg_R11b: return OFF_R_L(r11);
  case X86_Reg_R12b: return OFF_R_L(r12);
  case X86_Reg_R13b: return OFF_R_L(r13);
  case X86_Reg_R14b: return OFF_R_L(r14);
  case X86_Reg_R15b: return OFF_R_L(r15);
  case X86_Reg_Ax:   return OFF_R_W(a);
  case X86_Reg_Bx:   return OFF_R_W(b);
  case X86_Reg_Cx:   return OFF_R_W(c);
  case X86_Reg_Dx:   return OFF_R_W(d);
  case X86_Reg_Sp:   return OFF_R_W(sp);
  case X86_Reg_Bp:   return OFF_R_W(bp);
  case X86_Reg_Si:   return OFF_R_W(si);
  case X86_Reg_Di:   return OFF_R_W(di);
  case X86_Reg_R8w:  return OFF_R_W(r8);
  case X86_Reg_R9w:  return OFF_R_W(r9);
  case X86_Reg_R10w: return OFF_R_W(r10);
  case X86_Reg_R11w: return OFF_R_W(r11);
  case X86_Reg_R12w: return OFF_R_W(r12);
  case X86_Reg_R13w: return OFF_R_W(r13);
  case X86_Reg_R14w: return OFF_R_W(r14);
  case X86_Reg_R15w: return OFF_R_W(r15);
  case X86_Reg_Ip:   return OFF_R_W(ip);
  case X86_Reg_Cs:   return OFF_S(cs);
  case X86_Reg_Ds:   return OFF_S(ds);
  case X86_Reg_Es:   return OFF_S(es);
  case X86_Reg_Ss:   return OFF_S(ss);
  case X86_Reg_Fs:   return OFF_S(fs);
  case X86_Reg_Gs:   return OFF_S(gs);
  case X86_Reg_Eax:  return OFF_R_E(a);
  case X86_Reg_Ebx:  return OFF_R_E(b);
  case X86_Reg_Ecx:  return OFF_R_E(c);
  case X86_Reg_Edx:  return OFF_R_E(d);
  case X86_Reg_Esp:  return OFF_R_E(sp);
  case X86_Reg_Ebp:  return OFF_R_E(bp);
  case X86_Reg_Esi:  return OFF_R_E(si);
  case X86_Reg_Edi:  return OFF_R_E(di);
  case X86_Reg_R8d:  return OFF_R_E(r8);
  case X86_Reg_R9d:  return OFF_R_E(r9);
  case X86_Reg_R10d: return OFF_R_E(r10);
  case X86_Reg_R11d: return OFF_R_E(r11);
  case X86_Reg_R12d: return OFF_R_E(r12);
  case X86_Reg_R13d: return OFF_R_E(r13);
  case X86_Reg_R14d: return OFF_R_E(r14);
  case X86_Reg_R15d: return OFF_R_E(r15);
  case X86_Reg_Eip:  return OFF_R_E(ip);
  case X86_Reg_Rax:  return OFF_R_R(a);
  case X86_Reg_Rbx:  return OFF_R_R(b);
  case X86_Reg_Rcx:  return OFF_R_R(c);
  case X86_Reg_Rdx:  return OFF_R_R(d);
  case X86_Reg_Rsp:  return OFF_R_R(sp);
  case X86_Reg_Rbp:  return OFF_R_R(bp);
  case X86_Reg_Rsi:  return OFF_R_R(si);
  case X86_Reg_Rdi:  return OFF_R_R(di);
  case X86_Reg_R8:   return OFF_R_R(r8);
  case X86_Reg_R9:   return OFF_R_R(r9);
  case X86_Reg_R10:  return OFF_R_R(r10);
  case X86_Reg_R11:  return OFF_R_R(r11);
  case X86_Reg_R12:  return OFF_R_R(r12);
  case X86_Reg_R13:  return OFF_R_R(r13);
  case X86_Reg_R14:  return OFF_R_R(r14);
  case X86_Reg_R15:  return OFF_R_R(r15);
  case X86_Reg_Rip:  return OFF_R_R(ip);
  default:           break;
  }

#undef OFF_R_L
#undef OFF_R_H
#undef OFF_R_W
#undef OFF_S
#undef OFF_R_E
#undef OFF_R_R

  return -1;
}

void X86Architecture::X86CpuContext::GetRegisters(RegisterList& RegList) const
{
  RegList.push_back(X86_FlCf);
  RegList.push_back(X86_FlPf);
  RegList.push_back(X86_FlAf);
  RegList.push_back(X86_FlZf);
  RegList.push_back(X86_FlSf);
  RegList.push_back(X86_FlTf);
  RegList.push_back(X86_FlIf);
  RegList.push_back(X86_FlDf);
  RegList.push_back(X86_FlOf);

  switch (m_Bits)
  {
  case X86_Bit_16:
    RegList.push_back(X86_Reg_Ax);
    RegList.push_back(X86_Reg_Bx);
    RegList.push_back(X86_Reg_Cx);
    RegList.push_back(X86_Reg_Dx);
    RegList.push_back(X86_Reg_Si);
    RegList.push_back(X86_Reg_Di);
    RegList.push_back(X86_Reg_Sp);
    RegList.push_back(X86_Reg_Bp);
    RegList.push_back(X86_Reg_Ip);
    break;

  case X86_Bit_32:
    RegList.push_back(X86_Reg_Eax);
    RegList.push_back(X86_Reg_Ebx);
    RegList.push_back(X86_Reg_Ecx);
    RegList.push_back(X86_Reg_Edx);
    RegList.push_back(X86_Reg_Esi);
    RegList.push_back(X86_Reg_Edi);
    RegList.push_back(X86_Reg_Esp);
    RegList.push_back(X86_Reg_Ebp);
    RegList.push_back(X86_Reg_Eip);
    break;

  case X86_Bit_64:
    RegList.push_back(X86_Reg_Rax);
    RegList.push_back(X86_Reg_Rbx);
    RegList.push_back(X86_Reg_Rcx);
    RegList.push_back(X86_Reg_Rdx);
    RegList.push_back(X86_Reg_Rsi);
    RegList.push_back(X86_Reg_Rdi);
    RegList.push_back(X86_Reg_Rsp);
    RegList.push_back(X86_Reg_Rbp);
    RegList.push_back(X86_Reg_Rip);
    RegList.push_back(X86_Reg_R8);
    RegList.push_back(X86_Reg_R9);
    RegList.push_back(X86_Reg_R10);
    RegList.push_back(X86_Reg_R11);
    RegList.push_back(X86_Reg_R12);
    RegList.push_back(X86_Reg_R13);
    RegList.push_back(X86_Reg_R14);
    RegList.push_back(X86_Reg_R15);
    break;
  }
}

bool X86Architecture::X86CpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  if (m_Bits == X86_Bit_16)
  {
    rLinearAddress = rLogicalAddress.GetBase() * 0x10 + rLogicalAddress.GetOffset();
    return true;
  }

  if (CpuContext::Translate(rLogicalAddress, rLinearAddress) == true)
    return true;

  return false;
}

std::string X86Architecture::X86CpuContext::ToString(void) const
{
  std::string Result = "";

  std::string FmtFlags = "";
  FmtFlags += m_Context.CF ? 'C' : 'c';
  FmtFlags += m_Context.PF ? 'P' : 'p';
  FmtFlags += m_Context.AF ? 'A' : 'a';
  FmtFlags += m_Context.ZF ? 'Z' : 'z';
  FmtFlags += m_Context.SF ? 'S' : 's';
  FmtFlags += m_Context.TF ? 'T' : 't';
  FmtFlags += m_Context.IF ? 'I' : 'i';
  FmtFlags += m_Context.DF ? 'D' : 'd';
  FmtFlags += m_Context.OF ? 'O' : 'o';

  switch (m_Bits)
  {
  case X86_Bit_16:
    Result = (boost::format("ax: %04x bx: %04x cx: %04x dx: %04x\nsi: %04x di: %04x sp: %04x bp: %04x\nip: %04x flags: %s")
      % m_Context.a.w % m_Context.b.w % m_Context.c.w % m_Context.d.w % m_Context.si.w % m_Context.di.w % m_Context.sp.w % m_Context.bp.w % m_Context.ip.w % FmtFlags).str();
    break;

  case X86_Bit_32:
    Result = (boost::format("eax: %08x ebx: %08x ecx: %08x edx: %08x\nesi: %08x edi: %08x esp: %08x ebp: %08x\neip: %08x eflags: %s")
      % m_Context.a.e % m_Context.b.e % m_Context.c.e % m_Context.d.e % m_Context.si.e % m_Context.di.e % m_Context.sp.e % m_Context.bp.e % m_Context.ip.e % FmtFlags).str();
    break;

  case X86_Bit_64:
    Result = (boost::format("rax: %016x rbx:    %016x rcx: %016x rdx: %016x\n"
      "rsi: %016x rdi:    %016x rsp: %016x rbp: %016x\n"
      "r8:  %016x r9:     %016x r10: %016x r11: %016x\n"
      "r12: %016x r13:    %016x r14: %016x r15: %016x\n"
      "rip: %016x rflags: %s")
      % m_Context.a.r   % m_Context.b.r   % m_Context.c.r   % m_Context.d.r
      % m_Context.si.r  % m_Context.di.r  % m_Context.sp.r  % m_Context.bp.r
      % m_Context.r8.r  % m_Context.r9.r  % m_Context.r10.r % m_Context.r11.r
      % m_Context.r12.r % m_Context.r13.r % m_Context.r14.r % m_Context.r15.r
      % m_Context.ip.r  % FmtFlags).str();
    break;

  default: return "";
  }

  Result += (boost::format("\ncs: %04x ds: %04x es: %04x ss: %04x fs: %04x gs: %04x")
    % m_Context.cs % m_Context.ds % m_Context.es % m_Context.ss % m_Context.fs % m_Context.gs).str();
  Result += "\n";
  return Result;
}