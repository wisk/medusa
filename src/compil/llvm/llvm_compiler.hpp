#ifndef EMUL_LLVM_HPP
#define EMUL_LLVM_HPP

#include <medusa/compilation.hpp>

#include <stack>
#include <tuple>

#include <llvm/Support/TargetSelect.h>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/DataLayout.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Target/TargetMachine.h>

#include <llvm/CodeGen/VirtRegMap.h>

#include <llvm/IR/Intrinsics.h>


#if defined(_WIN32) || defined(WIN32)
# ifdef compil_llvm_EXPORTS
#  define COMPIL_LLVM_EXPORT __declspec(dllexport)
# else
#  define COMPIL_LLVM_EXPORT __declspec(dllimport)
# endif
#else
# define COMPIL_LLVM_EXPORT
#endif

MEDUSA_NAMESPACE_USE

extern "C" COMPIL_LLVM_EXPORT Compiler* GetCompiler(void);

class LlvmCompiler : public Compiler
{
public:
  LlvmCompiler();
  ~LlvmCompiler() override;

  std::string GetName() const override { return "llvm"; }

  bool Compile(std::vector<u8>& rBuffer) override;

private:
  llvm::IRBuilder<> m_Builder;

  using VarMapType = std::unordered_map<std::string, std::tuple<u32, llvm::Value*>>;
  VarMapType m_Vars;

  class LlvmExpressionVisitor : public ExpressionVisitor
  {
  public:
    LlvmExpressionVisitor(
      Compiler* pCompil,
      VarMapType& rVars,
      llvm::IRBuilder<>& rBuilder);

    ~LlvmExpressionVisitor() override;

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
    llvm::Value* _MakeInteger(BitVector const& rInt) const;
    llvm::Value* _MakePointer(u32 Bits, void* pPointer, s32 Offset = 0) const;
    llvm::Value* _MakePointer(u32 Bits, llvm::Value* pPointerValue, s32 Offset = 0) const;

    llvm::Type*  _BitVectorToLlvmType(BitVector const& rInt) const;
    llvm::Type*  _BitSizeToLlvmType(u16 BitSize) const;

    llvm::Value* _CallIntrinsic(llvm::Intrinsic::ID IntrId, std::vector<llvm::Type*> const& rTypes, std::vector<llvm::Value*> const& rArgs) const;

    llvm::Value* _EmitComparison(u8 CondOp, char const* pCmpName);
    llvm::Value* _EmitFloatingPointBinaryOperation(OperationExpression::Type FOpType, llvm::Value* pLeftVal, llvm::Value* pRightVal) const;

    Compiler*                 m_pCompil;
    LlvmCompiler::VarMapType& m_rVars;
    llvm::IRBuilder<>&        m_rBuilder;

    std::stack<llvm::Value*> m_ValueStack;

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

#endif // !COMPIL_LLVM_HPP
