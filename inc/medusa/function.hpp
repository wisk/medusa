#ifndef MEDUSA_FUNCTION_HPP
#define MEDUSA_FUNCTION_HPP

#include <list>

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"

MEDUSA_NAMESPACE_BEGIN

//! Function is a MultiCell which handles a function.
class MEDUSA_EXPORT Function : public MultiCell
{
public:
  using List = std::list<Function>;

  Function(u16 Size, u16 InsnCnt);

  //! This method returns the number of instruction.
  u16 GetInstructionCount() const { return m_InsnCnt; }

private:
  u16 m_InsnCnt;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_FUNCTION_HPP