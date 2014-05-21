#include "medusa/value.hpp"

MEDUSA_NAMESPACE_BEGIN

void Value::Modify(u8& rValue) const
{
  switch (GetSubType() & ModifierMask)
  {
  case NotType:    rValue = ~rValue; break;
  case NegateType: rValue = -static_cast<s8>(rValue); break;
  default: break;
  }
}

void Value::Modify(u16& rValue) const
{
  switch (GetSubType() & ModifierMask)
  {
  case NotType:    rValue = ~rValue; break;
  case NegateType: rValue = -static_cast<s16>(rValue); break;
  default: break;
  }
}

void Value::Modify(u32& rValue) const
{
  switch (GetSubType() & ModifierMask)
  {
  case NotType:    rValue = ~rValue; break;
  case NegateType: rValue = -static_cast<s32>(rValue); break;
  default: break;
  }
}

void Value::Modify(u64& rValue) const
{
  switch (GetSubType() & ModifierMask)
  {
  case NotType:    rValue = ~rValue; break;
  case NegateType: rValue = -static_cast<s64>(rValue); break;
  default: break;
  }
}

MEDUSA_NAMESPACE_END