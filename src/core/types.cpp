#include "medusa/types.hpp"
#include "medusa/endian.hpp"

MEDUSA_NAMESPACE_BEGIN

std::string BitVector::ToString(u16 Base) const
{
  std::ostringstream Out;
  switch (Base)
  {
  case 16: Out << "0x" << std::setw(m_BitSize / 8 * 2) << std::hex; break;
  default: break;
  }
  Out << std::setfill('0') << GetUnsignedValue();
  return Out.str();
}

void BitVector::SignExtend(u16 NewBitSize)
{
  u16 Pos = m_BitSize - 1;

  if (!((GetUnsignedValue() >> Pos) & 1))
  {
    m_BitSize = NewBitSize;
    return;
  }
  ap_int InsertedBits = ((ap_int(1) << NewBitSize) - 1) - ((ap_int(1) << Pos) - 1);
  m_Value |= InsertedBits;
  m_BitSize = NewBitSize;
}

void BitVector::ZeroExtend(u16 NewBitSize)
{
  //assert(NewBitSize > m_BitSize);
  m_BitSize = NewBitSize;
}

void BitVector::BitCast(u16 NewBitSize)
{
  m_BitSize = NewBitSize;
  ap_int Mask = (ap_int(1) << m_BitSize) - 1;
  m_Value &= Mask;
}

ap_int BitVector::GetSignedValue(void) const
{
  if (m_BitSize == 0)
    return 0;

  // If the value is positive, we don't need to do anything
  if (!((GetUnsignedValue() >> (m_BitSize - 1)) & 1))
    return m_Value;

  // ... otherwise we need to re-encode it
  ap_int Mask = ((ap_int)1 << m_BitSize) - 1;
  return -((m_Value ^ Mask) + 1);
}

ap_uint BitVector::GetUnsignedValue(void) const
{
  if (m_BitSize == 0)
    return 0;

  // If the value is positive, we don't need to do anything
  if (!m_Value.backend().sign())
    return m_Value;

  // ... otherwise we need to re-encode it
  ap_uint InsertMask = (ap_int(1) << m_BitSize) - 1;
  ap_uint NegValue = (boost::multiprecision::abs(m_Value) ^ InsertMask) + 1;
  return NegValue;
}

