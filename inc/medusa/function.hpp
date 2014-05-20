#ifndef _MEDUSA_FUNCTION_
#define _MEDUSA_FUNCTION_

#include <list>

#include "medusa/namespace.hpp"
#include "medusa/multicell.hpp"
#include "medusa/control_flow_graph.hpp"

MEDUSA_NAMESPACE_BEGIN

//! Function is a MultiCell which handles a function.
class Function : public MultiCell
{
public:
  typedef std::list<Function> List;

  Function(Id FunctionId, u16 Size, u16 InsnCnt);
  Function(std::string Name, u16 Size, u16 InsnCnt);

  //! This method returns the number of instruction.
  u16 GetInstructionCounter(void) const { return m_InsnCnt; }
  u8 GetParameterCounter(void) const { return m_ParamCnt; }

  void SetParameterCounter(u8 ParamCnt) { m_ParamCnt = ParamCnt; }

  virtual bool DisplayCell(void) const { return true; }

private:
  u16              m_InsnCnt;
  u8               m_ParamCnt;
};

MEDUSA_NAMESPACE_END

#endif // !_MEDUSA_FUNCTION_