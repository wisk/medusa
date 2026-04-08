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

extern "C" EMUL_INTERPRETER_EXPORT Emulator* GetEmulator(void);

class InterpreterEmulator : public Emulator
{
public:
  InterpreterEmulator(void);
  ~InterpreterEmulator(void) override;

  std::string GetName(void) const override { return "interpreter"; }
  ReturnType Execute(Expression::VSPType const& rExprs) override;

protected:
  std::unordered_map<std::string, BitVector> m_Vars;

private:
  class InterpreterExpressionVisitor : public ExpressionVisitor
  {
  public:
    InterpreterExpressionVisitor(HookAddressHashMap const& Hooks, CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, std::unordered_map<std::string, BitVector>& rVars)
      : m_rHooks(Hooks), m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_rVars(rVars), m_NrOfValueToRead(), m_State(Unknown) {}

    ~InterpreterExpressionVisitor(void) override;

    Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr) override;
    Expression::SPType VisitBind(BindExpression::SPType spBindExpr) override;
    Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr) override;
    Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr) override;
    Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr) override;
    Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr) override;
    Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr) override;
    Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr) override;
    Expression::SPType VisitBitVector(BitVectorExpression::SPType spConstExpr) override;
    Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr) override;
    Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr) override;
    Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr) override;
    Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr) override;
    Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr) override;
    Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr) override;

  protected:
    HookAddressHashMap const& m_rHooks;
    CpuContext*               m_pCpuCtxt;
    MemoryContext*            m_pMemCtxt;

  private:
    bool _EvaluateComparison(u8 CondOp, bool& rRes);

    std::unordered_map<std::string, BitVector>& m_rVars;
    Expression::DataContainerType m_Values;
    size_t m_NrOfValueToRead;

    enum State
    {
      Unknown,
      Read,
      Write,
    };

    State m_State;
  };
};

#endif // !EMUL_INTERPRETER_HPP
