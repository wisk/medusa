#ifndef _EMUL_INTERPRETER_
#define _EMUL_INTERPRETER_

#include <medusa/emulation.hpp>

#if defined(_WIN32) || defined(WIN32)
# ifdef emul_interpreter_EXPORTS
#  define EMUL_INTERPRETER_EXPORT __declspec(dllexport)
# else
#  define EMUL_INTERPRETER_EXPORT __declspec(dllimport)
# endif
#else
# define EMUL_INTERPRETER_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" EMUL_INTERPRETER_EXPORT Emulator* GetEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);

class InterpreterEmulator : public Emulator
{
public:
           InterpreterEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);
  virtual ~InterpreterEmulator(void);

  virtual std::string GetName(void) const { return "interpreter"; }

  virtual void ReadRegister (u32 Register, void*       pValue, u32 ValueSize) const;
  virtual void WriteRegister(u32 Register, void const* pValue, u32 ValueSize);

  virtual void Execute(Expression const& rExpr);
  virtual void Execute(Expression::List const& rExprList);

protected:

private:
  class InterpreterExpressionVisitor : public ExpressionVisitor
  {
  public:
    InterpreterExpressionVisitor(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt) : m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt) {}
    virtual Expression* VisitBind(Expression::List const& rExprList);
    virtual Expression* VisitCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr);
    virtual Expression* VisitIfCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr);
    virtual Expression* VisitIfElseCondition(u32 Type, Expression const* pRefExpr, Expression const* pTestExpr, Expression const* pThenExpr, Expression const* pElseExpr);
    virtual Expression* VisitOperation(u32 Type, Expression const* pLeftExpr, Expression const* pRightExpr);
    virtual Expression* VisitConstant(u32 Type, u64 Value);
    virtual Expression* VisitIdentifier(u32 Id, CpuInformation const* pCpuInfo);
    virtual Expression* VisitMemory(u32 AccessSizeInBit, Expression const* pBaseExpr, Expression const* pOffsetExpr, bool Deref);

  protected:
    CpuContext* m_pCpuCtxt;
    MemoryContext* m_pMemCtxt;
  };
};

#endif // !_EMUL_INTERPRETER_