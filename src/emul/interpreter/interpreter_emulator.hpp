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

  virtual bool Execute(Address const& rAddress, Expression::SPtr spExpr);
  virtual bool Execute(Address const& rAddress, Expression::List const& rExprList);

protected:

private:
  class InterpreterExpressionVisitor : public ExpressionVisitor
  {
  public:
    InterpreterExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
      : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt) {}

    virtual Expression::SPtr VisitSystem(SystemExpression::SPtr spSysExpr);
    virtual Expression::SPtr VisitBind(BindExpression::SPtr spBindExpr);
    virtual Expression::SPtr VisitTernaryCondition(TernaryConditionExpression::SPtr spTernExpr);
    virtual Expression::SPtr VisitIfElseCondition(IfElseConditionExpression::SPtr spIfElseExpr);
    virtual Expression::SPtr VisitWhileCondition(WhileConditionExpression::SPtr spWhileExpr);
    virtual Expression::SPtr VisitAssignment(AssignmentExpression::SPtr spAssignExpr);
    virtual Expression::SPtr VisitOperation(OperationExpression::SPtr spOpExpr);
    virtual Expression::SPtr VisitConstant(ConstantExpression::SPtr spConstExpr);
    virtual Expression::SPtr VisitIdentifier(IdentifierExpression::SPtr spIdExpr);
    virtual Expression::SPtr VisitTrackedIdentifier(TrackedIdentifierExpression::SPtr spTrkIdExpr);
    virtual Expression::SPtr VisitMemory(MemoryExpression::SPtr spMemExpr);
    virtual Expression::SPtr VisitSymbolic(SymbolicExpression::SPtr spSymExpr);

  protected:
    HookAddressHashMap const& m_rHooks;
    CpuContext*               m_pCpuCtxt;
    MemoryContext*            m_pMemCtxt;
  };
};

#endif // !_EMUL_INTERPRETER_