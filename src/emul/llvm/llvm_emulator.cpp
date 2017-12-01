#include "llvm_emulator.hpp"

#include <medusa/log.hpp>
#include <medusa/expression_visitor.hpp>

#ifdef WIN32
# include "llvm/Support/Host.h"
#endif

#include <llvm/IR/Verifier.h>

#include <llvm/ExecutionEngine/MCJIT.h>

#include <llvm/Object/ObjectFile.h> // NOTE: Needed to avoid incomplete type llvm::object::ObjectFile

#include <llvm/Support/DynamicLibrary.h>

MEDUSA_NAMESPACE_USE

// This function allows the JIT'ed code to read from a register
extern "C" EMUL_LLVM_EXPORT bool JitReadRegister(CpuContext* pCpuCtxt, u32 Reg, void* pVal, u32 BitSize)
{
  if (!pCpuCtxt->ReadRegister(Reg, pVal, BitSize))
  {
    Log::Write("emul_llvm").Level(LogError) << "failed to read register " << pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Reg) << LogEnd;
    return false;
  }
  return true;
}

static llvm::Function* s_pReadRegisterFunc;

// This function allows the JIT'ed code to write into a register
extern "C" EMUL_LLVM_EXPORT bool JitWriteRegister(CpuContext* pCpuCtxt, u32 Reg, void const* pVal, u32 BitSize)
{
  if (!pCpuCtxt->WriteRegister(Reg, pVal, BitSize))
  {
    Log::Write("emul_llvm").Level(LogError) << "failed to write register " << pCpuCtxt->GetCpuInformation().ConvertIdentifierToName(Reg) << LogEnd;
    return false;
  }
  return true;
}

static llvm::Function* s_pWriteRegisterFunc;

// This function allows the JIT'ed code to convert logical address to linear address
extern "C" EMUL_LLVM_EXPORT u64 JitTranslateAddress(u8* pCpuCtxtObj, BaseType Base, OffsetType Offset)
{
  auto pCpuCtxt = reinterpret_cast<CpuContext*>(pCpuCtxtObj);
  u64 LinAddr;
  if (!pCpuCtxt->Translate(Address(Base, Offset), LinAddr))
    return Offset;

  return LinAddr;
}

static llvm::Function* s_pTranslateAddressFunc;

// This function allows the JIT'ed code to access to memory
extern "C" EMUL_LLVM_EXPORT void* JitGetMemory(u8* pCpuCtxtObj, u8* pMemCtxtObj, BaseType Base, OffsetType Offset, u32 BitSize)
{
  auto pCpuCtxt = reinterpret_cast<CpuContext*>(pCpuCtxtObj);
  auto pMemCtxt = reinterpret_cast<MemoryContext*>(pMemCtxtObj);
  void* pMemory;

  Address CurAddr(Base, Offset);
  u64 LinAddr;
  if (!pCpuCtxt->Translate(CurAddr, LinAddr))
  {
    Log::Write("emul_llvm").Level(LogWarning) << "unable to translate address " << CurAddr << LogEnd;
    return nullptr;
  }

  u32 MemOff;
  u32 MemSize;
  MemoryArea::Access Flags;
  if (!pMemCtxt->FindMemory(LinAddr, pMemory, MemOff, MemSize, Flags))
  {
    Log::Write("emul_llvm").Level(LogWarning) << "invalid memory access: linear address: " << LinAddr << LogEnd;
    return nullptr;
  }

  return reinterpret_cast<u8*>(pMemory) + MemOff;
}

static llvm::Function* s_pGetMemoryFunc = nullptr;

// This function allows the JIT'ed code to call instruction hook
extern "C" EMUL_LLVM_EXPORT Emulator::ReturnType JitCallInstructionHook(u8* pEmulObj)
{
  auto pEmul = reinterpret_cast<Emulator*>(pEmulObj);
  return pEmul->CallInstructionHook();
}

static llvm::Function* s_pCallInstructionHookFunc = nullptr;

// This function allows the JIT'ed code to handle hook
extern "C" EMUL_LLVM_EXPORT Emulator::ReturnType JitHandleHook(u8* pEmulObj, u8* pCpuCtxtObj, BaseType Base, OffsetType Offset, Emulator::HookType HkTy)
{
  auto pEmul = reinterpret_cast<Emulator*>(pEmulObj);
  auto pCpuCtxt = reinterpret_cast<CpuContext*>(pCpuCtxtObj);
  return pEmul->CallHookOnExecutionIfNeeded(Address(Base, Offset));
}

static llvm::Function* s_pHandleHookFunc = nullptr;

#ifdef _MSC_VER
extern "C" EMUL_LLVM_EXPORT __m128 __umodti3(__m128 a, __m128 b)
{
  u128 A, B;

  A = a.m128_u64[1];
  A <<= 64;
  A |= a.m128_u64[0];

  B = b.m128_u64[1];
  B <<= 64;
  B |= b.m128_u64[0];

  auto C = A % B;
  __m128 Result;
  Result.m128_u64[1] = static_cast<u64>(C >> 64);
  Result.m128_u64[0] = static_cast<u64>(C & 0xffffffffffffffffULL);

  return Result;
}

extern "C" EMUL_LLVM_EXPORT __m128 __udivti3(__m128 a, __m128 b)
{
  u128 A, B;

  A = a.m128_u64[1];
  A <<= 64;
  A |= a.m128_u64[0];

  B = b.m128_u64[1];
  B <<= 64;
  B |= b.m128_u64[0];

  auto C = A / B;
  __m128 Result;
  Result.m128_u64[1] = static_cast<u64>(C >> 64);
  Result.m128_u64[0] = static_cast<u64>(C & 0xffffffffffffffffULL);

  return Result;
}

#endif

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
  : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
  , m_Builder(llvm::getGlobalContext())
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
}

LlvmEmulator::~LlvmEmulator(void)
{
}

Emulator::ReturnType LlvmEmulator::Execute(Expression::VSPType const& rExprs)
{
  return Error;
}

