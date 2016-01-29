#include "x86_calling_convention.hpp"

MEDUSA_NAMESPACE_USE

// cdecl //////////////////////////////////////////////////////////////////////

bool CdeclCallingConvention::GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const
{
  switch (m_Mode)
  {
  case X86_Bit_16:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u16 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Sp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u16 ParamVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr + (ParamNr + 1) * 2, ParamVal))
      return false;
    rParamVal = BitVector(ParamVal);
    return true;
  }

  case X86_Bit_32:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u32 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Esp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u32 ParamVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr + (ParamNr + 1) * 4, ParamVal))
      return false;
    rParamVal = BitVector(ParamVal);
    return true;
  }

  default:
    return false;
  }
}

bool CdeclCallingConvention::ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const
{
  switch (m_Mode)
  {
  case X86_Bit_16:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u16 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Sp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u16 RetVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr, RetVal))
      return false;
    if (!pCpuCtxt->WriteRegister(X86_Reg_Ip, RetVal))
      return false;
    StkOff += 2;
    if (!pCpuCtxt->WriteRegister(X86_Reg_Sp, StkOff))
      return false;
    return true;
  }

  case X86_Bit_32:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u32 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Esp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u32 RetVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr, RetVal))
      return false;
    if (!pCpuCtxt->WriteRegister(X86_Reg_Eip, RetVal))
      return false;
    StkOff += 4;
    if (!pCpuCtxt->WriteRegister(X86_Reg_Esp, StkOff))
      return false;
    return true;
  }

  default:
    return false;
  }
}

bool CdeclCallingConvention::ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const
{
  if (!ReturnFromFunction(pCpuCtxt, pMemCtxt, ParamNo))
    return false;
  switch (m_Mode)
  {
  case X86_Bit_16: return pCpuCtxt->WriteRegister(X86_Reg_Ax, rRetVal);
  case X86_Bit_32: return pCpuCtxt->WriteRegister(X86_Reg_Eax, rRetVal);
  default:         return false;
  }
}

Expression::SPType CdeclCallingConvention::EmitGetParameter(u16 ParamNr, ValueType ParamTy) const
{
  switch (m_Mode)
  {
  case X86_Bit_16: return Expr::MakeMem(16, Expr::MakeId(X86_Reg_Ss, &m_rCpuInfo),
    Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Sp, &m_rCpuInfo), Expr::MakeBitVector(BitVector(16, ParamNr * 2))));
  case X86_Bit_32: return Expr::MakeMem(32, Expr::MakeId(X86_Reg_Ss, &m_rCpuInfo),
    Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Esp, &m_rCpuInfo), Expr::MakeBitVector(BitVector(32, ParamNr * 4))));
  default: return nullptr;
  }
}

Expression::SPType CdeclCallingConvention::EmitReturnFromFunction(u16 ParamNo) const
{
  // TODO(wisk)
  return nullptr;
}

Expression::SPType CdeclCallingConvention::EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const
{
  // TODO(wisk)
  return nullptr;
}

bool CdeclCallingConvention::AnalyzeArgument(Expression::SPType spExpr, u16& rArgNr, CallingConvention::ValueType& rArgTy) const
{
  // TODO(wisk)
  return false;
}

bool CdeclCallingConvention::AnalyzeParameter(Expression::SPType spExpr, u16& rParamNr, CallingConvention::ValueType& rParamTy) const
{
  auto spMemExpr = expr_cast<MemoryExpression>(spExpr);
  if (spMemExpr == nullptr)
    return false;
  auto spBinOp = expr_cast<BinaryOperationExpression>(spMemExpr->GetOffsetExpression());
  if (spBinOp == nullptr)
    return false;
  if (spBinOp->GetOperation() != OperationExpression::OpAdd)
    return false;
  auto spSpId = expr_cast<IdentifierExpression>(spBinOp->GetLeftExpression());
  if (spSpId == nullptr)
    return false;
  auto spStkOff = expr_cast<BitVectorExpression>(spBinOp->GetRightExpression());
  if (spStkOff == nullptr)
    return false;
  auto StkOff = spStkOff->GetInt().ConvertTo<s32>();

  switch (m_Mode)
  {
  case X86_Bit_16:
  {
    if (spSpId->GetId() != X86_Reg_Sp)
      return false;
    if (StkOff < 2)
      return false;
    if ((StkOff & 0x1) != 0x0)
      return false;
    rParamNr = StkOff >> 1;
    rParamTy = IntegerType;
    break;
  }

  case X86_Bit_32:
  {
    if (spSpId->GetId() != X86_Reg_Esp)
      return false;
    if (StkOff < 4)
      return false;
    if ((StkOff & 0x2) != 0x0)
      return false;
    rParamNr = StkOff >> 2;
    rParamTy = IntegerType;
    break;
  }

  default:
    return false;
  }

  return true;
}

