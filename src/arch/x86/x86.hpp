#ifndef __X86_HPP__
#define __X86_HPP__

#include <medusa/types.hpp>

namespace x86
{
  using medusa::u8;

  class ModRM
  {
  public:
    ModRM(u8 ModRm) : m_ModRm(ModRm) {                          };
    u8 Mod(void)                     { return m_ModRm >> 6;     };
    u8 Reg(void)                     { return m_ModRm >> 3 & 7; };
    u8 Rm(void)                      { return m_ModRm & 7;      };

  private:
    u8 m_ModRm;
  };

  class Sib
  {
  public:
    Sib(u8 Sib) : m_Sib(Sib) {                        };
    u8 Scale(void)           { return m_Sib >> 6;     };
    u8 Index(void)           { return m_Sib >> 3 & 7; };
    u8 Base(void)            { return m_Sib & 7;      };

  private:
    u8 m_Sib;
  };
}

#endif // !__X86_HPP__