Emulator::ReturnType LlvmEmulator::Execute(Address const& rAddress)
{
  Address ExecAddr = rAddress;
  if (!m_pCpuCtxt->SetAddress(CpuContext::AddressExecution, ExecAddr))
    return Error;
  u64 LinAddr;
  if (!m_pCpuCtxt->Translate(ExecAddr, LinAddr))
    return Error;

  // Check if the code to be executed is already JIT'ed
  auto pCode = m_FunctionCache[LinAddr];
  if (pCode == nullptr)
  {
    Expression::VSPType Exprs;
    Expression::LSPType JitExprs;

    // Disassemble code and retrieve semantic
    _Disassemble(ExecAddr, [&](Address const& rInsnAddr, Instruction& rCurInsn, Architecture& rCurArch, u8 CurMode)
    {
      // Check if we have a hook before updating the current address
      if (m_Hooks.find(rInsnAddr) != std::end(m_Hooks))
      {
        Exprs.push_back(Expr::MakeSys("jit_restore_ctxt", rInsnAddr));
        Exprs.push_back(Expr::MakeSys("check_exec_hook", rInsnAddr));
      }

      if (m_InsnCb)
      {
        Exprs.push_back(Expr::MakeSys("call_insn_cb", rInsnAddr));
      }

      // Set the current IP/PC address
      auto CurAddr = rCurArch.CurrentAddress(rInsnAddr, rCurInsn);
      if (!rCurArch.EmitSetExecutionAddress(Exprs, CurAddr, CurMode))
        return false;

      // Check for missing instruction semantic
      auto const& rCurInsnSem = rCurInsn.GetSemantic();
      if (rCurInsnSem.empty())
      {
        Log::Write("emul_llvm").Level(LogError) << "failed to find instruction semantic at " << rInsnAddr << LogEnd;
        Log::Write("emul_llvm").Level(LogError) << "details: " << rCurInsn.ToString() << LogEnd;
        Exprs.clear();
        return false;
      }

      for (auto spExpr : rCurInsnSem)
      {
        if (auto spSysExpr = expr_cast<SystemExpression>(spExpr))
        {
          auto const& rSysName = spSysExpr->GetName();
          // Don't bother to continue if a "stop" is asked
          if (rSysName == "stop")
          {
            Log::Write("emul_llvm").Level(LogWarning) << "stop asked at " << rInsnAddr << LogEnd;
            Exprs.clear();
            return false;
          }
        }

        Exprs.push_back(spExpr);
      }

      // Jump, Call, Return types finish the block
      if (rCurInsn.GetSubType() & (Instruction::JumpType | Instruction::CallType | Instruction::ReturnType))
        return false;

      return true;
    });

    if (Exprs.empty())
    {
      Log::Write("emul_llvm").Level(LogError) << "failed to disassemble code at " << ExecAddr << LogEnd;
      Exprs.clear();
      return Error;
    }

    // Instruction callback must be called. To do so, we have to disable JIT optimization
    if (m_InsnCb)
    {
      Log::Write("emul_llvm").Level(LogDebug) << "instruction hook detected, disable optimization" << LogEnd;

      for (auto spExpr : Exprs)
        JitExprs.push_back(spExpr);
    }

    // ...if not, we can freely make expressions more JIT friendly
    else
    {
      Log::Write("emul_llvm").Level(LogDebug) << "no instruction hook detected, enable optimization" << LogEnd;

      // This visitor will normalize id if needed
      NormalizeIdentifier NrmId(m_pCpuCtxt->GetCpuInformation(), m_pCpuCtxt->GetMode());

      // This visitor will replace id to variable
      IdentifierToVariable Id2Var;

      // Change the code to make it more JIT friendly
      for (auto spExpr : Exprs)
      {
        if (auto spSysExpr = expr_cast<SystemExpression>(spExpr))
        {
          auto const& rSysName = spSysExpr->GetName();
          // jit_restore_ctxt forces id in variable to be write back in context
          if (rSysName == "jit_restore_ctxt")
          {
            for (auto Id : Id2Var.GetUsedId())
            {
              auto const& rCpuInfo = m_pCpuCtxt->GetCpuInformation();
              auto IdName = rCpuInfo.ConvertIdentifierToName(Id);
              JitExprs.push_back(Expr::MakeAssign(
                Expr::MakeId(Id, &rCpuInfo),
                Expr::MakeVar(IdName, VariableExpression::Use)));
            }
            continue;
          }
        }

        // Normalize Id (e.g. al → eax in 32-bit, al → rax in 64-bit)
        auto spNrmExpr = spExpr->Visit(&NrmId);
        if (spNrmExpr == nullptr)
        {
          Log::Write("emul_llvm").Level(LogError) << "failed to normalize id with expression: " << spExpr->ToString() << LogEnd;
          Exprs.clear();
          return Error;
        }

        // Id to var
        auto spId2Var = spNrmExpr->Visit(&Id2Var);
        if (spId2Var == nullptr)
        {
          Log::Write("emul_llvm").Level(LogError) << "failed to convert id to var with expression: " << spNrmExpr->ToString() << LogEnd;
          Exprs.clear();
          return Error;
        }
        JitExprs.push_back(spId2Var);
      }

      // For each used id, we must allocate and free a variable
      auto const& rCpuInfo = m_pCpuCtxt->GetCpuInformation();
      for (auto Id : Id2Var.GetUsedId())
      {
        auto pIdName = rCpuInfo.ConvertIdentifierToName(Id);
        auto IdBitSize = rCpuInfo.GetSizeOfRegisterInBit(Id);
        if (pIdName == nullptr || IdBitSize == 0)
        {
          Log::Write("emul_llvm").Level(LogError) << "invalid id: " << Id << LogEnd;
          Exprs.clear();
          return Error;
        }

        // Copy id value to register (2)
        JitExprs.push_front(Expr::MakeAssign(
          Expr::MakeVar(pIdName, VariableExpression::Use),
          Expr::MakeId(Id, &rCpuInfo)));

        // Allocate variable (1)
        JitExprs.push_front(Expr::MakeVar(pIdName, VariableExpression::Alloc, IdBitSize));

        // Copy variable to id (3)
        JitExprs.push_back(Expr::MakeAssign(
          Expr::MakeId(Id, &rCpuInfo),
          Expr::MakeVar(pIdName, VariableExpression::Use)));

        // Free variable (4)
        JitExprs.push_back(Expr::MakeVar(pIdName, VariableExpression::Free));
      }
    }

    // Create a new LLVM function
    auto pExecFunc = m_JitHelper.CreateFunction(ExecAddr.ToString());

    // Expose its parameters: CpuCtxt and MemCtxt
    auto itParam = pExecFunc->arg_begin();
    auto pCpuCtxtObjParam = itParam++;
    auto pMemCtxtObjParam = itParam;

    // Insert a new basic block
    auto pBscBlk = llvm::BasicBlock::Create(llvm::getGlobalContext(), llvm::StringRef("entry_") + ExecAddr.ToString(), pExecFunc);
    m_Builder.SetInsertPoint(pBscBlk);

    // This visitor will now emit code into the basic block using the LLVM builder
    LlvmExpressionVisitor EmitLlvm(
      this,
      m_Hooks,
      m_pCpuCtxt, m_pMemCtxt,
      m_Vars,
      m_Builder,
      pCpuCtxtObjParam, pMemCtxtObjParam);

    // Emit the code for each expression
    for (auto spExpr : JitExprs)
    {
      //Log::Write("emul_llvm") << "compile expression: " << spExpr->ToString() << LogEnd;
      if (spExpr->Visit(&EmitLlvm) == nullptr)
      {
        Log::Write("emul_llvm").Level(LogError) << "failed to JIT expression at " << ExecAddr << LogEnd;
        Exprs.clear();
        return Error;
      }
    }

    m_Builder.CreateRet(llvm::ConstantInt::getIntegerValue(llvm::Type::getInt8Ty(llvm::getGlobalContext()), llvm::APInt(8, Emulator::Continue)));
    //pExecFunc->dump();
    pCode = m_JitHelper.GetFunctionCode(ExecAddr.ToString());
    if (pCode == nullptr)
    {
      Log::Write("emul_llvm").Level(LogError) << "failed to JIT code for function " << pExecFunc->getName().data() << LogEnd;
      Exprs.clear();
      return Error;
    }
    m_FunctionCache[LinAddr] = pCode;
  }

  if (!pCode(reinterpret_cast<u8*>(m_pCpuCtxt), reinterpret_cast<u8*>(m_pMemCtxt)))
    return Error;

  Address NextAddr;
  if (!m_pCpuCtxt->GetAddress(CpuContext::AddressExecution, NextAddr))
    return Error;
  NextAddr.SetBase(0x0); // HACK(KS)
  return CallHookOnExecutionIfNeeded(NextAddr);
}

bool LlvmEmulator::InvalidateCache(void)
{
  m_JitHelper = LlvmJitHelper();
  m_FunctionCache.clear();
  return true;
}

