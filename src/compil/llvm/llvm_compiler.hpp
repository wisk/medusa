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
  LlvmCompiler(void);
  virtual ~LlvmCompiler(void);

  virtual std::string GetName(void) const { return "llvm"; }

  virtual bool Compile(std::vector<u8>& rBuffer);

private:
  llvm::IRBuilder<> m_Builder;

  typedef std::unordered_map<std::string, std::tuple<u32, llvm::Value*>> VarMapType;
  VarMapType m_Vars;

  class LlvmExpressionVisitor : public ExpressionVisitor
  {
  public:
    LlvmExpressionVisitor(
      Compiler* pCompil,
      VarMapType& rVars,
      llvm::IRBuilder<>& rBuilder);

    virtual ~LlvmExpressionVisitor(void);

    virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
    virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
    virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
    virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
    virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
    virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
    virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr);
    virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr);
    virtual Expression::SPType VisitBitVector(BitVectorExpression::SPType spConstExpr);
    virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
    virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
    virtual Expression::SPType VisitTrack(TrackExpression::SPType spTrkExpr);
    virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
    virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
    virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

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
