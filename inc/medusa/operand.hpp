#ifndef MEDUSA_OPERAND_HPP
#define MEDUSA_OPERAND_HPP

#include <string>

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/expression.hpp"
#include "medusa/information.hpp"

MEDUSA_NAMESPACE_BEGIN

/* Operand
    8  LSB contains information about size
    24 MSB contains information about operand itself
*/
inline constexpr u64 O_NONE       = 0x0000000000000000;  // Unused

inline constexpr u64 O_MASK       = 0x00000000fff00000;
inline constexpr u64 O_REG        = 0x0000000000100000;  // Register
inline constexpr u64 O_IMM        = 0x0000000000200000;  // Immediate e.g. 0x10000
inline constexpr u64 O_DISP       = 0x0000000000400000;  // Displacement e.g. xxx + 0x11223344, [xxx + 0x100000]
inline constexpr u64 O_REL        = 0x0000000000800000;  // Relative e.g. +0x10
inline constexpr u64 O_ABS        = 0x0000000001000000;  // Absolute e.g. 0x10000

inline constexpr u64 O_SCALE      = 0x0000000002000000;  // Scale e.g. [xxx * 4] or LSL #xxx

inline constexpr u64 O_MEM        = 0x0000000004000000;  // Memory e.g. [0x100000] or [Reg]
inline constexpr u64 O_SREG       = 0x0000000008000000;  // Second register e.g. [reg + reg]
inline constexpr u64 O_SEG        = 0x0000000010000000;  // Segment e.g. seg:[xxx]
inline constexpr u64 O_SEG_VAL    = 0x0000000020000000;  // Segment value e.g. xxxx:yyyy

inline constexpr u64 O_NO_REF     = 0x0000000040000000;  // Operand can't hold an address
inline constexpr u64 O_REG_PC_REL = 0x0000000080000000;  // Register operand holds the current address
inline constexpr u64 O_WRITEBACK  = 0x0000000100000000;  // Write back

// Register Size
inline constexpr u64 RS_MASK      = 0x000000000000000f;
inline constexpr u64 RS_1BIT      = 0x0000000000000001;
inline constexpr u64 RS_8BIT      = 0x0000000000000002;
inline constexpr u64 RS_16BIT     = 0x0000000000000003;
inline constexpr u64 RS_32BIT     = 0x0000000000000004;
inline constexpr u64 RS_64BIT     = 0x0000000000000005;
inline constexpr u64 RS_80BIT     = 0x0000000000000006;
inline constexpr u64 RS_128BIT    = 0x0000000000000007;

// Data Size (IMM / DISP / REL)
inline constexpr u64 DS_MASK      = 0x00000000000000f0;
inline constexpr u64 DS_UNK       = 0x0000000000000010;
inline constexpr u64 DS_8BIT      = 0x0000000000000020;
inline constexpr u64 DS_16BIT     = 0x0000000000000030;
inline constexpr u64 DS_32BIT     = 0x0000000000000040;
inline constexpr u64 DS_64BIT     = 0x0000000000000050;
inline constexpr u64 DS_128BIT    = 0x0000000000000060;

// SCale
inline constexpr u64 SC_MASK      = 0x0000000000000f00;
inline constexpr u64 SC_1         = 0x0000000000000100;
inline constexpr u64 SC_2         = 0x0000000000000200;
inline constexpr u64 SC_4         = 0x0000000000000400;
inline constexpr u64 SC_8         = 0x0000000000000800;

// Memory Size (dereferencement)
inline constexpr u64 MS_MASK      = 0x000000000000f000;
inline constexpr u64 MS_8BIT      = 0x0000000000001000;
inline constexpr u64 MS_16BIT     = 0x0000000000002000;
inline constexpr u64 MS_32BIT     = 0x0000000000003000;
inline constexpr u64 MS_64BIT     = 0x0000000000004000;
inline constexpr u64 MS_80BIT     = 0x0000000000005000;
inline constexpr u64 MS_128BIT    = 0x0000000000006000;

// Address Size
inline constexpr u64 AS_MASK      = 0x00000000000f0000;
inline constexpr u64 AS_8BIT      = 0x0000000000010000;
inline constexpr u64 AS_16BIT     = 0x0000000000020000;
inline constexpr u64 AS_32BIT     = 0x0000000000030000;
inline constexpr u64 AS_64BIT     = 0x0000000000040000;

