#include "medusa/types.hpp"
#include "medusa/endian.hpp"

MEDUSA_NAMESPACE_BEGIN

std::string IntType::ToString(u16 Base) const
{
  std::ostringstream Out;
  Out << std::setw(m_BitSize / 8) << std::setfill('0') << m_Value;
  return Out.str();
}

void IntType::SignExtend(u16 NewBitSize)
{
  assert(NewBitSize > m_BitSize);
  ap_int Mask = (ap_int(1) << NewBitSize) - 1;
  m_Value = Mask & m_Value;
  m_BitSize = NewBitSize;
}

void IntType::ZeroExtend(u16 NewBitSize)
{
  assert(NewBitSize > m_BitSize);
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
  ap_int Mask = 1;
  Mask <<= m_BitSize;
  --Mask;
  ap_int Res = m_Value;
  ap_int Not = Res ^ Mask;
  ap_int Neg = Not + 1;
  return Neg * -1;
}

ap_uint IntType::GetUnsignedValue(void) const
{
  // If the value is positive, we don't need to do anything
  if (!m_Value.sign())
    return m_Value;

  // ... otherwise we need to re-encode it
  ap_uint Res = m_Value * -1;
  ap_uint InsertMask = 1;
  InsertMask <<= m_BitSize;
  --InsertMask;

  ap_uint ClearMask = (ap_uint(1) << Msb().ConvertTo<u32>()) - 1;
  ap_uint Mask = InsertMask - ClearMask;
  return Mask | Res;
}

IntType IntType::Not(void) const
{
  return IntType(m_BitSize, ~m_Value);
}

IntType IntType::Neg(void) const
{
  return IntType(m_BitSize, -m_Value);
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
  if (m_Value.sign())
    // HACK(KS): to avoid "Testing individual bits in negative values is not supported - results are undefined."
    return IntType(m_BitSize, boost::multiprecision::msb(m_Value * -1));
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
      return IntType(m_BitSize, Res);
    }

  case 32:
  {
    auto Res = ConvertTo<u32>();
    EndianSwap(Res);
    return IntType(m_BitSize, Res);
  }

  case 64:
  {
    auto Res = ConvertTo<u64>();
    EndianSwap(Res);
    return IntType(m_BitSize, Res);
  }

  // TODO(KS):
  default:
    return *this;
  }
}

IntType IntType::Add(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value + rVal.m_Value);
}

IntType IntType::Sub(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value - rVal.m_Value);
}

IntType IntType::Mul(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value * rVal.m_Value);
}

IntType IntType::UDiv(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value / rVal.m_Value);
}

IntType IntType::SDiv(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value / rVal.GetSignedValue());
}

IntType IntType::UMod(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value % rVal.m_Value);
}

IntType IntType::SMod(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value % rVal.GetSignedValue());
}

IntType IntType::And(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value & rVal.m_Value);
}

IntType IntType::Or(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value | rVal.m_Value);
}

IntType IntType::Xor(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value ^ rVal.m_Value);
}

IntType IntType::Lls(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value << rVal.ConvertTo<u32>());
}

IntType IntType::Lrs(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value >> rVal.ConvertTo<u32>());
}

IntType IntType::Ars(IntType const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  return IntType(m_BitSize, m_Value >> rVal.ConvertTo<u32>());
}

MEDUSA_NAMESPACE_END