CallingConvention::RegisterType CdeclCallingConvention::GetRegisterType(u32 Id) const
{
  switch (m_Mode)
  {
  case X86_Bit_16:
    switch (Id)
    {
    case X86_Reg_Ax: case X86_Reg_Cx: case X86_Reg_Dx:
      return CallingConvention::VolatileRegister;
    case X86_Reg_Bx: case X86_Reg_Si: case X86_Reg_Di:
    case X86_Reg_Sp: case X86_Reg_Bp: return CallingConvention::NonVolatileRegister;
    default: return CallingConvention::UnknownRegister;
    }
  case X86_Bit_32:
    switch (Id)
    {
    case X86_Reg_Eax: case X86_Reg_Ecx: case X86_Reg_Edx:
      return CallingConvention::VolatileRegister;
    case X86_Reg_Ebx: case X86_Reg_Esi: case X86_Reg_Edi:
    case X86_Reg_Esp: case X86_Reg_Ebp:
      return CallingConvention::NonVolatileRegister;
    default: return CallingConvention::UnknownRegister;
    }
  default: return CallingConvention::UnknownRegister;
  }
}

CallingConvention::StackCleanerType CdeclCallingConvention::StackCleanupBy(void) const
{
  return StackCleanedByCaller;
}

// stdcall ////////////////////////////////////////////////////////////////////

bool StdCallCallingConvention::GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const
{
  switch (m_Mode)
  {
  case X86_Bit_16:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u16 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Sp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u16 ParamVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr + (ParamNr + 1) * 2, ParamVal))
      return false;
    rParamVal = BitVector(ParamVal);
    return true;
  }

  case X86_Bit_32:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u32 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Esp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u32 ParamVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr + (ParamNr + 1) * 4, ParamVal))
      return false;
    rParamVal = BitVector(ParamVal);
    return true;
  }

  default:
    return false;
  }
}

bool StdCallCallingConvention::ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const
{
  switch (m_Mode)
  {
  case X86_Bit_16:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u16 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Sp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u16 RetVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr, RetVal))
      return false;
    if (!pCpuCtxt->WriteRegister(X86_Reg_Ip, RetVal))
      return false;
    StkOff += (2 + ParamNo * 2);
    if (!pCpuCtxt->WriteRegister(X86_Reg_Sp, StkOff))
      return false;
    return true;
  }

  case X86_Bit_32:
  {
    u16 StkSeg;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Ss, StkSeg))
      return false;
    u32 StkOff;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Esp, StkOff))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(StkSeg, StkOff), StkLinAddr))
      return false;
    u32 RetVal;
    if (!pMemCtxt->ReadMemory(StkLinAddr, RetVal))
      return false;
    if (!pCpuCtxt->WriteRegister(X86_Reg_Eip, RetVal))
      return false;
    StkOff += (4 + ParamNo * 4);
    if (!pCpuCtxt->WriteRegister(X86_Reg_Esp, StkOff))
      return false;
    return true;
  }

  default:
    return false;
  }
}

bool StdCallCallingConvention::ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const
{
  if (!ReturnFromFunction(pCpuCtxt, pMemCtxt, ParamNo))
    return false;
  switch (m_Mode)
  {
  case X86_Bit_16: return pCpuCtxt->WriteRegister(X86_Reg_Ax, rRetVal);
  case X86_Bit_32: return pCpuCtxt->WriteRegister(X86_Reg_Eax, rRetVal);
  default:         return false;
  }
}

