#ifndef EMUL_INTERPRETER_HPP
#define EMUL_INTERPRETER_HPP

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

  virtual bool Execute(Address const& rAddress, Expression::SPType spExpr);
  virtual bool Execute(Address const& rAddress, Expression::LSPType  const& rExprList);

protected:

private:
  class InterpreterExpressionVisitor : public ExpressionVisitor
  {
  public:
    InterpreterExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
      : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt) {}

    virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
    virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
    virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
    virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
    virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
    virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
    virtual Expression::SPType VisitOperation(OperationExpression::SPType spOpExpr);
    virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr);
    virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
    virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
    virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);
    virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
    virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  protected:
    HookAddressHashMap const& m_rHooks;
    CpuContext*               m_pCpuCtxt;
    MemoryContext*            m_pMemCtxt;

  private:
    bool _EvaluateCondition(ConditionExpression::SPType spCondExpr, bool& rResult);
    template<typename _Type>
    Expression::SPType _DoOperation(u8 Op, Expression::SPType spLeftExpr, Expression::SPType spRightExpr);
  };
};

#endif // !EMUL_INTERPRETER_HPP