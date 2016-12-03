#include "llvm_compiler.hpp"

Compiler* GetCompiler(void)
{
  return new LlvmCompiler;
}

int main(void) { return 0; }