LlvmEmulator::LlvmJitHelper::LlvmJitHelper(void)
: m_pCurMod(nullptr)
{

}

LlvmEmulator::LlvmJitHelper::~LlvmJitHelper(void)
{
  for (auto& rModEE : m_ModuleExecEngineMap)
  {
    delete rModEE.second;
  }
}

llvm::Function* LlvmEmulator::LlvmJitHelper::CreateFunction(std::string const& rFnName)
{
  for (auto const* pMod : m_Modules)
  {
    auto pFunc = pMod->getFunction(std::string("fn_") + rFnName);
    if (pFunc != nullptr)
      return pFunc;
  }

  // Define method(CpuContext* pCpuCtxt, MemoryContext* pMemCtxt)
  static llvm::FunctionType* s_pExecFuncType = nullptr;
  if (s_pExecFuncType == nullptr)
  {
    auto pBytePtrType = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
    std::vector<llvm::Type*> Params;
    Params.push_back(pBytePtrType);
    Params.push_back(pBytePtrType);
    s_pExecFuncType = llvm::FunctionType::get(llvm::Type::getInt8Ty(llvm::getGlobalContext()), Params, false);
  }

  if (m_pCurMod == nullptr)
    _CreateModule(rFnName);
  auto pExecFunc = llvm::Function::Create(s_pExecFuncType, llvm::GlobalValue::InternalLinkage, llvm::StringRef("fn_") + rFnName, m_pCurMod);
  return pExecFunc;
}

LlvmEmulator::BasicBlockCode LlvmEmulator::LlvmJitHelper::GetFunctionCode(std::string const& rFnName)
{
  // We need a fresh module here
  if (m_pCurMod == nullptr)
    return nullptr;

  auto FPM = new llvm::legacy::FunctionPassManager(m_pCurMod);

#ifndef DEBUG
  FPM->add(llvm::createVerifierPass());
#endif
  // Set up the optimizer pipeline.  Start with registering info about how the
  // target lays out data structures.
  //FPM->add(new llvm::DataLayout(*EE->getDataLayout()));
  // Provide basic AliasAnalysis support for GVN.
  FPM->add(llvm::createBasicAliasAnalysisPass());
  // Promote allocas to registers.
  FPM->add(llvm::createPromoteMemoryToRegisterPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  FPM->add(llvm::createInstructionCombiningPass());
  // Reassociate expressions.
  FPM->add(llvm::createReassociatePass());
  // Eliminate Common SubExpressions.
  FPM->add(llvm::createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  FPM->add(llvm::createCFGSimplificationPass());


  FPM->doInitialization();

  for (auto& rFunc : *m_pCurMod)
    FPM->run(rFunc);

  auto EE = m_ModuleExecEngineMap[m_pCurMod];
  EE->finalizeObject();
  auto pFuncCode = (BasicBlockCode)EE->getFunctionAddress("fn_" + rFnName);
  m_pCurMod = nullptr;
  return pFuncCode;
}

void LlvmEmulator::LlvmJitHelper::_CreateModule(std::string const& rModName)
{
  m_pCurMod = new llvm::Module("medusa-llvm-emulator-module-" + rModName, llvm::getGlobalContext());

  // Create the engine builder
  std::string ErrStr;
  auto EE = llvm::EngineBuilder(std::unique_ptr<llvm::Module>(m_pCurMod)).setErrorStr(&ErrStr).create();
  m_ModuleExecEngineMap[m_pCurMod] = EE;

  auto& rCtxt = llvm::getGlobalContext();

  // Initialize ReadRegister function type
  {
    static std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),   // pCpuCtxt
      llvm::Type::getInt32Ty(rCtxt),    // Reg
      llvm::Type::getInt8PtrTy(rCtxt), // pVal
      llvm::Type::getInt32Ty(rCtxt)   // BitSize
    };
    static auto pReadRegisterFuncType = llvm::FunctionType::get(llvm::Type::getInt1Ty(rCtxt), Params, false);
    s_pReadRegisterFunc = llvm::Function::Create(pReadRegisterFuncType, llvm::GlobalValue::ExternalLinkage, "JitReadRegister", m_pCurMod);
    EE->addGlobalMapping(s_pReadRegisterFunc, (void*)JitReadRegister);
    llvm::sys::DynamicLibrary::AddSymbol("JitReadRegister", (void*)JitReadRegister);
  }

  // Initialize WriteRegister function type
  {
    static std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),   // pCpuCtxt
      llvm::Type::getInt32Ty(rCtxt),    // Reg
      llvm::Type::getInt8PtrTy(rCtxt), // pVal
      llvm::Type::getInt32Ty(rCtxt)   // BitSize
    };
    static auto pWriteRegisterFuncType = llvm::FunctionType::get(llvm::Type::getInt1Ty(rCtxt), Params, false);
    s_pWriteRegisterFunc = llvm::Function::Create(pWriteRegisterFuncType, llvm::GlobalValue::ExternalLinkage, "JitWriteRegister", m_pCurMod);
    EE->addGlobalMapping(s_pWriteRegisterFunc, (void*)JitWriteRegister);
    llvm::sys::DynamicLibrary::AddSymbol("JitWriteRegister", (void*)JitWriteRegister);
  }

  // Initialize TranslateMemory function type
  {
    static std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),    // pCpuCtxt
      llvm::Type::getInt16Ty(rCtxt),     // Base
      llvm::Type::getInt64Ty(rCtxt),    // Offset
    };
    static auto pTranslateAddressFuncType = llvm::FunctionType::get(llvm::Type::getInt64Ty(rCtxt), Params, false);
    s_pTranslateAddressFunc = llvm::Function::Create(pTranslateAddressFuncType, llvm::GlobalValue::ExternalLinkage, "JitTranslateAddress", m_pCurMod);
    EE->addGlobalMapping(s_pTranslateAddressFunc, (void*)JitTranslateAddress);
    llvm::sys::DynamicLibrary::AddSymbol("JitTranslateAddress", (void*)JitTranslateAddress);
  }

  // Initialize GetMemory function type
  {
    static std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),   // pCpuCtxt
      llvm::Type::getInt8PtrTy(rCtxt),  // pMemCtxt
      llvm::Type::getInt16Ty(rCtxt),   // Base
      llvm::Type::getInt64Ty(rCtxt),  // Offset
      llvm::Type::getInt32Ty(rCtxt), // AccessBitSize
    };
    static auto pGetMemoryFuncType = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(rCtxt), Params, false);
    s_pGetMemoryFunc = llvm::Function::Create(pGetMemoryFuncType, llvm::GlobalValue::ExternalLinkage, "JitGetMemory", m_pCurMod);
    EE->addGlobalMapping(s_pGetMemoryFunc, (void*)JitGetMemory);
    llvm::sys::DynamicLibrary::AddSymbol("JitGetMemory", (void*)JitGetMemory);
  }

  // Initialize CallInstructionHook function type
  {
    static std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt) // pEmul
    };
    static auto pCallInstructionHookType = llvm::FunctionType::get(llvm::Type::getInt8Ty(rCtxt), Params, false);
    s_pCallInstructionHookFunc = llvm::Function::Create(pCallInstructionHookType, llvm::GlobalValue::ExternalLinkage, "JitCallInstructionHook", m_pCurMod);
    EE->addGlobalMapping(s_pCallInstructionHookFunc, (void*)JitCallInstructionHook);
    llvm::sys::DynamicLibrary::AddSymbol("JitCallInstructionHook", (void*)JitCallInstructionHook);
  }

  // Initialize HandleHook function type
  {
    static std::vector<llvm::Type*> Params{
      llvm::Type::getInt8PtrTy(rCtxt),   // pEmul
      llvm::Type::getInt8PtrTy(rCtxt),  // pCpuCtxt
      llvm::Type::getInt16Ty(rCtxt),   // Base
      llvm::Type::getInt64Ty(rCtxt),  // Offset
      llvm::Type::getInt32Ty(rCtxt), // Type
    };
    static auto pHandleHookType = llvm::FunctionType::get(llvm::Type::getInt8Ty(rCtxt), Params, false);
    s_pHandleHookFunc = llvm::Function::Create(pHandleHookType, llvm::GlobalValue::ExternalLinkage, "JitHandleHook", m_pCurMod);
    EE->addGlobalMapping(s_pHandleHookFunc, (void*)JitHandleHook);
    llvm::sys::DynamicLibrary::AddSymbol("JitHandleHook", (void*)JitHandleHook);
  }

  m_Modules.push_back(m_pCurMod);
}

