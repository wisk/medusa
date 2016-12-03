#include "llvm_compiler.hpp"

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

LlvmCompiler::LlvmCompiler(void)
  : m_Builder(llvm::getGlobalContext())
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
}

LlvmCompiler::~LlvmCompiler(void)
{
}

bool LlvmCompiler::Compile(std::string const& rFormatName, Path const& rOutputFile)
{
  return false;
}

bool LlvmCompiler::Compile(std::string const& rFormatName, void*& rpOutputBuffer, u32& rOutputSize)
{
  return false;
}