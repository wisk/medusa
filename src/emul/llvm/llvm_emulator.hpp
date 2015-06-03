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
#include <llvm/PassManager.h>
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
  virtual ~LlvmEmulator(void);

  virtual std::string GetName(void) const { return "llvm"; }

  virtual bool Execute(Expression::VSPType const& rExprs);
  virtual bool Execute(Address const& rAddress);

private:
  typedef bool (*BasicBlockCode)(u8* pCpuCtxtObj, u8* pMemCtxtObj);

  llvm::IRBuilder<>             m_Builder;

  typedef std::unordered_map<std::string, std::tuple<u32, llvm::Value*>> VarMapType;
  VarMapType m_Vars;

  // TODO: Implement InvalidCache to handle self-modifying code
  // TODO: Implement a method in CpuContext to get the current address (we can't always rely on CpuInformation::ProgramPointerRegister)
  typedef std::unordered_map<u64, BasicBlockCode>   FunctionCacheType;
  FunctionCacheType             m_FunctionCache;

  class LlvmJitHelper
  {
  public:
    LlvmJitHelper(void);
    ~LlvmJitHelper(void);

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
//    EventListener(void);
//    virtual ~EventListener(void);
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

    virtual ~LlvmExpressionVisitor(void);

    virtual Expression::SPType VisitSystem(SystemExpression::SPType spSysExpr);
    virtual Expression::SPType VisitBind(BindExpression::SPType spBindExpr);
    virtual Expression::SPType VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr);
    virtual Expression::SPType VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr);
    virtual Expression::SPType VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr);
    virtual Expression::SPType VisitAssignment(AssignmentExpression::SPType spAssignExpr);
    virtual Expression::SPType VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr);
    virtual Expression::SPType VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr);
    virtual Expression::SPType VisitConstant(ConstantExpression::SPType spConstExpr);
    virtual Expression::SPType VisitIdentifier(IdentifierExpression::SPType spIdExpr);
    virtual Expression::SPType VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr);
    virtual Expression::SPType VisitTrackedIdentifier(TrackedIdentifierExpression::SPType spTrkIdExpr);
    virtual Expression::SPType VisitVariable(VariableExpression::SPType spVarExpr);
    virtual Expression::SPType VisitMemory(MemoryExpression::SPType spMemExpr);
    virtual Expression::SPType VisitSymbolic(SymbolicExpression::SPType spSymExpr);

  protected:
    llvm::Value* _MakeInteger(IntType const& rInt) const;
    llvm::Value* _MakePointer(u32 Bits, void* pPointer, s32 Offset = 0) const;
    llvm::Value* _MakePointer(u32 Bits, llvm::Value* pPointerValue, s32 Offset = 0) const;

    llvm::Type*  _IntTypeToLlvmType(IntType const& rInt) const;
    llvm::Type*  _BitSizeToLlvmType(u16 BitSize) const;

    llvm::Value* _CallIntrinsic(llvm::Intrinsic::ID IntrId, std::vector<llvm::Type*> const& rTypes, std::vector<llvm::Value*> const& rArgs) const;

    llvm::Value* _EmitComparison(u8 CondOp);
    llvm::Value* _EmitReadRegister(u32 Reg, CpuInformation const& rCpuInfo);
    bool         _EmitWriteRegister(u32 Reg, CpuInformation const& rCpuInfo, llvm::Value* pVal);
    void         _EmitReturnIfNull(llvm::Value* pChkVal, llvm::Value* pRetVal);

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