Expression::SPType StdCallCallingConvention::EmitGetParameter(u16 ParamNr, ValueType ParamTy) const
{
  switch (m_Mode)
  {
  case X86_Bit_16: return Expr::MakeMem(16, Expr::MakeId(X86_Reg_Ss, &m_rCpuInfo),
    Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Sp, &m_rCpuInfo), Expr::MakeBitVector(BitVector(16, ParamNr * 2))));
  case X86_Bit_32: return Expr::MakeMem(32, Expr::MakeId(X86_Reg_Ss, &m_rCpuInfo),
    Expr::MakeBinOp(OperationExpression::OpAdd, Expr::MakeId(X86_Reg_Esp, &m_rCpuInfo), Expr::MakeBitVector(BitVector(32, ParamNr * 4))));
  default: return nullptr;
  }
}

Expression::SPType StdCallCallingConvention::EmitReturnFromFunction(u16 ParamNo) const
{
  // TODO(wisk)
  return nullptr;
}

Expression::SPType StdCallCallingConvention::EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const
{
  // TODO(wisk)
  return nullptr;
}

bool StdCallCallingConvention::AnalyzeArgument(Expression::SPType spExpr, u16& rArgNr, CallingConvention::ValueType& rArgTy) const
{
  // TODO(wisk)
  return false;
}

bool StdCallCallingConvention::AnalyzeParameter(Expression::SPType spExpr, u16& rParamNr, CallingConvention::ValueType& rParamTy) const
{
  auto spMemExpr = expr_cast<MemoryExpression>(spExpr);
  if (spMemExpr == nullptr)
    return false;
  auto spBinOp = expr_cast<BinaryOperationExpression>(spMemExpr->GetOffsetExpression());
  if (spBinOp == nullptr)
    return false;
  if (spBinOp->GetOperation() != OperationExpression::OpAdd)
    return false;
  auto spSpId = expr_cast<IdentifierExpression>(spBinOp->GetLeftExpression());
  if (spSpId == nullptr)
    return false;
  auto spStkOff = expr_cast<BitVectorExpression>(spBinOp->GetRightExpression());
  if (spStkOff == nullptr)
    return false;
  auto StkOff = spStkOff->GetInt().ConvertTo<s32>();

  switch (m_Mode)
  {
  case X86_Bit_16:
  {
    if (spSpId->GetId() != X86_Reg_Sp)
      return false;
    if (StkOff < 2)
      return false;
    if ((StkOff & 0x1) != 0x0)
      return false;
    rParamNr = StkOff >> 1;
    rParamTy = IntegerType;
    break;
  }

  case X86_Bit_32:
  {
    if (spSpId->GetId() != X86_Reg_Esp)
      return false;
    if (StkOff < 4)
      return false;
    if ((StkOff & 0x2) != 0x0)
      return false;
    rParamNr = StkOff >> 2;
    rParamTy = IntegerType;
    break;
  }

  default:
    return false;
  }

  return true;
}

CallingConvention::RegisterType StdCallCallingConvention::GetRegisterType(u32 Id) const
{
  switch (m_Mode)
  {
  case X86_Bit_16:
    switch (Id)
    {
    case X86_Reg_Ax: case X86_Reg_Cx: case X86_Reg_Dx:
      return CallingConvention::VolatileRegister;
    case X86_Reg_Bx: case X86_Reg_Si: case X86_Reg_Di:
    case X86_Reg_Sp: case X86_Reg_Bp: return CallingConvention::NonVolatileRegister;
    default: return CallingConvention::UnknownRegister;
    }
  case X86_Bit_32:
    switch (Id)
    {
    case X86_Reg_Eax: case X86_Reg_Ecx: case X86_Reg_Edx:
      return CallingConvention::VolatileRegister;
    case X86_Reg_Ebx: case X86_Reg_Esi: case X86_Reg_Edi:
    case X86_Reg_Esp: case X86_Reg_Ebp:
      return CallingConvention::NonVolatileRegister;
    default: return CallingConvention::UnknownRegister;
    }
  default: return CallingConvention::UnknownRegister;
  }
}

CallingConvention::StackCleanerType StdCallCallingConvention::StackCleanupBy(void) const
{
  return StackCleanedByCallee;
}

// Microsoft x64 //////////////////////////////////////////////////////////////

