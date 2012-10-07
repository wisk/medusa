#ifndef _MEDUSA_INSTRUCTION_
#define _MEDUSA_INSTRUCTION_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/cell.hpp"
#include "medusa/operand.hpp"
#include "medusa/database.hpp"

#include <cstring>

#ifdef _MSC_VER
# pragma warning(disable: 4251)
#endif

MEDUSA_NAMESPACE_BEGIN

#define I_NONE 0x0
#define P_NONE 0x0

#define OPERAND_NO  4

//! Instruction is a Cell which handles an instruction for any Architecture.
class Medusa_EXPORT Instruction : public Cell
{
public:
  enum OperationType
  {
    //! The instruction has specific no type
    OpUnknown = 0,
    //! The instruction changes the execution flow
    OpJump    = 1 << 0,
    //! The instruction calls a function
    OpCall    = 1 << 1,
    //! The instruction returns from a procedure
    OpRet     = 1 << 2,
    //! The instruction is conditional
    OpCond    = 1 << 3
  };

  enum FlagsType
  {
    FlNone      = 0,
    FlCarry     = 1 << 0,
    FlParity    = 1 << 1,
    FlAuxCarry  = 1 << 2,
    FlZero      = 1 << 3,
    FlSign      = 1 << 4,
    FlTrap      = 1 << 5,
    FlInterrupt = 1 << 6,
    FlDirection = 1 << 7,
    FlOverflow  = 1 << 8
  };

#define INSN_FLAG_TEST(flags)   ((flags) << 32)
#define INSN_FLAG_UPDATE(flags) ((flags) << 16)
#define INSN_FLAG_CLEAR(flags)  ((flags) <<  0)

  /*! Instruction construction
   * \param Name is the name of the instruction, it must not be allocated.
   * \param Opcode is the unique id for a kind of instruction.
   * \param Length is the length of this instruction.
   * \param Cond must be set if this instruction has a condition.
   * \param Prefix must be set if this instruction has a prefix.
   */
  Instruction(char const* Name = NULL, u32 Opcode = I_NONE, u8 Length = 0, u16 Prefix = P_NONE, u64 Flags = FlNone)
    : Cell(CellData::InstructionType)
    , m_OperationType(OpUnknown)
    , m_pName(NULL)
    , m_Opcd(Opcode)
    , m_Length(Length)
    , m_Prefix(Prefix)
    , m_Flags(Flags)
  {
  }
  ~Instruction(void) {}

  virtual size_t          GetLength(void) const       { return m_Length;          }

  char const*             GetName(void) const         { return m_pName;           }
  u32                     GetOperationType(void) const{ return m_OperationType;   }

  void                    SetName(char const* pName)  { m_pName = pName;          }
  void                    SetOpcode(u32 Opcd)         { m_Opcd = Opcd;            }
  void                    SetOperationType(u8 OperationType) { m_OperationType = OperationType; }
  void                    SetFlags(u64 Flags)         { m_Flags = Flags;          }

  medusa::Operand*        Operand(unsigned int Oprd)
  { return Oprd > OPERAND_NO ? NULL : &m_Oprd[Oprd];                              }
  medusa::Operand const*  Operand(unsigned int Oprd) const
  { return Oprd > OPERAND_NO ? NULL : &m_Oprd[Oprd];                              }
  medusa::Operand&        FirstOperand(void)          { return m_Oprd[0];         }
  medusa::Operand&        SecondOperand(void)         { return m_Oprd[1];         }
  medusa::Operand&        ThirdOperand(void)          { return m_Oprd[2];         }
  medusa::Operand&        FourthOperand(void)         { return m_Oprd[3];         }

  u32&                    Opcode(void)                { return m_Opcd;            }
  u8 &                    OperationType(void)         { return m_OperationType;   }
  u8 &                    Length(void)                { return m_Length;          }
  u32&                    Prefix(void)                { return m_Prefix;          }
  u64&                    Flags(void)                 { return m_Flags;           }
  u32                     GetPrefix(void) const       { return m_Prefix;          }
  u64                     GetFlags(void) const        { return m_Flags;           }

  /*! This method gives the offset of a specified operand
   * \param Oprd The operand number between 0 (included) and OPERAND_NO (excluded).
   * \return Returns 0 if the specified operand doesn't hold an offset or the offset.
   */
  u8                      GetOperandOffset(u8 Oprd) const;
  bool                    GetOperandReference(Database const& rDatabase, u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const;
  u8                      GetOperandReferenceLength(u8 Oprd) const;
  bool                    GetOperandAddress(u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const;

private:
  u8                      m_OperationType;    /*! This integer holds jmp/branch type (call, ret, ...)           */
  char const*             m_pName;            /*! This string holds the instruction name ("call", "lsl", ...)   */
  medusa::Operand         m_Oprd[OPERAND_NO]; /*! This array holds all operands                                 */
  u32                     m_Opcd;             /*! This integer holds the current opcode (ARM_Ldr, GB_Swap, ...) */
  u8                      m_Length;           /*! This integer holds the length of instruction (1, 2, ...)      */
  u32                     m_Prefix;           /*! This integer holds prefix flag (REP, LOCK, ...)               */
  u64                     m_Flags;            /*! This integer holds flags that this instruction can:
                                                  - test  [47:32] (INSN_FLAG_TEST)
                                                  - update[31:16] (INSN_FLAG_UPDATE)
                                                  - clear [15:0]  (INSN_FLAG_CLEAR)
                                              */
  };

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_INSTRUCTION_