//LlvmEmulator::EventListener::EventListener(void)
//{
//}
//
//LlvmEmulator::EventListener::~EventListener(void)
//{
//}
//
//void LlvmEmulator::EventListener::NotifyObjectEmitted(llvm::object::ObjectFile const& rObj, llvm::RuntimeDyld::LoadedObjectInfo const& rLdObjInfo)
//{
//  Log::Write("emul_llvm").Level(LogDebug) << "object emitted" << LogEnd;
//}
//
//void LlvmEmulator::EventListener::NotifyFreeingObject(llvm::object::ObjectFile const& rObj)
//{
//  Log::Write("emul_llvm").Level(LogDebug) << "object freed" << LogEnd;
//}

LlvmEmulator::LlvmExpressionVisitor::LlvmExpressionVisitor(
  Emulator* pEmul,
  HookAddressHashMap const& rHooks,
  CpuContext* pCpuCtxt, MemoryContext* pMemCtxt, VarMapType& rVars,
  llvm::IRBuilder<>& rBuilder,
  llvm::Value* pCpuCtxtObjParam, llvm::Value* pMemCtxtObjParam)
  : m_pEmul(pEmul)
  , m_rHooks(rHooks)
  , m_pCpuCtxt(pCpuCtxt), m_pMemCtxt(pMemCtxt), m_rVars(rVars)
  , m_rBuilder(rBuilder)
  , m_NrOfValueToRead(), m_State(Unknown)
  , m_pCpuCtxtObjParam(pCpuCtxtObjParam), m_pMemCtxtObjParam(pMemCtxtObjParam)
{
}

