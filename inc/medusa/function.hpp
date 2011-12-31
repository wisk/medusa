#ifndef _MEDUSA_FUNCTION_
#define _MEDUSA_FUNCTION_

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"

MEDUSA_NAMESPACE_BEGIN

//! Function is a MultiCell which handles a function.
class Function : public MultiCell
{
public:
  Function(u16 Size, u16 InsnCnt)
    : MultiCell(MultiCell::FunctionType)
    , m_InsnCnt(InsnCnt)
  {}

  //! This method returns the number of instruction.
  u16 GetInstructionCounter(void) const { return m_InsnCnt; }

private:
  u16 m_InsnCnt;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_FUNCTION_
