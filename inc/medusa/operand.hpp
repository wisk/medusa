#ifndef _MEDUSA_OPERAND_
#define _MEDUSA_OPERAND_

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
#define O_NONE       0x0000000000000000  // Unused

#define O_MASK       0x00000000fff00000
#define O_REG        0x0000000000100000  // Register
#define O_IMM        0x0000000000200000  // Immediate e.g. 0x10000
#define O_DISP       0x0000000000400000  // Displacement e.g. xxx + 0x11223344, [xxx + 0x100000]
#define O_REL        0x0000000000800000  // Relative e.g. +0x10
#define O_ABS        0x0000000001000000  // Absolute e.g. 0x10000

#define O_SCALE      0x0000000002000000  // Scale e.g. [xxx * 4] or LSL #xxx

#define O_MEM        0x0000000004000000  // Memory e.g. [0x100000] or [Reg]
#define O_SREG       0x0000000008000000  // Second register e.g. [reg + reg]
#define O_SEG        0x0000000010000000  // Segment e.g. seg:[xxx]
#define O_SEG_VAL    0x0000000020000000  // Segment value e.g. xxxx:yyyy

#define O_NO_REF     0x0000000040000000  // Operand can't hold an address
#define O_REG_PC_REL 0x0000000080000000  // Register operand holds the current address
#define O_WRITEBACK  0x0000000100000000  // Write back

// Register Size
#define RS_MASK      0x000000000000000f
#define RS_1BIT      0x0000000000000001
#define RS_8BIT      0x0000000000000002
#define RS_16BIT     0x0000000000000003
#define RS_32BIT     0x0000000000000004
#define RS_64BIT     0x0000000000000005
#define RS_80BIT     0x0000000000000006
#define RS_128BIT    0x0000000000000007

// Data Size (IMM / DISP / REL)
#define DS_MASK      0x00000000000000f0
#define DS_UNK       0x0000000000000010
#define DS_8BIT      0x0000000000000020
#define DS_16BIT     0x0000000000000030
#define DS_32BIT     0x0000000000000040
#define DS_64BIT     0x0000000000000050
#define DS_128BIT    0x0000000000000060

// SCale
#define SC_MASK      0x0000000000000f00
#define SC_1         0x0000000000000100
#define SC_2         0x0000000000000200
#define SC_4         0x0000000000000400
#define SC_8         0x0000000000000800

// Memory Size (dereferencement)
#define MS_MASK      0x000000000000f000
#define MS_8BIT      0x0000000000001000
#define MS_16BIT     0x0000000000002000
#define MS_32BIT     0x0000000000003000
#define MS_64BIT     0x0000000000004000
#define MS_80BIT     0x0000000000005000
#define MS_128BIT    0x0000000000006000

// Address Size
#define AS_MASK      0x00000000000f0000
#define AS_8BIT      0x0000000000010000
#define AS_16BIT     0x0000000000020000
#define AS_32BIT     0x0000000000030000
#define AS_64BIT     0x0000000000040000

// Alias
#define O_REG1     (O_REG   | RS_1BIT  )
#define O_REG8     (O_REG   | RS_8BIT  )
#define O_REG16    (O_REG   | RS_16BIT )
#define O_REG32    (O_REG   | RS_32BIT )
#define O_REG64    (O_REG   | RS_64BIT )
#define O_REG80    (O_REG   | RS_80BIT )
#define O_REG128   (O_REG   | RS_128BIT)
#define O_IMM8     (O_IMM   | DS_8BIT  )
#define O_IMM16    (O_IMM   | DS_16BIT )
#define O_IMM32    (O_IMM   | DS_32BIT )
#define O_IMM64    (O_IMM   | DS_64BIT )
#define O_DISP8    (O_DISP  | DS_8BIT  )
#define O_DISP16   (O_DISP  | DS_16BIT )
#define O_DISP32   (O_DISP  | DS_32BIT )
#define O_DISP64   (O_DISP  | DS_64BIT )
#define O_REL8     (O_REL   | DS_8BIT  )
#define O_REL16    (O_REL   | DS_16BIT )
#define O_REL32    (O_REL   | DS_32BIT )
#define O_REL64    (O_REL   | DS_64BIT )
#define O_ABS8     (O_ABS   | DS_8BIT  )
#define O_ABS16    (O_ABS   | DS_16BIT )
#define O_ABS32    (O_ABS   | DS_32BIT )
#define O_ABS64    (O_ABS   | DS_64BIT )
#define O_SCALE1   (O_SCALE | SC_1     )
#define O_SCALE2   (O_SCALE | SC_2     )
#define O_SCALE4   (O_SCALE | SC_4     )
#define O_SCALE8   (O_SCALE | SC_8     )
#define O_MEM8     (O_MEM   | MS_8BIT  )
#define O_MEM16    (O_MEM   | MS_16BIT )
#define O_MEM32    (O_MEM   | MS_32BIT )
#define O_MEM64    (O_MEM   | MS_64BIT )
#define O_MEM80    (O_MEM   | MS_80BIT )
#define O_MEM128   (O_MEM   | MS_128BIT)
#define O_ADDR8    (O_MEM   | AS_8BIT  )
#define O_ADDR16   (O_MEM   | AS_16BIT )
#define O_ADDR32   (O_MEM   | AS_32BIT )
#define O_ADDR64   (O_MEM   | AS_64BIT )

//! Operand is a generic class which contains information about an instruction operand.
class Medusa_EXPORT Operand
{
public:
  Operand(void)
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
  ~Operand(void) {}

  u64&        Type(void)                         { return m_Type;            }
  u16&        Reg(void)                          { return m_Reg;             }
  u16&        SecReg(void)                       { return m_SecReg;          }
  u16&        Seg(void)                          { return m_Seg;             }
  u64&        Value(void)                        { return m_Value;           }
  u16&        SegValue(void)                     { return m_SegValue;        }

  u8          GetOffset(void)   const            { return m_Offset;          }
  u64         GetType(void)     const            { return m_Type;            }
  u16         GetReg(void)      const            { return m_Reg;             }
  u16         GetSecReg(void)   const            { return m_SecReg;          }
  u16         GetSeg(void)      const            { return m_Seg;             }
  u64         GetValue(void)    const
  {
    switch (m_Type & DS_MASK)
    {
    case DS_8BIT:           return m_Value & 0xff;
    case DS_16BIT:          return m_Value & 0xffff;
    case DS_32BIT:          return m_Value & 0xffffffff;
    case DS_64BIT: default: return m_Value;
    }
  }
  u16         GetSegValue(void) const            { return m_SegValue;        }

  u8          GetLength(void) const;
  u8          GetRawLength(void) const;
  u32         GetSizeInBit(void) const;

  Expression *GetSemantic(u8 Mode, CpuInformation const* pCpuInfo, u8 InstructionLength = 0, bool Dereference = true) const;

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

#endif // !_MEDUSA_OPERAND_
