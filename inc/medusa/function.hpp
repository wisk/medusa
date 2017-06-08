#ifndef MEDUSA_FUNCTION_HPP
#define MEDUSA_FUNCTION_HPP

#include <list>

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"
#include "medusa/control_flow_graph.hpp"

MEDUSA_NAMESPACE_BEGIN

//! Function is a MultiCell which handles a function.
class MEDUSA_EXPORT Function : public MultiCell
{
public:
  typedef std::list<Function> List;

  Function(u16 Size, u16 InsnCnt);

  //! This method returns the number of instruction.
  u16 GetInstructionCount(void) const { return m_InsnCnt; }

private:
  u16 m_InsnCnt;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_FUNCTION_HPP