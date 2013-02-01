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
#define READ_F(flg)   *reinterpret_cast<u32 *>(pValue) = (m_Context.flags & (1 << (flg))) ? 1 : 0;

  switch (Register)
  {
  case X86_FlCf:    READ_F(X86_CfBit); break;
  case X86_FlPf:    READ_F(X86_PfBit); break;
  case X86_FlAf:    READ_F(X86_AfBit); break;
  case X86_FlZf:    READ_F(X86_ZfBit); break;
  case X86_FlSf:    READ_F(X86_SfBit); break;
  case X86_FlTf:    READ_F(X86_TfBit); break;
  case X86_FlIf:    READ_F(X86_IfBit); break;
  case X86_FlDf:    READ_F(X86_DfBit); break;
  case X86_FlOf:    READ_F(X86_OfBit); break;

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

  return true;
}

bool X86Architecture::X86CpuContext::WriteRegister(u32 Register, void const* pValue, u32 Size)
{
  auto RegisterSize = m_rCpuInfo.GetSizeOfRegisterInBit(Register) / 8;
  if (RegisterSize != Size) return false;

#define WRITE_R_L(reg) m_Context.reg.x.l = *reinterpret_cast<u8  const*>(pValue);
#define WRITE_R_H(reg) m_Context.reg.x.h = *reinterpret_cast<u8  const*>(pValue);
#define WRITE_R_W(reg) m_Context.reg.w   = *reinterpret_cast<u16 const*>(pValue);
#define WRITE_S(seg)   m_Context.seg     = *reinterpret_cast<u16 const*>(pValue);
#define WRITE_R_E(reg) m_Context.reg.e   = *reinterpret_cast<u32 const*>(pValue);
#define WRITE_R_R(reg) m_Context.reg.r   = *reinterpret_cast<u64 const*>(pValue);
#define WRITE_F(flg)   if (*reinterpret_cast<u8 const*>(pValue) == 0) { m_Context.flags &= ~(1 << (flg)); } else { m_Context.flags |= (1 << (flg)); }

  switch (Register)
  {
  case X86_FlCf:    WRITE_F(X86_CfBit); break;
  case X86_FlPf:    WRITE_F(X86_PfBit); break;
  case X86_FlAf:    WRITE_F(X86_AfBit); break;
  case X86_FlZf:    WRITE_F(X86_ZfBit); break;
  case X86_FlSf:    WRITE_F(X86_SfBit); break;
  case X86_FlTf:    WRITE_F(X86_TfBit); break;
  case X86_FlIf:    WRITE_F(X86_IfBit); break;
  case X86_FlDf:    WRITE_F(X86_DfBit); break;
  case X86_FlOf:    WRITE_F(X86_OfBit); break;

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

  return true;
}

bool X86Architecture::X86CpuContext::Translate(Address const& rLogicalAddress, u64& rLinearAddress) const
{
  if (m_rCfg.Get("Bit") == X86_Bit_16)
  {
    rLinearAddress = rLogicalAddress.GetBase() * 0x10 + rLogicalAddress.GetOffset();
    return true;
  }

  return CpuContext::Translate(rLogicalAddress, rLinearAddress);
}

std::string X86Architecture::X86CpuContext::ToString(void) const
{
  std::string Result = "";
  switch (m_rCfg.Get("Bit"))
  {
  case X86_Bit_16:
    Result = (boost::format("ax: %04x bx: %04x cx: %04x dx: %04x\nsi: %04x di: %04x sp: %04x bp: %04x\nip: %04x flags: %04x")
      % m_Context.a.w % m_Context.b.w % m_Context.c.w % m_Context.d.w % m_Context.si.w % m_Context.di.w % m_Context.sp.w % m_Context.bp.w % m_Context.ip.w % m_Context.flags).str();
    break;

  case X86_Bit_32:
    Result = (boost::format("eax: %08x ebx: %08x ecx: %08x edx: %08x\nesi: %08x edi: %08x esp: %08x ebp: %08x\neip: %08x eflags: %08x")
      % m_Context.a.e % m_Context.b.e % m_Context.c.e % m_Context.d.e % m_Context.si.e % m_Context.di.e % m_Context.sp.e % m_Context.bp.e % m_Context.ip.e % m_Context.flags).str();
    break;

  case X86_Bit_64:
    Result = (boost::format("rax: %016x rbx:    %016x rcx: %016x rdx: %016x\n"
      "rsi: %016x rdi:    %016x rsp: %016x rbp: %016x\n"
      "r8:  %016x r9:     %016x r10: %016x r11: %016x\n"
      "r12: %016x r13:    %016x r14: %016x r15: %016x\n"
      "rip: %016x rflags: %016x")
      % m_Context.a.r   % m_Context.b.r   % m_Context.c.r   % m_Context.d.r
      % m_Context.si.r  % m_Context.di.r  % m_Context.sp.r  % m_Context.bp.r
      % m_Context.r8.r  % m_Context.r9.r  % m_Context.r10.r % m_Context.r11.r
      % m_Context.r12.r % m_Context.r13.r % m_Context.r14.r % m_Context.r15.r
      % m_Context.ip.r  % m_Context.flags).str();
    break;

  default: return "";
  }

  Result += (boost::format("\ncs: %04x ds: %04x es: %04x ss: %04x fs: %04x gs: %04x")
    % m_Context.cs % m_Context.ds % m_Context.es % m_Context.ss % m_Context.fs % m_Context.gs).str();

  return Result;

}