// Alias
inline constexpr u64 O_REG1       = O_REG   | RS_1BIT;
inline constexpr u64 O_REG8       = O_REG   | RS_8BIT;
inline constexpr u64 O_REG16      = O_REG   | RS_16BIT;
inline constexpr u64 O_REG32      = O_REG   | RS_32BIT;
inline constexpr u64 O_REG64      = O_REG   | RS_64BIT;
inline constexpr u64 O_REG80      = O_REG   | RS_80BIT;
inline constexpr u64 O_REG128     = O_REG   | RS_128BIT;
inline constexpr u64 O_IMM8       = O_IMM   | DS_8BIT;
inline constexpr u64 O_IMM16      = O_IMM   | DS_16BIT;
inline constexpr u64 O_IMM32      = O_IMM   | DS_32BIT;
inline constexpr u64 O_IMM64      = O_IMM   | DS_64BIT;
inline constexpr u64 O_DISP8      = O_DISP  | DS_8BIT;
inline constexpr u64 O_DISP16     = O_DISP  | DS_16BIT;
inline constexpr u64 O_DISP32     = O_DISP  | DS_32BIT;
inline constexpr u64 O_DISP64     = O_DISP  | DS_64BIT;
inline constexpr u64 O_REL8       = O_REL   | DS_8BIT;
inline constexpr u64 O_REL16      = O_REL   | DS_16BIT;
inline constexpr u64 O_REL32      = O_REL   | DS_32BIT;
inline constexpr u64 O_REL64      = O_REL   | DS_64BIT;
inline constexpr u64 O_ABS8       = O_ABS   | DS_8BIT;
inline constexpr u64 O_ABS16      = O_ABS   | DS_16BIT;
inline constexpr u64 O_ABS32      = O_ABS   | DS_32BIT;
inline constexpr u64 O_ABS64      = O_ABS   | DS_64BIT;
inline constexpr u64 O_SCALE1     = O_SCALE | SC_1;
inline constexpr u64 O_SCALE2     = O_SCALE | SC_2;
inline constexpr u64 O_SCALE4     = O_SCALE | SC_4;
inline constexpr u64 O_SCALE8     = O_SCALE | SC_8;
inline constexpr u64 O_MEM8       = O_MEM   | MS_8BIT;
inline constexpr u64 O_MEM16      = O_MEM   | MS_16BIT;
inline constexpr u64 O_MEM32      = O_MEM   | MS_32BIT;
inline constexpr u64 O_MEM64      = O_MEM   | MS_64BIT;
inline constexpr u64 O_MEM80      = O_MEM   | MS_80BIT;
inline constexpr u64 O_MEM128     = O_MEM   | MS_128BIT;
inline constexpr u64 O_ADDR8      = O_MEM   | AS_8BIT;
inline constexpr u64 O_ADDR16     = O_MEM   | AS_16BIT;
inline constexpr u64 O_ADDR32     = O_MEM   | AS_32BIT;
inline constexpr u64 O_ADDR64     = O_MEM   | AS_64BIT;

//! Operand is a generic class which contains information about an instruction operand.
class MEDUSA_EXPORT Operand
{
public:
  Operand()
  : m_Type(O_NONE)
  , m_Name("")
  , m_Offset()
  , m_Reg()
  , m_SecReg()
  , m_Seg()
  , m_Value()
  , m_SegValue()
{
}
  ~Operand() {}

  u64&        Type()                         { return m_Type;            }
  u16&        Reg()                          { return m_Reg;             }
  u16&        SecReg()                       { return m_SecReg;          }
  u16&        Seg()                          { return m_Seg;             }
  u64&        Value()                        { return m_Value;           }
  u16&        SegValue()                     { return m_SegValue;        }

  u8          GetOffset()   const            { return m_Offset;          }
  u64         GetType()     const            { return m_Type;            }
  u16         GetReg()      const            { return m_Reg;             }
  u16         GetSecReg()   const            { return m_SecReg;          }
  u16         GetSeg()      const            { return m_Seg;             }
  u64         GetValue()    const
  {
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:           return m_Value & 0xff;
    case DS_16BIT:          return m_Value & 0xffff;
    case DS_32BIT:          return m_Value & 0xffffffff;
    case DS_64BIT: default: return m_Value;
    }
  }
  u16         GetSegValue() const            { return m_SegValue;        }

  u8          GetLength() const;
  u8          GetRawLength() const;
  u32         GetBitSize() const;

  Expression::SPType GetSemantic(u8 Mode, CpuInformation const* pCpuInfo, u8 InstructionLength = 0, bool Dereference = true) const;

  void        SetType(u64 Type)                  { m_Type     = Type;        }
  void        SetOffset(u8 Offset)               { m_Offset   = Offset;      }
  void        SetReg(u16 Reg)                    { m_Reg      = Reg;         }
  void        SetSecReg(u16 SecReg)              { m_SecReg   = SecReg;      }
  void        SetSeg(u16 Seg)                    { m_Seg      = Seg;         }
  void        SetValue(u64 Value)                { m_Value    = Value;       }
  void        SetSegValue(u16 SegValue)          { m_SegValue = SegValue;    }

private:
  u64         m_Type;
  std::string m_Name;
  u8          m_Offset;
  u16         m_Reg;
  u16         m_SecReg;
  u16         m_Seg;
  u64         m_Value;
  u16         m_SegValue;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_OPERAND_HPP
