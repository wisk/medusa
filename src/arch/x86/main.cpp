#include "x86_architecture.hpp"

int main(void) { return 0; }

Architecture* GetArchitecture(void)
{
  return new X86Architecture;
}
