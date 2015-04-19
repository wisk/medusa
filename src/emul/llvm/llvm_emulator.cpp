#include "llvm_emulator.hpp"

#include "medusa/log.hpp"

#include <llvm/ExecutionEngine/MCJIT.h>

MEDUSA_NAMESPACE_USE

llvm::Module*           LlvmEmulator::sm_pModule          = nullptr;
llvm::ExecutionEngine*  LlvmEmulator::sm_pExecutionEngine = nullptr;
llvm::DataLayout*       LlvmEmulator::sm_pDataLayout      = nullptr;

static void* GetMemory(u8* pCpuCtxtObj, u8* pMemCtxtObj, TBase Base, TOffset Offset, u32 AccessSizeInBit)
{
  auto pCpuCtxt = reinterpret_cast<CpuContext*>(pCpuCtxtObj);
  auto pMemCtxt = reinterpret_cast<MemoryContext*>(pMemCtxtObj);
  void* pMemory;

  u64 LinAddr;
  if (pCpuCtxt->Translate(Address(Base, Offset), LinAddr) == false)
    LinAddr = Offset; // FIXME later

  if (pMemCtxt->FindMemory(LinAddr, pMemory, AccessSizeInBit) == false)
  {
    Log::Write("emul_llvm") << "Invalid memory access: linear address: " << LinAddr << LogEnd;
    Log::Write("emul_llvm") << pMemCtxt->ToString() << LogEnd;
    return nullptr;
  }
  return pMemory;
}

static llvm::Function* s_pGetMemoryFunc = nullptr;

LlvmEmulator::LlvmEmulator(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt)
  : Emulator(pCpuInfo, pCpuCtxt, pMemCtxt)
  , m_Builder(llvm::getGlobalContext())
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::LLVMContext& rCtxt = llvm::getGlobalContext();
  std::string ErrStr;

  if (sm_pModule == nullptr)
    sm_pModule = new llvm::Module("medusa-emulator-llvm", rCtxt);
  if (sm_pExecutionEngine == nullptr)
  {
    auto pChkStkType = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), false);
    llvm::IRBuilder<> ChkStkBld(llvm::getGlobalContext());
    auto pChkStk = llvm::Function::Create(pChkStkType, llvm::GlobalValue::ExternalLinkage, "__chkstk", sm_pModule);
    sm_pExecutionEngine->addGlobalMapping(pChkStk, (void*)0x1337);
  }
  if (sm_pExecutionEngine == nullptr)
    Log::Write("emul_llvm") << "Error: " << ErrStr << LogEnd;
  if (sm_pDataLayout == nullptr)
    sm_pDataLayout = new llvm::DataLayout(sm_pModule);


  if (s_pGetMemoryFunc == nullptr)
  {
    auto& rCtxt = llvm::getGlobalContext();
    std::vector<llvm::Type*> Params;
    Params.push_back(llvm::Type::getInt8PtrTy(rCtxt));
    Params.push_back(llvm::Type::getInt8PtrTy(rCtxt));
    Params.push_back(llvm::Type::getInt16Ty(rCtxt));
    Params.push_back(llvm::Type::getInt64Ty(rCtxt));
    Params.push_back(llvm::Type::getInt32Ty(rCtxt));
    auto pGetMemoryFuncType = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(rCtxt), Params, false);
    s_pGetMemoryFunc = llvm::Function::Create(pGetMemoryFuncType, llvm::GlobalValue::ExternalLinkage, "GetMemory", sm_pModule);

    sm_pExecutionEngine->addGlobalMapping(s_pGetMemoryFunc, (void*)GetMemory);
  }

  llvm::FunctionPassManager FuncPassMgr(sm_pModule);
  FuncPassMgr.add(llvm::createBasicAliasAnalysisPass());
  FuncPassMgr.add(llvm::createInstructionCombiningPass());
  FuncPassMgr.add(llvm::createReassociatePass());
  FuncPassMgr.add(llvm::createGVNPass());
  FuncPassMgr.add(llvm::createCFGSimplificationPass());
  FuncPassMgr.add(llvm::createPromoteMemoryToRegisterPass());
  FuncPassMgr.add(llvm::createDeadCodeEliminationPass());
  FuncPassMgr.doInitialization();
}

LlvmEmulator::~LlvmEmulator(void)
{
}

bool LlvmEmulator::Execute(Address const& rAddress, Expression::SPType spExpr)
{
  assert(0 && "Not implemented");
  return false;
}

bool LlvmEmulator::Execute(Address const& rAddress, Expression::LSPType const& rExprList)
{
  assert(0 && "Not implemented");
  return false;
}