bool MsX64CallingConvention::GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const
{
  u32 IdParam = 0;
  switch (ParamNr)
  {
  case 0: IdParam = X86_Reg_Rcx; break;
  case 1: IdParam = X86_Reg_Rdx; break;
  case 2: IdParam = X86_Reg_R8;  break;
  case 3: IdParam = X86_Reg_R9;  break;
  default:                       break;
  }

  u64 ParamVal;
  if (IdParam != 0)
  {
    if (!pCpuCtxt->ReadRegister(IdParam, ParamVal))
      return false;
  }
  else
  {
    u64 StkVal;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Rsp, StkVal))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(0x0, StkVal), StkLinAddr))
      return false;
    StkLinAddr += (ParamNr * 0x8);
    if (!pMemCtxt->ReadMemory(StkLinAddr, ParamVal))
      return false;
  }

  rParamVal = BitVector(ParamVal);
  return true;
}

bool MsX64CallingConvention::ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const
{
  u64 StkVal;
  if (!pCpuCtxt->ReadRegister(X86_Reg_Rsp, StkVal))
    return false;
  u64 StkLinAddr;
  if (!pCpuCtxt->Translate(Address(0x0, StkVal), StkLinAddr))
    return false;
  u64 RetVal;
  if (!pMemCtxt->ReadMemory(StkLinAddr, RetVal))
    return false;
  if (!pCpuCtxt->WriteRegister(X86_Reg_Rip, RetVal))
    return false;
  StkVal += 8;
  if (!pCpuCtxt->WriteRegister(X86_Reg_Rsp, StkVal))
    return false;
  return true;
}

bool MsX64CallingConvention::ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const
{
  if (!ReturnFromFunction(pCpuCtxt, pMemCtxt, ParamNo))
    return false;
  return pCpuCtxt->WriteRegister(X86_Reg_Rax, rRetVal);
}

Expression::SPType MsX64CallingConvention::EmitGetParameter(u16 ParamNr, ValueType ParamTy) const
{
  u32 Reg = 0;
  switch (ParamNr)
  {
  case 0: Reg = X86_Reg_Rcx; break;
  case 1: Reg = X86_Reg_Rdx; break;
  case 2: Reg = X86_Reg_R8;  break;
  case 3: Reg = X86_Reg_R9;  break;
  default:                   return nullptr; // TODO(wisk): handle stack based parameter
  }
  return Expr::MakeId(Reg, &m_rCpuInfo);
}

Expression::SPType MsX64CallingConvention::EmitReturnFromFunction(u16 ParamNo) const
{
  // TODO(wisk)
  return nullptr;
}

Expression::SPType MsX64CallingConvention::EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const
{
  // TODO(wisk)
  return nullptr;
}

bool MsX64CallingConvention::AnalyzeArgument(Expression::SPType spExpr, u16& rArgNr, CallingConvention::ValueType& rArgTy) const
{
  // TODO(wisk)
  return false;
}

bool MsX64CallingConvention::AnalyzeParameter(Expression::SPType spExpr, u16& rParamNr, CallingConvention::ValueType& rParamTy) const
{
  // TODO(wisk)
  return false;
}

// src: https://msdn.microsoft.com/en-us/library/6t169e9c.aspx
CallingConvention::RegisterType MsX64CallingConvention::GetRegisterType(u32 Id) const
{
  switch (Id)
  {
  case X86_Reg_Rax: case X86_Reg_Rcx: case X86_Reg_Rdx: case X86_Reg_R8: case X86_Reg_R9: case X86_Reg_R10: case X86_Reg_R11:
    return CallingConvention::VolatileRegister;
  case X86_Reg_Rbx: case X86_Reg_Rbp: case X86_Reg_Rdi: case X86_Reg_Rsi: case X86_Reg_Rsp: case X86_Reg_R12: case X86_Reg_R13: case X86_Reg_R14: case X86_Reg_R15:
    return CallingConvention::NonVolatileRegister;
  default:
    return CallingConvention::UnknownRegister;
  }
}

CallingConvention::StackCleanerType MsX64CallingConvention::StackCleanupBy(void) const
{
  return StackCleanedByCallee;
}

// System V ///////////////////////////////////////////////////////////////////

