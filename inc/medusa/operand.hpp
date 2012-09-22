#ifndef _MEDUSA_OPERAND_
#define _MEDUSA_OPERAND_

#include <string>

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"

MEDUSA_NAMESPACE_BEGIN

/* Operand
    8  LSB contains information about size
    24 MSB contains information about operand itself
*/
#define O_NONE       0x00000000  // Unused

#define O_MASK       0xffff0000
#define O_REG        0x00010000
#define O_IMM        0x00020000  // Immediate e.g. 0x10000
#define O_DISP       0x00040000  // Displacement e.g. xxx + 0x11223344, [xxx + 0x100000]
#define O_REL        0x00080000  // Relative e.g. +0x10
#define O_ABS        0x00100000  // Absolute e.g. 0x10000

#define O_SCALE      0x00200000  // Scale e.g. [xxx * 4] or LSL #xxx

#define O_MEM        0x00400000  // Memory e.g. [0x100000] or [Reg]
#define O_SREG       0x00800000  // Second register e.g. [reg + reg]
#define O_SEG        0x01000000  // Segment e.g. seg:[xxx]
#define O_SEG_VAL    0x02000000  // Segment value e.g. xxxx:yyyy

#define O_NO_REF     0x04000000 // Operand can't hold an address
#define O_REG_PC_REL 0x08000000 // Register operand holds the current address

// Register Size
#define REG_MASK     0x0000000f
#define RS_UNK       0x00000001
#define RS_8BIT      0x00000002
#define RS_16BIT     0x00000003
#define RS_32BIT     0x00000004
#define RS_64BIT     0x00000005
#define RS_80BIT     0x00000006
#define RS_128BIT    0x00000007

// Data Size (IMM / DISP / REL)
#define DS_MASK      0x000000f0
#define DS_UNK       0x00000010
#define DS_8BIT      0x00000020
#define DS_16BIT     0x00000030
#define DS_32BIT     0x00000040
#define DS_64BIT     0x00000050
#define DS_128BIT    0x00000060

// SCale
#define SC_MASK      0x00000f00
#define SC_UNK       0x00000100
#define SC_1         0x00000200
#define SC_2         0x00000300
#define SC_4         0x00000400
#define SC_8         0x00000500

// Memory Size (dereferencement)
#define MS_MASK      0x0000f000
#define MS_8BIT      0x00001000
#define MS_16BIT     0x00002000
#define MS_32BIT     0x00003000
#define MS_64BIT     0x00004000
#define MS_80BIT     0x00005000
#define MS_128BIT    0x00006000

// Alias
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

  u32&        Type(void)                  { return m_Type;     }
  u16&        Reg(void)                   { return m_Reg;      }
  u16&        SecReg(void)                { return m_SecReg;   }
  u16&        Seg(void)                   { return m_Seg;      }
  u64&        Value(void)                 { return m_Value;    }
  u16&        SegValue(void)              { return m_SegValue; }

  std::string GetName(void)     const     { return m_Name;     }
  u8          GetOffset(void)   const     { return m_Offset;   }
  u32         GetType(void)     const     { return m_Type;     }
  u16         GetReg(void)      const     { return m_Reg;      }
  u16         GetSecReg(void)   const     { return m_SecReg;   }
  u16         GetSeg(void)      const     { return m_Seg;      }
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
  u16         GetSegValue(void) const     { return m_SegValue;        }

  u8          GetLength(void) const;

  void        SetType(u32 Type)           { m_Type     = Type;        }
  void        SetName(std::string const& rName)  { m_Name     = rName;}
  void        SetOffset(u8 Offset)        { m_Offset   = Offset;      }
  void        SetReg(u16 Reg)             { m_Reg      = Reg;         }
  void        SetSecReg(u16 SecReg)       { m_SecReg   = SecReg;      }
  void        SetSeg(u16 Seg)             { m_Seg      = Seg;         }
  void        SetValue(u64 Value)         { m_Value    = Value;       }
  void        SetSegValue(u16 SegValue)   { m_SegValue = SegValue;    }

private:
  u32         m_Type;
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