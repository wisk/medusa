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

extern "C" COMPIL_LLVM_EXPORT Compiler* GetCompiler(CpuInformation const* pCpuInfo, CpuContext* pCpuCtxt, MemoryContext *pMemCtxt);

class LlvmCompiler : public Compiler
{
public:
  LlvmCompiler(void);
  virtual ~LlvmCompiler(void);

  virtual std::string GetName(void) const { return "llvm"; }

  virtual bool Compile(std::string const& rFormatName, Path const& rOutputFile);
  virtual bool Compile(std::string const& rFormatName, void*& rpOutputBuffer, u32& rOutputSize);

private:
  llvm::IRBuilder<> m_Builder;

  typedef std::unordered_map<std::string, std::tuple<u32, llvm::Value*>> VarMapType;
  VarMapType m_Vars;

};

#endif // !COMPIL_LLVM_HPP