LlvmEmulator::LlvmExpressionVisitor::~LlvmExpressionVisitor(void)
{
  while (!m_ValueStack.empty())
  {
    auto pCurVal = m_ValueStack.top();
    m_ValueStack.pop();
    Log::Write("emul_llvm") << "leaked value: " << pCurVal->getName().str() << LogEnd;
  }
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitSystem(SystemExpression::SPType spSysExpr)
{
  auto const& rSysName = spSysExpr->GetName();

  if (rSysName == "call_insn_cb")
  {
    auto pEmulRet = m_rBuilder.CreateCall(s_pCallInstructionHookFunc, _MakePointer(8, m_pEmul));

    _EmitReturn(m_rBuilder.CreateICmpNE(pEmulRet, _MakeInteger(BitVector(8, Emulator::Continue))),
      pEmulRet);

    return spSysExpr;
  }
  if (rSysName == "check_exec_hook")
  {
    Address CurAddr = spSysExpr->GetAddress();
    auto pEmulRet = m_rBuilder.CreateCall(
      s_pHandleHookFunc,
      {
        _MakePointer(8, m_pEmul), m_pCpuCtxtObjParam,
        _MakeInteger(BitVector(16, spSysExpr->GetAddress().GetBase())), _MakeInteger(BitVector(64, spSysExpr->GetAddress().GetOffset())),
        _MakeInteger(BitVector(32, Emulator::HookOnExecute))
      });

    _EmitReturn(m_rBuilder.CreateICmpNE(pEmulRet, _MakeInteger(BitVector(8, Emulator::Continue))),
      pEmulRet);

    return spSysExpr;
  }

  Log::Write("emul_llvm").Level(LogWarning) << "unhandled system expression: " << rSysName << LogEnd;
  return spSysExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitBind(BindExpression::SPType spBindExpr)
{
  for (auto spExpr : spBindExpr->GetBoundExpressions())
  {
    if (spExpr->Visit(this) == nullptr)
      return nullptr;
  }
  return spBindExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitTernaryCondition(TernaryConditionExpression::SPType spTernExpr)
{
  // _Emit test and ref
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spTernExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spTernExpr->GetTestExpression()->Visit(this);

  // Emit the condition
  auto pCondVal = _EmitComparison(spTernExpr->GetType(), "emit_tern_cmp");
  if (pCondVal == nullptr)
    return nullptr;

  auto spTrueExpr = spTernExpr->GetTrueExpression();
  if (spTrueExpr->Visit(this) == nullptr)
    return nullptr;
  if (m_ValueStack.size() < 1)
    return nullptr;
  auto pTrueVal = m_ValueStack.top();
  m_ValueStack.pop();

  auto spFalseExpr = spTernExpr->GetFalseExpression();
  if (spFalseExpr->Visit(this) == nullptr)
    return nullptr;
  if (m_ValueStack.size() < 1)
    return nullptr;
  auto pFalseVal = m_ValueStack.top();
  m_ValueStack.pop();

  m_State = OldState;

  auto pTernVal = m_rBuilder.CreateSelect(pCondVal, pTrueVal, pFalseVal, "tern");
  m_ValueStack.push(pTernVal);
  return spTernExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitIfElseCondition(IfElseConditionExpression::SPType spIfElseExpr)
{
  auto pBbOrig = m_rBuilder.GetInsertBlock();
  auto& rCtxt  = llvm::getGlobalContext();
  auto pFunc   = pBbOrig->getParent();

  auto pBbCond = llvm::BasicBlock::Create(rCtxt, "cond", pFunc);
  auto pBbThen = llvm::BasicBlock::Create(rCtxt, "then", pFunc);
  auto pBbElse = llvm::BasicBlock::Create(rCtxt, "else", pFunc);
  auto pBbMerg = llvm::BasicBlock::Create(rCtxt, "merg", pFunc);

  m_rBuilder.CreateBr(pBbCond);

  // _Emit test and ref
  m_rBuilder.SetInsertPoint(pBbCond);

  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spIfElseExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spIfElseExpr->GetTestExpression()->Visit(this);

  // Emit the condition
  auto pCondVal = _EmitComparison(spIfElseExpr->GetType(), "emit_if_else_cmp");
  if (pCondVal == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateCondBr(pCondVal, pBbThen, pBbElse);

  // Emit the then branch
  m_rBuilder.SetInsertPoint(pBbThen);

  auto spThenExpr = spIfElseExpr->GetThenExpression();
  OldState = m_State;
  m_State = Unknown;
  if (spThenExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbMerg);

  // Emit the else branch
  m_rBuilder.SetInsertPoint(pBbElse);

  auto spElseExpr = spIfElseExpr->GetElseExpression();
  OldState = m_State;
  m_State = Unknown;
  if (spElseExpr != nullptr)
    if (spElseExpr->Visit(this) == nullptr)
      return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbMerg);

  m_rBuilder.SetInsertPoint(pBbMerg);
  return spIfElseExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitWhileCondition(WhileConditionExpression::SPType spWhileExpr)
{
  auto pBbOrig = m_rBuilder.GetInsertBlock();
  auto& rCtxt = llvm::getGlobalContext();
  auto pFunc = pBbOrig->getParent();

  auto pBbCond = llvm::BasicBlock::Create(rCtxt, "cond", pFunc);
  auto pBbBody = llvm::BasicBlock::Create(rCtxt, "body", pFunc);
  auto pBbNext = llvm::BasicBlock::Create(rCtxt, "next", pFunc);

  m_rBuilder.CreateBr(pBbCond);

  m_rBuilder.SetInsertPoint(pBbCond);

  // _Emit test and ref
  State OldState = m_State;
  m_State = Read;
  auto spRefExpr = spWhileExpr->GetReferenceExpression()->Visit(this);
  auto spTestExpr = spWhileExpr->GetTestExpression()->Visit(this);
  auto pCondVal = _EmitComparison(spWhileExpr->GetType(), "emit_while_cmp");
  if (pCondVal == nullptr)
    return nullptr;
  m_State = OldState;

  // Emit the condition
  m_rBuilder.CreateCondBr(pCondVal, pBbBody, pBbNext);

  // Emit the body branch
  auto spBodyExpr = spWhileExpr->GetBodyExpression();
  m_rBuilder.SetInsertPoint(pBbBody);
  OldState = m_State;
  m_State = Unknown;
  if (spBodyExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;
  m_rBuilder.CreateBr(pBbCond);

  m_rBuilder.SetInsertPoint(pBbNext);

  return spWhileExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitAssignment(AssignmentExpression::SPType spAssignExpr)
{
  if (auto spDstVecId = expr_cast<VectorIdentifierExpression>(spAssignExpr->GetDestinationExpression()))
    m_NrOfValueToRead = spDstVecId->GetVector().size();
  else
    m_NrOfValueToRead = 0;

  State OldState = m_State;

  m_State = Read;
  auto spSrc = spAssignExpr->GetSourceExpression()->Visit(this);
  m_State = Write;
  auto spDst = spAssignExpr->GetDestinationExpression()->Visit(this);
  m_State = OldState;

  if (spDst == nullptr || spSrc == nullptr)
    return nullptr;

  return spAssignExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitUnaryOperation(UnaryOperationExpression::SPType spUnOpExpr)
{
  auto spExpr = spUnOpExpr->GetExpression()->Visit(this);

  if (spExpr == nullptr)
    return nullptr;

  if (m_ValueStack.size() < 1)
    return nullptr;
  auto pVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pUnOpVal = nullptr;

  switch (spUnOpExpr->GetOperation())
  {
  default:
    Log::Write("emul_llvm") << "unhandled unary operation for " << spUnOpExpr->ToString() << LogEnd;
    return nullptr;

    case OperationExpression::OpNot:
      pUnOpVal = m_rBuilder.CreateNot(pVal);
      break;

    case OperationExpression::OpNeg:
      pUnOpVal = m_rBuilder.CreateNeg(pVal);
      break;

    case OperationExpression::OpSwap:
      pUnOpVal = _CallIntrinsic(llvm::Intrinsic::bswap, { pVal->getType() }, { pVal });
      break;

    case OperationExpression::OpBsf:
      pUnOpVal = _CallIntrinsic(llvm::Intrinsic::cttz,
        { pVal->getType() },
        { pVal, _MakeInteger(BitVector(1, 1)) });
      break;

    case OperationExpression::OpBsr:
    {
      auto pCtlz = _CallIntrinsic(llvm::Intrinsic::ctlz,
        { pVal->getType() },
        { pVal, _MakeInteger(BitVector(1, 1)) });
      // NOTE(wisk): LLVM operation ctlz gives the result from the beginning (in x86 it emits bsr op0, op1 ; xor op0, op1.bitsize - 1)
      // we have to reverse the last xor to avoid invalid result
      auto ValBitSize = pVal->getType()->getScalarSizeInBits();
      pUnOpVal = m_rBuilder.CreateXor(pCtlz, _MakeInteger(BitVector(ValBitSize, ValBitSize - 1)));
    }
    break;
  }

  if (pUnOpVal == nullptr)
    return nullptr;
  m_ValueStack.push(pUnOpVal);
  return spUnOpExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitBinaryOperation(BinaryOperationExpression::SPType spBinOpExpr)
{
  auto spLeft = spBinOpExpr->GetLeftExpression()->Visit(this);
  auto spRight = spBinOpExpr->GetRightExpression()->Visit(this);

  if (spLeft == nullptr || spRight == nullptr)
    return nullptr;

  if (m_ValueStack.size() < 2)
    return nullptr;

  auto RightVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto LeftVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pBinOpVal = nullptr;
  auto BinOp = spBinOpExpr->GetOperation();

  switch (BinOp)
  {
  default:
    Log::Write("emul_llvm") << "unhandled binary operation for " << spBinOpExpr->ToString() << LogEnd;
    return nullptr;

  case OperationExpression::OpAnd:
    pBinOpVal = m_rBuilder.CreateAnd(LeftVal, RightVal, "and");
    break;

  case OperationExpression::OpOr:
    pBinOpVal = m_rBuilder.CreateOr(LeftVal, RightVal, "or");
    break;

  case OperationExpression::OpXor:
    pBinOpVal = m_rBuilder.CreateXor(LeftVal, RightVal, "xor");
    break;

  case OperationExpression::OpLls:
    pBinOpVal = m_rBuilder.CreateShl(LeftVal, RightVal, "lls");
    break;

  case OperationExpression::OpLrs:
    pBinOpVal = m_rBuilder.CreateLShr(LeftVal, RightVal, "lrs");
    break;

  case OperationExpression::OpArs:
    pBinOpVal = m_rBuilder.CreateAShr(LeftVal, RightVal, "ars");
    break;

  case OperationExpression::OpRol:
  {
    auto pLeftType = LeftVal->getType();
    auto LeftBits  = pLeftType->getScalarSizeInBits();
    if (LeftBits == 0)
      break;

    auto pCnt    = m_rBuilder.CreateURem(RightVal, _MakeInteger(BitVector(LeftBits, LeftBits)));
    auto pRol0   = m_rBuilder.CreateShl(LeftVal, pCnt, "rol_0");
    auto pRolSub = m_rBuilder.CreateSub(_MakeInteger(BitVector(LeftBits, LeftBits)), pCnt, "rol_sub");
    auto pRol1   = m_rBuilder.CreateLShr(LeftVal, pRolSub, "rol_1");
    pBinOpVal    = m_rBuilder.CreateOr(pRol0, pRol1, "rol");
  }
  break;

  case OperationExpression::OpRor:
  {
    auto pLeftType = LeftVal->getType();
    auto LeftBits  = pLeftType->getScalarSizeInBits();
    if (LeftBits == 0)
      break;

    auto pCnt    = m_rBuilder.CreateURem(RightVal, _MakeInteger(BitVector(LeftBits, LeftBits)));
    auto pRor0   = m_rBuilder.CreateLShr(LeftVal, pCnt, "ror_0");
    auto pRorSub = m_rBuilder.CreateSub(_MakeInteger(BitVector(LeftBits, LeftBits)), pCnt, "rol_sub");
    auto pRor1   = m_rBuilder.CreateShl(LeftVal, pRorSub, "ror_1");
    pBinOpVal    = m_rBuilder.CreateOr(pRor0, pRor1, "ror");
  }
  break;

  case OperationExpression::OpAdd:
    pBinOpVal = m_rBuilder.CreateAdd(LeftVal, RightVal, "add");
    break;

  case OperationExpression::OpSub:
    pBinOpVal = m_rBuilder.CreateSub(LeftVal, RightVal, "sub");
    break;

  case OperationExpression::OpMul:
    pBinOpVal = m_rBuilder.CreateMul(LeftVal, RightVal, "mul");
    break;

  case OperationExpression::OpSDiv:
    pBinOpVal = m_rBuilder.CreateSDiv(LeftVal, RightVal, "sdiv");
    break;

  case OperationExpression::OpUDiv:
    pBinOpVal = m_rBuilder.CreateUDiv(LeftVal, RightVal, "udiv");
    break;

  case OperationExpression::OpSMod:
    pBinOpVal = m_rBuilder.CreateSRem(LeftVal, RightVal, "smod");
    break;

  case OperationExpression::OpUMod:
  {
    pBinOpVal = m_rBuilder.CreateURem(LeftVal, RightVal, "umod");
    break;
  }

  case OperationExpression::OpSext:
    pBinOpVal = m_rBuilder.CreateSExt(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "sext");
    break;

  case OperationExpression::OpZext:
    pBinOpVal = m_rBuilder.CreateZExt(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "zext");
    break;

  case OperationExpression::OpBcast:
    pBinOpVal = m_rBuilder.CreateZExtOrTrunc(LeftVal, _BitSizeToLlvmType(spRight->GetBitSize()), "bcast");
    break;

  case OperationExpression::OpInsertBits:
  {
    auto spRConst = expr_cast<BitVectorExpression>(spRight);
    if (spRConst == nullptr)
      break;
    auto pShift = _MakeInteger(spRConst->GetInt().Lsb());
    auto pMask = _MakeInteger(spRConst->GetInt());
    auto pShiftedVal = m_rBuilder.CreateShl(LeftVal, pShift, "insert_bits0");
    pBinOpVal = m_rBuilder.CreateAnd(pShiftedVal, pMask, "insert_bits1");
    break;
  }

  case OperationExpression::OpExtractBits:
  {
    auto spRConst = expr_cast<BitVectorExpression>(spRight);
    if (spRConst == nullptr)
      break;
    auto pShift = _MakeInteger(spRConst->GetInt().Lsb());
    auto pMask = _MakeInteger(spRConst->GetInt());
    auto pMaskedVal = m_rBuilder.CreateAnd(LeftVal, pMask, "extract_bits0");
    pBinOpVal = m_rBuilder.CreateLShr(pMaskedVal, pShift, "extract_bits1");
    break;
  }

  case OperationExpression::OpClearBits:
  {
    pBinOpVal = m_rBuilder.CreateAnd(LeftVal, m_rBuilder.CreateNot(RightVal), "clear_bits");
    break;
  }

  case OperationExpression::OpFAdd:
  case OperationExpression::OpFSub:
  case OperationExpression::OpFMul:
  case OperationExpression::OpFDiv:
  case OperationExpression::OpFMod:
    pBinOpVal = _EmitFloatingPointBinaryOperation(static_cast<OperationExpression::Type>(BinOp), LeftVal, RightVal);
    break;
  }

  if (pBinOpVal == nullptr)
  {
    Log::Write("emul_llvm").Level(LogError) << "failed to emit expression: " << spBinOpExpr->ToString() << LogEnd;
    return nullptr;
  }

  m_ValueStack.push(pBinOpVal);
  return spBinOpExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitBitVector(BitVectorExpression::SPType spConstExpr)
{
  if (m_State != Read)
  {
    Log::Write("emul_llvm").Level(LogError) << "constant can only be read" << LogEnd;
    return nullptr;
  }
  auto pConstVal = _MakeInteger(spConstExpr->GetInt());
  if (pConstVal == nullptr)
    return nullptr;
  m_ValueStack.push(pConstVal);
  return spConstExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitIdentifier(IdentifierExpression::SPType spIdExpr)
{
  switch (m_State)
  {
  case Read:
  {
    auto pRegVal = _EmitReadRegister(spIdExpr->GetId(), *spIdExpr->GetCpuInformation());
    if (pRegVal == nullptr)
      return nullptr;
    m_ValueStack.push(pRegVal);
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
      return nullptr;
    auto pRegVal = m_ValueStack.top();
    m_ValueStack.pop();
    if (!_EmitWriteRegister(spIdExpr->GetId(), *spIdExpr->GetCpuInformation(), pRegVal))
      return nullptr;
    break;
  }

  default:
    return nullptr;
  }
  return spIdExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitVectorIdentifier(VectorIdentifierExpression::SPType spVecIdExpr)
{
  auto const* pCpuInfo = spVecIdExpr->GetCpuInformation();
  switch (m_State)
  {
  case Read:
  {
    auto VecId = spVecIdExpr->GetVector();
    for (auto Id : VecId)
    {
      auto pRegVal = _EmitReadRegister(Id, *spVecIdExpr->GetCpuInformation());
      if (pRegVal == nullptr)
        return nullptr;
      m_ValueStack.push(pRegVal);
    }
    break;
  }

  case Write:
  {
    auto VecId = spVecIdExpr->GetVector();
    for (auto Id : VecId)
    {
      if (m_ValueStack.empty())
        return nullptr;
      auto pRegVal = m_ValueStack.top();
      m_ValueStack.pop();
      if (!_EmitWriteRegister(Id, *spVecIdExpr->GetCpuInformation(), pRegVal))
        return nullptr;
    }
    break;
  }

  default:
    return nullptr;
  }

  return spVecIdExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitTrack(TrackExpression::SPType spTrkExpr)
{
  return spTrkExpr->GetTrackedExpression()->Visit(this);
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitVariable(VariableExpression::SPType spVarExpr)
{
  switch (m_State)
  {
  case Unknown:
  {
    switch (spVarExpr->GetType())
    {
    case VariableExpression::Alloc:
      m_rVars[spVarExpr->GetName()] = std::make_tuple(spVarExpr->GetBitSize(), m_rBuilder.CreateAlloca(_BitSizeToLlvmType(spVarExpr->GetBitSize()), nullptr, spVarExpr->GetName()));
      break;

    case VariableExpression::Free:
      m_rVars.erase(spVarExpr->GetName());
      break;

    default:
      Log::Write("emul_llvm").Level(LogError) << "unknown variable action" << LogEnd;
      return nullptr;
    }
    break;
  }

  case Read:
    if (spVarExpr->GetType() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      spVarExpr->SetBitSize(std::get<0>(itVar->second));
      m_ValueStack.push(m_rBuilder.CreateLoad(std::get<1>(itVar->second), "read_var"));
      break;
    }
    else
    {
      Log::Write("emul_llvm").Level(LogError) << "invalid state for variable reading" << LogEnd;
      return nullptr;
    }

  case Write:
    if (spVarExpr->GetType() == VariableExpression::Use)
    {
      auto itVar = m_rVars.find(spVarExpr->GetName());
      if (itVar == std::end(m_rVars))
        return nullptr;
      spVarExpr->SetBitSize(std::get<0>(itVar->second));
      if (m_ValueStack.empty())
        return nullptr;
      auto pVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateStore(pVal, std::get<1>(itVar->second));
      break;
    }
    else
    {
      Log::Write("emul_llvm").Level(LogError) << "invalid state for variable writing" << LogEnd;
      return nullptr;
    }

  default:
    return nullptr;
  }

  return spVarExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitMemory(MemoryExpression::SPType spMemExpr)
{
  State OldState = m_State;
  m_State = Read;

  auto spOffsetExpr = spMemExpr->GetOffsetExpression();
  auto spBaseExpr = spMemExpr->GetBaseExpression();

  if (spBaseExpr == nullptr)
    m_ValueStack.push(_MakeInteger(BitVector(16, 0x0)));
  else if (spBaseExpr->Visit(this) == nullptr)
    return nullptr;

  if (spOffsetExpr->Visit(this) == nullptr)
    return nullptr;
  m_State = OldState;

  if (m_ValueStack.size() < 2)
  {
    Log::Write("emul_llvm").Level(LogError) << "no value for address" << LogEnd;
    return nullptr;
  }

  auto pOffVal = m_rBuilder.CreateZExtOrBitCast(m_ValueStack.top(), _BitSizeToLlvmType(64));
  m_ValueStack.pop();
  auto pBaseVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto AccBitSize = spMemExpr->GetAccessSizeInBit();
  auto pAccBitSizeVal = _MakeInteger(BitVector(32, AccBitSize));

  llvm::Value* pPtrVal = nullptr;
  if (m_State != Read || spMemExpr->IsDereferencable())
  {
    auto pRawMemVal = m_rBuilder.CreateCall(
      s_pGetMemoryFunc,
      {
        m_pCpuCtxtObjParam,
        m_pMemCtxtObjParam,
        pBaseVal, pOffVal,
        pAccBitSizeVal
      }, "get_memory");
    pPtrVal = m_rBuilder.CreateBitCast(pRawMemVal, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), AccBitSize));
    _EmitReturnIfNull(pPtrVal, Emulator::Error);
  }

  switch (m_State)
  {
  default:
    Log::Write("emul_llvm").Level(LogError) << "unknown state for address" << LogEnd;
    return nullptr;

  case Read:
  {
    if (!spMemExpr->IsDereferencable())
    {
      auto pTransAddrRes = m_rBuilder.CreateCall(
        s_pTranslateAddressFunc, 
        {
          m_pCpuCtxtObjParam,
          pBaseVal, pOffVal
        },
        "translate_memory");
      auto pTransAddrResCast = m_rBuilder.CreateTruncOrBitCast(pTransAddrRes, _BitSizeToLlvmType(AccBitSize));
      m_ValueStack.push(pTransAddrResCast);
      break;
    }

    if (m_NrOfValueToRead == 0)
    {
      auto pVal = m_rBuilder.CreateLoad(pPtrVal, "read_mem");
      m_ValueStack.push(pVal);
    }
    while (m_NrOfValueToRead != 0)
    {
      auto pVal = m_rBuilder.CreateLoad(pPtrVal, "read_mem");
      m_ValueStack.push(pVal);
      // FIXME(KS):
      //m_rBuilder.CreateAdd(pPtrVal, _MakeInteger(BitVector(AccBitSize, AccBitSize / 8)), "inc_ptr");
      --m_NrOfValueToRead;
    }
    break;
  }

  case Write:
  {
    if (m_ValueStack.empty())
    {
      Log::Write("emul_llvm").Level(LogError) << "no value for address writing" << LogEnd;
      return nullptr;
    }

    //  // NOTE: Trying to write an non-deferencable address is like
    //  // changing its offset.
    //  if (!spMemExpr->IsDereferencable())
    //  {
    //    auto spOffsetExpr = spMemExpr->GetOffsetExpression()->Visit(this);
    //    if (spOffsetExpr == nullptr)
    //      return nullptr;
    //    break;
    //  }

    do
    {
      auto pCurVal = m_ValueStack.top();
      m_ValueStack.pop();
      m_rBuilder.CreateStore(pCurVal, pPtrVal);
      // FIXME(KS):
      //m_rBuilder.CreateAdd(pPtrVal, _MakeInteger(BitVector(AccBitSize, AccBitSize / 8)), "inc_ptr");
    } while (!m_ValueStack.empty());
    break;
  }
  }

  return spMemExpr;
}

Expression::SPType LlvmEmulator::LlvmExpressionVisitor::VisitSymbolic(SymbolicExpression::SPType spSymExpr)
{
  return nullptr;
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_MakeInteger(BitVector const& rInt) const
{
  // TODO(KS): Handle integer larger than 64-bit
  if (rInt.GetBitSize() > 64)
  {
    // Skip "0x"
    return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(rInt.GetBitSize(), rInt.ToString().c_str() + 2, 16));
    Log::Write("emul_llvm").Level(LogError) << "unsupported int size " << rInt.GetBitSize() << LogEnd;
    return nullptr;
  }
  return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(rInt.GetBitSize(), rInt.ConvertTo<u64>()));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_MakePointer(u32 Bits, void* pPointer, s32 Offset) const
{
  //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
  auto pConstInt = llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), reinterpret_cast<u64>(pPointer));
  auto pPtr = llvm::ConstantExpr::getIntToPtr(pConstInt, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));

  if (Offset == 0x0)
    return pPtr;

  return m_rBuilder.CreateGEP(pPtr, _MakeInteger(BitVector(Offset)));
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_MakePointer(u32 Bits, llvm::Value* pPointerValue, s32 Offset) const
{
  if (Offset != 0x0)
  {
    //src: http://llvm.1065342.n5.nabble.com/Creating-Pointer-Constants-td31886.html
    pPointerValue = m_rBuilder.CreateGEP(pPointerValue, _MakeInteger(BitVector(Offset)));
  }

  return m_rBuilder.CreateBitCast(pPointerValue, llvm::PointerType::getIntNPtrTy(llvm::getGlobalContext(), Bits));
}

llvm::Type* LlvmEmulator::LlvmExpressionVisitor::_BitVectorToLlvmType(BitVector const& rInt) const
{
  return _BitSizeToLlvmType(rInt.GetBitSize());
}

llvm::Type* LlvmEmulator::LlvmExpressionVisitor::_BitSizeToLlvmType(u16 BitSize) const
{
  return llvm::Type::getIntNTy(llvm::getGlobalContext(), BitSize);
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_CallIntrinsic(
  llvm::Intrinsic::ID IntrId,
  std::vector<llvm::Type*> const& rTypes,
  std::vector<llvm::Value*> const& rArgs) const
{
  auto pModule   = m_rBuilder.GetInsertBlock()->getParent()->getParent();
  auto pIntrFunc = llvm::Intrinsic::getDeclaration(pModule, IntrId, rTypes);
  auto pCallIntr = m_rBuilder.CreateCall(pIntrFunc, rArgs);
  return pCallIntr;
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_EmitComparison(u8 CondOp, char const* pCmpName)
{
  if (m_ValueStack.size() < 2)
  {
    Log::Write("emul_llvm").Level(LogError) << "no enough values to do comparison" << LogEnd;
    return nullptr;
  }

  auto RefVal = m_ValueStack.top();
  m_ValueStack.pop();
  auto TestVal = m_ValueStack.top();
  m_ValueStack.pop();

  llvm::Value* pCmpVal = nullptr;

  switch (CondOp)
  {
  case ConditionExpression::CondEq:
    pCmpVal = m_rBuilder.CreateICmpEQ(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondNe:
    pCmpVal = m_rBuilder.CreateICmpNE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUgt:
    pCmpVal = m_rBuilder.CreateICmpUGT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUge:
    pCmpVal = m_rBuilder.CreateICmpUGE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUlt:
    pCmpVal = m_rBuilder.CreateICmpULT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondUle:
    pCmpVal = m_rBuilder.CreateICmpULE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSgt:
    pCmpVal = m_rBuilder.CreateICmpSGT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSge:
    pCmpVal = m_rBuilder.CreateICmpSGE(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSlt:
    pCmpVal = m_rBuilder.CreateICmpSLT(TestVal, RefVal, pCmpName);
    break;

  case ConditionExpression::CondSle:
    pCmpVal = m_rBuilder.CreateICmpSLE(TestVal, RefVal, pCmpName);
    break;

  default:
    Log::Write("emul_llvm") << "unknown comparison" << LogEnd;
    return nullptr;
  }

  return pCmpVal;
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_EmitReadRegister(u32 Reg, CpuInformation const& rCpuInfo)
{
  auto RegName = llvm::StringRef(rCpuInfo.ConvertIdentifierToName(Reg));

  // We need the register bit size to declare its type
  auto RegBitSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);

  // Make values for function parameters
  auto pRegVal = _MakeInteger(BitVector(32, Reg));
  auto pBitSize = _MakeInteger(BitVector(32, RegBitSize));

  // Allocate the result on the stack
  auto pRegAlloca = m_rBuilder.CreateAlloca(_BitSizeToLlvmType(RegBitSize), nullptr, RegName + "_alloc");
  auto pRegPtrVal = m_rBuilder.CreateBitCast(pRegAlloca, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), 8));

  // Call ReadRegister wrapper
  auto pCallVal = m_rBuilder.CreateCall(
    s_pReadRegisterFunc,
    {
      m_pCpuCtxtObjParam,
      pRegVal,
      pRegPtrVal,
      pBitSize
    }, RegName + "_read");

  // Return the result
  auto pRegResVal = m_rBuilder.CreateLoad(pRegAlloca, RegName + "_val");
  return pRegResVal;
}

bool LlvmEmulator::LlvmExpressionVisitor::_EmitWriteRegister(u32 Reg, CpuInformation const& rCpuInfo, llvm::Value* pVal)
{
  auto RegName = llvm::StringRef(rCpuInfo.ConvertIdentifierToName(Reg));

  // We need the register bit size to declare its type
  auto RegBitSize = rCpuInfo.GetSizeOfRegisterInBit(Reg);

  // Make values for function parameters
  auto pRegVal = _MakeInteger(BitVector(32, Reg));
  auto pBitSize = _MakeInteger(BitVector(32, RegBitSize));

  // Allocate the new value on the stack
  auto pRegAlloca = m_rBuilder.CreateAlloca(_BitSizeToLlvmType(RegBitSize), nullptr, RegName + "_alloc");
  auto pStoreReg = m_rBuilder.CreateStore(pVal, pRegAlloca);
  auto pRegPtrVal = m_rBuilder.CreateBitCast(pRegAlloca, llvm::Type::getIntNPtrTy(llvm::getGlobalContext(), 8), RegName + "_ptr");

  // Call WriteRegister wrapper
  auto pCallVal = m_rBuilder.CreateCall(
    s_pWriteRegisterFunc,
    {
      m_pCpuCtxtObjParam,
      pRegVal,
      pRegPtrVal,
      pBitSize
    },
    RegName + "_write");

  return true;
}

void LlvmEmulator::LlvmExpressionVisitor::_EmitReturn(llvm::Value* pCondVal, llvm::Value* pRetVal)
{
  auto pBbOrig = m_rBuilder.GetInsertBlock();
  auto& rCtxt = llvm::getGlobalContext();
  auto pFunc = pBbOrig->getParent();

  auto pBbRet = llvm::BasicBlock::Create(rCtxt, "ret", pFunc);
  auto pBbCont = llvm::BasicBlock::Create(rCtxt, "continue", pFunc);

  m_rBuilder.CreateCondBr(pCondVal, pBbRet, pBbCont);

  m_rBuilder.SetInsertPoint(pBbRet);
  m_rBuilder.CreateRet(pRetVal);

  m_rBuilder.SetInsertPoint(pBbCont);
}

void LlvmEmulator::LlvmExpressionVisitor::_EmitReturn(llvm::Value* pCondVal, Emulator::ReturnType RetVal)
{
  _EmitReturn(pCondVal, _MakeInteger(BitVector(8, RetVal)));
}

void LlvmEmulator::LlvmExpressionVisitor::_EmitReturnIfNull(llvm::Value* pChkVal, Emulator::ReturnType RetVal)
{
  auto& rCtxt = llvm::getGlobalContext();

  auto pChkValInt = m_rBuilder.CreatePtrToInt(pChkVal, llvm::Type::getIntNTy(rCtxt, sizeof(void*) * 8));
  auto pNullPtrInt = _MakeInteger(BitVector(sizeof(void*) * 8, 0));
  auto pCmpPtr = m_rBuilder.CreateICmpEQ(pChkValInt, pNullPtrInt, "ret_if_null_cmp");
  _EmitReturn(pCmpPtr, RetVal);
}

llvm::Value* LlvmEmulator::LlvmExpressionVisitor::_EmitFloatingPointBinaryOperation(OperationExpression::Type FOpType, llvm::Value* pLeftVal, llvm::Value* pRightVal) const
{
  auto pLeftType = pLeftVal->getType();
  auto LeftBits = pLeftType->getScalarSizeInBits();
  auto pRightType = pRightVal->getType();
  auto RightBits = pRightType->getScalarSizeInBits();

  if (LeftBits != RightBits)
  {
    Log::Write("emul_llvm").Level(LogError) << "mismatch type for floating point operations" << LogEnd;
    return nullptr;
  }

  llvm::Type* pFloatType = nullptr;
  switch (LeftBits)
  {
  case 32:
    pFloatType = llvm::Type::getFloatTy(llvm::getGlobalContext());
    break;

  case 64:
    pFloatType = llvm::Type::getDoubleTy(llvm::getGlobalContext());
    break;

  default:
    Log::Write("emul_llvm").Level(LogError) << "unhandle floating point size: " << LeftBits << LogEnd;
    return nullptr;
  }

  auto pLeftFloat  = m_rBuilder.CreateCast(llvm::Instruction::BitCast, pLeftVal, pFloatType, "leftval_to_flp");
  auto pRightFloat = m_rBuilder.CreateCast(llvm::Instruction::BitCast, pRightVal, pFloatType, "rightval_to_flp");

  llvm::Value* pFloatOpVal = nullptr;
  switch (FOpType)
  {
  case OperationExpression::OpFAdd: pFloatOpVal = m_rBuilder.CreateFAdd(pLeftFloat, pRightFloat, "fadd"); break;
  case OperationExpression::OpFSub: pFloatOpVal = m_rBuilder.CreateFSub(pLeftFloat, pRightFloat, "fsub"); break;
  case OperationExpression::OpFMul: pFloatOpVal = m_rBuilder.CreateFMul(pLeftFloat, pRightFloat, "fmul"); break;
  case OperationExpression::OpFDiv: pFloatOpVal = m_rBuilder.CreateFDiv(pLeftFloat, pRightFloat, "fdiv"); break;
  case OperationExpression::OpFMod: pFloatOpVal = m_rBuilder.CreateFRem(pLeftFloat, pRightFloat, "fmod"); break;

  default:
    Log::Write("emul_llvm").Level(LogError) << "unhandle floating point operation" << LogEnd;
    return nullptr;
  }

  return m_rBuilder.CreateCast(llvm::Instruction::BitCast, pFloatOpVal, pLeftType, "res_to_bv");
}