#ifndef EMUL_LLVM_HPP
#define EMUL_LLVM_HPP

#include <medusa/emulation.hpp>

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

#include <llvm/IR/Intrinsics.h>


#if defined(_WIN32) || defined(WIN32)
# ifdef emul_llvm_EXPORTS
#  define EMUL_LLVM_EXPORT __declspec(dllexport)
# else
#  define EMUL_LLVM_EXPORT __declspec(dllimport)
# endif
#else
# define EMUL_LLVM_EXPORT
#endif

MEDUSA_NAMESPACE_USE

  extern "C" EMUL_LLVM_EXPORT Emulator* GetEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);

// TODO: overload VariableContext to use llvm Alloca
class LlvmEmulator : public medusa::Emulator
{
public:
  LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);
  ~LlvmEmulator() override;

  std::string GetName() const override { return "llvm"; }

  ReturnType Execute(Expression::VSPType const& rExprs) override;
  ReturnType Execute(Address const& rAddress) override;

  bool InvalidateCache() override;

private:
  using BasicBlockCode = bool (*)(u8* pCpuCtxtObj, u8* pMemCtxtObj);

  llvm::IRBuilder<>             m_Builder;

  using VarMapType = std::unordered_map<std::string, std::tuple<u32, llvm::Value*>>;
  VarMapType m_Vars;

  // TODO: Implement InvalidateCache to handle self-modifying code
  // TODO: Implement a method in CpuContext to get the current address (we can't always rely on CpuInformation::ProgramPointerRegister)
  using FunctionCacheType = std::unordered_map<u64, BasicBlockCode>;
  FunctionCacheType             m_FunctionCache;

  class LlvmJitHelper
  {
  public:
    LlvmJitHelper();
    ~LlvmJitHelper();

    llvm::Function* CreateFunction(std::string const& rFnName);
    BasicBlockCode GetFunctionCode(std::string const& rFnName);

  private:
    void _CreateModule(std::string const& rModName);

    std::vector<llvm::Module*> m_Modules;
    std::map<llvm::Module*, llvm::ExecutionEngine*> m_ModuleExecEngineMap;
    llvm::Module* m_pCurMod;

  } m_JitHelper;

//  class EventListener : public llvm::JITEventListener
//  {
//  public:
//    EventListener();
//    virtual ~EventListener();
//    virtual void NotifyObjectEmitted(llvm::object::ObjectFile const& rObj, llvm::RuntimeDyld::LoadedObjectInfo const& rLdObjInfo);
//    virtual void NotifyFreeingObject(llvm::object::ObjectFile const& rObj);
//  };

  class LlvmExpressionVisitor : public ExpressionVisitor
  {
  public:
    LlvmExpressionVisitor(
      Emulator* pEmul,
      HookAddressHashMap const& Hooks,
      CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VarMapType& rVars,
      llvm::IRBuilder<>& rBuilder,
      llvm::Value* pCpuCtxtObjParam, llvm::Value* pMemCtxtObjParam);

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
    llvm::Value* _EmitReadRegister(u32 Reg, CpuInformation const& rCpuInfo);
    bool         _EmitWriteRegister(u32 Reg, CpuInformation const& rCpuInfo, llvm::Value* pVal);
    void         _EmitReturn(llvm::Value* pCondVal, llvm::Value* pRetVal);
    void         _EmitReturn(llvm::Value* pCondVal, Emulator::ReturnType RetVal);
    void         _EmitReturnIfNull(llvm::Value* pChkVal, Emulator::ReturnType RetVal);
    llvm::Value* _EmitFloatingPointBinaryOperation(OperationExpression::Type FOpType, llvm::Value* pLeftVal, llvm::Value* pRightVal) const;

    Emulator*                 m_pEmul;
    HookAddressHashMap const& m_rHooks;
    CpuContext*               m_pCpuCtxt;
    MemoryContext*            m_pMemCtxt;
    LlvmEmulator::VarMapType& m_rVars;
    llvm::IRBuilder<>&        m_rBuilder;

    std::stack<llvm::Value*> m_ValueStack;
    llvm::Value*             m_pCpuCtxtObjParam;
    llvm::Value*             m_pMemCtxtObjParam;

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

#endif // !EMUL_LLVM_HPP
