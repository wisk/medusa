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

  virtual bool Execute(Address const& rAddress, Expression& rExpr);
  virtual bool Execute(Address const& rAddress, Expression::List const& rExprList);

protected:

private:
  class InterpreterExpressionVisitor : public ExpressionVisitor
  {
  public:
    InterpreterExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
      : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt) {}

    virtual Expression* VisitSystem(SystemExpression* pSysExpr);
    virtual Expression* VisitBind(BindExpression* pBindExpr);
    virtual Expression* VisitTernaryCondition(TernaryConditionExpression* pTernExpr);
    virtual Expression* VisitIfElseCondition(IfElseConditionExpression* pIfElseExpr);
    virtual Expression* VisitWhileCondition(WhileConditionExpression* pWhileExpr);
    virtual Expression* VisitAssignment(AssignmentExpression* pAssignExpr);
    virtual Expression* VisitOperation(OperationExpression* pOpExpr);
    virtual Expression* VisitConstant(ConstantExpression* pConstExpr);
    virtual Expression* VisitIdentifier(IdentifierExpression* pIdExpr);
    virtual Expression* VisitTrackedIdentifier(TrackedIdentifierExpression* pTrkIdExpr);
    virtual Expression* VisitMemory(MemoryExpression* pMemExpr);
    virtual Expression* VisitSymbolic(SymbolicExpression* pSymExpr);

  protected:
    HookAddressHashMap const& m_rHooks;
    CpuContext*               m_pCpuCtxt;
    MemoryContext*            m_pMemCtxt;
  };
};

#endif // !_EMUL_INTERPRETER_