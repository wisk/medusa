#include "medusa/types.hpp"
#include "medusa/endian.hpp"

MEDUSA_NAMESPACE_BEGIN

std::string IntType::ToString(u16 Base) const
{
  std::ostringstream Out;
  switch (Base)
  {
  case 16: Out << "0x" << std::setw(m_BitSize / 8) << std::hex; break;
  default: break;
  }
  Out << std::setfill('0') << GetUnsignedValue();
  return Out.str();
}

void IntType::SignExtend(u16 NewBitSize)
{
  //assert(NewBitSize > m_BitSize);
  ap_int Mask = (ap_int(1) << NewBitSize) - 1;
  m_Value = Mask & m_Value;
  m_BitSize = NewBitSize;
}

void IntType::ZeroExtend(u16 NewBitSize)
{
  //assert(NewBitSize > m_BitSize);
  m_BitSize = NewBitSize;
}

void IntType::BitCast(u16 NewBitSize)
{
  m_BitSize = NewBitSize;
  ap_int Mask = (ap_int(1) << m_BitSize) - 1;
  m_Value &= Mask;
}

ap_int IntType::GetSignedValue(void) const
{
  // If the value is positive, we don't need to do anything
  if (!m_Value.sign())
    return m_Value;

  // ... otherwise we need to re-encode it
  ap_int Mask = ((ap_int)1 << m_BitSize) - 1;
  return -((m_Value ^ Mask) + 1);
}

ap_uint IntType::GetUnsignedValue(void) const
{
  // If the value is positive, we don't need to do anything
  if (!m_Value.backend().sign())
    return m_Value;

  // ... otherwise we need to re-encode it
  ap_uint InsertMask = (ap_int(1) << m_BitSize) - 1;
  ap_uint ClearMask = (ap_uint(1) << Msb().ConvertTo<u32>()) - 1;
  ap_uint Mask = InsertMask - ClearMask;
  return Mask | boost::multiprecision::abs(m_Value);
}

IntType IntType::Not(void) const
{
  IntType Tmp(m_BitSize, ~m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType IntType::Neg(void) const
{
  IntType Tmp(m_BitSize, -m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::PreInc(void)
{
  ++m_Value;
  _Adjust();
  return *this;
}

IntType IntType::PostInc(void)
{
  IntType Tmp(m_BitSize, m_Value + 1);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::PreDec(void)
{
  --m_Value;
  _Adjust();
  return *this;
}

IntType IntType::PostDec(void)
{
  IntType Tmp(m_BitSize, m_Value - 1);
  Tmp._Adjust();
  return Tmp;
}

IntType IntType::Bsf(void) const
{
  return Lsb();
}

IntType IntType::Lsb(void) const
{
  if (m_Value.is_zero())
    return IntType(m_BitSize, 0);
  return IntType(m_BitSize, boost::multiprecision::lsb(m_Value));
}

IntType IntType::Bsr(void) const
{
  return Msb();
}

IntType IntType::Msb(void) const
{
  if (m_Value.is_zero())
    // Avoid "No bits were set in the operand."
    return IntType(m_BitSize, 0);
  if (m_Value.backend().sign())
    // HACK(KS): to avoid "Testing individual bits in negative values is not supported - results are undefined."
    return IntType(m_BitSize, boost::multiprecision::msb(boost::multiprecision::abs(m_Value)));
  return IntType(m_BitSize, boost::multiprecision::msb(m_Value));
}

IntType IntType::Swap(void) const
{
  switch (m_BitSize)
  {
  case 16:
    {
      auto Res = ConvertTo<u16>();
      EndianSwap(Res);
      IntType Tmp(m_BitSize, Res);
    }

  case 32:
  {
    auto Res = ConvertTo<u32>();
    EndianSwap(Res);
    IntType Tmp(m_BitSize, Res);
  }

  case 64:
  {
    auto Res = ConvertTo<u64>();
    EndianSwap(Res);
    IntType Tmp(m_BitSize, Res);
  }

  // TODO(KS):
  default:
    return *this;
  }
}

IntType IntType::Add(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value + rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::AddAssign(IntType const& rVal)
{
  m_Value += rVal.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Sub(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value - rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::SubAssign(IntType const& rVal)
{
  m_Value -= rVal.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Mul(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value * rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::MulAssign(IntType const& rVal)
{
  m_Value *= rVal.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::UDiv(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value / rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::UDivAssign(IntType const& rVal)
{
  IntType Tmp = UDiv(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::SDiv(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value / rVal.GetSignedValue());
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::SDivAssign(IntType const& rVal)
{
  IntType Tmp = UDiv(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::UMod(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value % rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::UModAssign(IntType const& rVal)
{
  IntType Tmp = UMod(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::SMod(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value % rVal.GetSignedValue());
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::SModAssign(IntType const& rVal)
{
  IntType Tmp = SMod(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::And(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value & rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::AndAssign(IntType const& rVal)
{
  m_Value &= rVal.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Or(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value | rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::OrAssign(IntType const& rVal)
{
  m_Value |= rVal.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Xor(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value ^ rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::XorAssign(IntType const& rVal)
{
  m_Value ^= rVal.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Lls(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value << rVal.ConvertTo<u32>());
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::LlsAssign(IntType const& rVal)
{
  IntType Tmp = Lls(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Lrs(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value >> rVal.ConvertTo<u32>());
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::LrsAssign(IntType const& rVal)
{
  IntType Tmp = Lrs(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

IntType IntType::Ars(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  IntType Tmp(m_BitSize, m_Value >> rVal.ConvertTo<u32>());
  Tmp._Adjust();
  return Tmp;
}

IntType& IntType::ArsAssign(IntType const& rVal)
{
  IntType Tmp = Ars(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

void IntType::_Adjust(void)
{
  ap_int Mask = (ap_int(1) << m_BitSize) - 1;
  m_Value = Mask & GetUnsignedValue();
}

MEDUSA_NAMESPACE_END