bool SystemVCallingConvention::GetParameter(CpuContext const* pCpuCtxt, MemoryContext const* pMemCtxt, u16 ParamNr, BitVector& rParamVal) const
{
  u32 IdParam = 0;
  switch (ParamNr)
  {
  case 0: IdParam = X86_Reg_Rdi; break;
  case 1: IdParam = X86_Reg_Rsi; break;
  case 2: IdParam = X86_Reg_Rdx; break;
  case 3: IdParam = X86_Reg_Rcx; break;
  case 4: IdParam = X86_Reg_R8;  break;
  case 5: IdParam = X86_Reg_R9;  break;
  default:                       break;
  }

  u64 ParamVal;
  if (IdParam != 0)
  {
    if (!pCpuCtxt->ReadRegister(IdParam, ParamVal))
      return false;
  }
  else
  {
    u64 StkVal;
    if (!pCpuCtxt->ReadRegister(X86_Reg_Rsp, StkVal))
      return false;
    u64 StkLinAddr;
    if (!pCpuCtxt->Translate(Address(0x0, StkVal), StkLinAddr))
      return false;
    StkLinAddr += (0x8 + ParamNr * 0x8); // FIXME(wisk): figure out the real offset
    if (!pMemCtxt->ReadMemory(StkLinAddr, ParamVal))
      return false;
  }

  rParamVal = BitVector(ParamVal);
  return true;
}

bool SystemVCallingConvention::ReturnFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo) const
{
  u64 StkVal;
  if (!pCpuCtxt->ReadRegister(X86_Reg_Rsp, StkVal))
    return false;
  u64 StkLinAddr;
  if (!pCpuCtxt->Translate(Address(0x0, StkVal), StkLinAddr))
    return false;
  u64 RetVal;
  if (!pMemCtxt->ReadMemory(StkLinAddr, RetVal))
    return false;
  if (!pCpuCtxt->WriteRegister(X86_Reg_Rip, RetVal))
    return false;
  StkVal += 8;
  if (!pCpuCtxt->WriteRegister(X86_Reg_Rsp, StkVal))
    return false;
  return true;
}

bool SystemVCallingConvention::ReturnValueFromFunction(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, u16 ParamNo, BitVector const& rRetVal) const
{
  if (!ReturnFromFunction(pCpuCtxt, pMemCtxt, ParamNo))
    return false;
  return pCpuCtxt->WriteRegister(X86_Reg_Rax, rRetVal);
}

Expression::SPType SystemVCallingConvention::EmitGetParameter(u16 ParamNr, ValueType ParamTy) const
{
  // TODO(wisk)
  return nullptr;
}

Expression::SPType SystemVCallingConvention::EmitReturnFromFunction(u16 ParamNo) const
{
  // TODO(wisk)
  return nullptr;
}

Expression::SPType SystemVCallingConvention::EmitReturnValueFromFunction(u16 ParamNo, BitVector const& rRetVal) const
{
  // TODO(wisk)
  return nullptr;
}

bool SystemVCallingConvention::AnalyzeArgument(Expression::SPType spExpr, u16& rArgNr, CallingConvention::ValueType& rArgTy) const
{
  // TODO(wisk)
  return false;
}

bool SystemVCallingConvention::AnalyzeParameter(Expression::SPType spExpr, u16& rParamNr, CallingConvention::ValueType& rParamTy) const
{
  // TODO(wisk)
  return false;
}

CallingConvention::RegisterType SystemVCallingConvention::GetRegisterType(u32 Id) const
{
  switch (Id)
  {
  case X86_Reg_Rax: case X86_Reg_Rcx: case X86_Reg_Rdx: case X86_Reg_Rdi: case X86_Reg_Rsi: case X86_Reg_R8: case X86_Reg_R9: case X86_Reg_R10: case X86_Reg_R11:
    return CallingConvention::VolatileRegister;
  case X86_Reg_Rbx: case X86_Reg_Rbp: case X86_Reg_Rsp: case X86_Reg_R12: case X86_Reg_R13: case X86_Reg_R14: case X86_Reg_R15:
    return CallingConvention::NonVolatileRegister;
  default:
    return CallingConvention::UnknownRegister;
  }
}

CallingConvention::StackCleanerType SystemVCallingConvention::StackCleanupBy(void) const
{
  return StackCleanedByCallee;
}