BitVector BitVector::Not(void) const
{
  BitVector Tmp(m_BitSize, ~m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector BitVector::Neg(void) const
{
  BitVector Tmp(m_BitSize, -m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::PreInc(void)
{
  ++m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::PostInc(void)
{
  BitVector Tmp(m_BitSize, m_Value + 1);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::PreDec(void)
{
  --m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::PostDec(void)
{
  BitVector Tmp(m_BitSize, m_Value - 1);
  Tmp._Adjust();
  return Tmp;
}

BitVector BitVector::Bsf(void) const
{
  return Lsb();
}

BitVector BitVector::Lsb(void) const
{
  if (m_Value.is_zero())
    return BitVector(m_BitSize, 0);
  return BitVector(m_BitSize, boost::multiprecision::lsb(m_Value));
}

BitVector BitVector::Bsr(void) const
{
  return Msb();
}

BitVector BitVector::Msb(void) const
{
  if (m_Value.is_zero())
    // Avoid "No bits were set in the operand."
    return BitVector(m_BitSize, 0);
  if (m_Value.backend().sign())
    // HACK(KS): to avoid "Testing individual bits in negative values is not supported - results are undefined."
    return BitVector(m_BitSize, boost::multiprecision::msb(boost::multiprecision::abs(m_Value)));
  return BitVector(m_BitSize, boost::multiprecision::msb(m_Value));
}

BitVector BitVector::Swap(void) const
{
  switch (m_BitSize)
  {
    case 16:
    {
      auto Res = ConvertTo<u16>();
      EndianSwap(Res);
      BitVector Tmp(m_BitSize, Res);
      return Tmp;
    }

    case 32:
    {
      auto Res = ConvertTo<u32>();
      EndianSwap(Res);
      BitVector Tmp(m_BitSize, Res);
      return Tmp;
    }

    case 64:
    {
      auto Res = ConvertTo<u64>();
      EndianSwap(Res);
      BitVector Tmp(m_BitSize, Res);
      return Tmp;
    }

    // TODO(KS):
    default:
      return *this;
  }
}

BitVector BitVector::Add(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value + rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::AddAssign(BitVector const& rVal)
{
  m_Value += rVal.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Sub(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value - rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::SubAssign(BitVector const& rVal)
{
  m_Value -= rVal.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Mul(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value * rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::MulAssign(BitVector const& rVal)
{
  m_Value *= rVal.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::UDiv(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value / rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::UDivAssign(BitVector const& rVal)
{
  BitVector Tmp = UDiv(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::SDiv(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, GetSignedValue() / rVal.GetSignedValue());
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::SDivAssign(BitVector const& rVal)
{
  BitVector Tmp = SDiv(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::UMod(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value % rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::UModAssign(BitVector const& rVal)
{
  BitVector Tmp = UMod(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::SMod(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, GetSignedValue() % rVal.GetSignedValue());
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::SModAssign(BitVector const& rVal)
{
  BitVector Tmp = SMod(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::And(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value & rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::AndAssign(BitVector const& rVal)
{
  m_Value &= rVal.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Or(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value | rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::OrAssign(BitVector const& rVal)
{
  m_Value |= rVal.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Xor(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value ^ rVal.m_Value);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::XorAssign(BitVector const& rVal)
{
  m_Value ^= rVal.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Lls(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value << rVal.ConvertTo<u32>());
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::LlsAssign(BitVector const& rVal)
{
  BitVector Tmp = Lls(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Lrs(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  BitVector Tmp(m_BitSize, m_Value >> rVal.ConvertTo<u32>());
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::LrsAssign(BitVector const& rVal)
{
  BitVector Tmp = Lrs(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Ars(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  u32 Count = rVal.ConvertTo<u32>();
  u32 ShiftedBitSize = Count > m_BitSize ? 1 : m_BitSize - Count;
  ap_int ShiftedValue = m_Value;
  if (Count > m_BitSize)
    ShiftedValue >>= m_BitSize - 1;
  else
    ShiftedValue >>= Count;
  BitVector Tmp(ShiftedBitSize, ShiftedValue);
  Tmp.SignExtend(m_BitSize);
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::ArsAssign(BitVector const& rVal)
{
  BitVector Tmp = Ars(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Rol(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  u32 Count = rVal.ConvertTo<u32>() % m_BitSize;
  BitVector Tmp(m_BitSize, (m_Value << Count) | (m_Value >> (m_BitSize - Count)));
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::RolAssign(BitVector const& rVal)
{
  BitVector Tmp = Rol(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::Ror(BitVector const& rVal) const
{
  //assert(m_BitSize == rVal.GetBitSize());
  u32 Count = rVal.ConvertTo<u32>() % m_BitSize;
  BitVector Tmp(m_BitSize, (m_Value >> Count) | (m_Value << (m_BitSize - Count)));
  Tmp._Adjust();
  return Tmp;
}

BitVector& BitVector::RorAssign(BitVector const& rVal)
{
  BitVector Tmp = Ror(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::FAdd(BitVector const& rVal) const
{
  switch (m_BitSize)
  {
  case 32:
    return BitVector(static_cast<float>(ConvertTo<float>() + rVal.ConvertTo<float>()));
  case 64:
    return BitVector(static_cast<double>(ConvertTo<double>() + rVal.ConvertTo<double>()));
  default:
    return *this;
  }
}

BitVector& BitVector::FAddAssign(BitVector const& rVal)
{
  BitVector Tmp = FAdd(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::FSub(BitVector const& rVal) const
{
  switch (m_BitSize)
  {
  case 32:
    return BitVector(ConvertTo<float>() - rVal.ConvertTo<float>());
  case 64:
    return BitVector(ConvertTo<double>() - rVal.ConvertTo<double>());
  default:
    return *this;
  }
}

BitVector& BitVector::FSubAssign(BitVector const& rVal)
{
  BitVector Tmp = FSub(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::FMul(BitVector const& rVal) const
{
  switch (m_BitSize)
  {
  case 32:
    return BitVector(ConvertTo<float>() * rVal.ConvertTo<float>());
  case 64:
    return BitVector(ConvertTo<double>() * rVal.ConvertTo<double>());
  default:
    return *this;
  }
}

BitVector& BitVector::FMulAssign(BitVector const& rVal)
{
  BitVector Tmp = FMul(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::FDiv(BitVector const& rVal) const
{
  switch (m_BitSize)
  {
  case 32:
    return BitVector(ConvertTo<float>() / rVal.ConvertTo<float>());
  case 64:
    return BitVector(ConvertTo<double>() / rVal.ConvertTo<double>());
  default:
    return *this;
  }
}

BitVector& BitVector::FDivAssign(BitVector const& rVal)
{
  BitVector Tmp = FDiv(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

BitVector BitVector::FMod(BitVector const& rVal) const
{
  switch (m_BitSize)
  {
  case 32:
    return BitVector(std::fmod(ConvertTo<float>(), rVal.ConvertTo<float>()));
  case 64:
    return BitVector(std::fmod(ConvertTo<double>(), rVal.ConvertTo<double>()));
  default:
    return *this;
  }
}

BitVector& BitVector::FModAssign(BitVector const& rVal)
{
  BitVector Tmp = FMod(rVal);
  m_Value = Tmp.m_Value;
  _Adjust();
  return *this;
}

void BitVector::_Adjust(void)
{
  ap_int Mask = (ap_int(1) << m_BitSize) - 1;
  m_Value = Mask & GetUnsignedValue();
}

template<>
float BitVector::ConvertTo<float>(void) const
{
  auto Tmp = ConvertTo<u32>();
  return *reinterpret_cast<float const*>(&Tmp);
}

template<>
double BitVector::ConvertTo<double>(void) const
{
  auto Tmp = ConvertTo<u64>();
  return *reinterpret_cast<double const*>(&Tmp);
}

MEDUSA_NAMESPACE_END