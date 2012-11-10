#include "arm_architecture.hpp"

int main(void) { return 0; }

Architecture* GetArchitecture(void)
{
  return new ArmArchitecture;
}