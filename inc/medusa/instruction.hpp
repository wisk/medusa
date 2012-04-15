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
#define C_NONE 0x0
#define P_NONE 0x0

#define OPERAND_NO  4

//! Instruction is a Cell which handles an instruction for any Architecture.
class Medusa_EXPORT Instruction : public Cell
{
public:
  enum OperationType
  {
    OpUnknown,  //! The instruction has specific no type
    OpJump,     //! The instruction changes the execution flow
    OpCall,     //! The instruction calls a function
    OpRet,      //! The instruction returns from a procedure
  };

  /*! Instruction construction
   * \param Name is the name of the instruction, it must not be allocated.
   * \param Opcode is the unique id for a kind of instruction.
   * \param Length is the length of this instruction.
   * \param Cond must be set if this instruction has a condition.
   * \param Prefix must be set if this instruction has a prefix.
   */
  Instruction(char const* Name = NULL, u32 Opcode = I_NONE, u8 Length = 0, u8 Cond = C_NONE, u16 Prefix = P_NONE)
    : Cell(Cell::InstructionType)
    , m_OperationType(OpUnknown)
    , m_pName(NULL)
    , m_Opcd(Opcode)
    , m_Length(Length)
    , m_Cond(Cond)
    , m_Prefix(Prefix)
  {
  }
  ~Instruction(void) {}

  virtual size_t          GetLength(void) const       { return m_Length;          }

  char const*             GetName(void) const         { return m_pName;           }
  u8                      GetCond(void) const         { return m_Cond;            }
  u32                     GetOperationType(void) const{ return m_OperationType;   }

  void                    SetName(char const* pName)  { m_pName = pName;          }
  void                    SetOpcode(u32 Opcd)         { m_Opcd = Opcd;            }
  void                    SetCondition(u8 Cond)       { m_Cond = Cond;            }
  void                    SetOperationType(u8 OperationType) { m_OperationType = OperationType; }

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
  u8 &                    Cond(void)                  { return m_Cond;            }
  u32&                    Prefix(void)                { return m_Prefix;          }
  u32                     GetPrefix(void) const       { return m_Prefix;          }

  /*! This method gives the offset of a specified operand
   * \param Oprd The operand number betweend 0 (included) and OPERAND_NO (excluded).
   * \return Returns 0 if the specified operand doesn't hold an offset or the offset.
   */
  u8                      GetOperandOffset(u8 Oprd) const;
  bool                    GetOperandReference(Database const& rDatabase, u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const;
  u8                      GetOperandReferenceLength(u8 Oprd) const;
  bool                    GetOperandAddress(u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const;

  //virtual void                  Load(SerializeEntity::SPtr spSrlzEtt);
  //virtual SerializeEntity::SPtr Save(void);

private:
  u8                      m_OperationType;    /*! This integer holds jmp/branch type (call, ret, ...)           */
  char const*             m_pName;            /*! This string holds the instruction name ("call", "lsl", ...)   */
  medusa::Operand         m_Oprd[OPERAND_NO]; /*! This array holds all operands                                 */
  u32                     m_Opcd;             /*! This integer holds the current opcode (ARM_Ldr, GB_Swap, ...) */
  u8                      m_Length;           /*! This integer holds the length of instruction (1, 2, ...)      */
  u8                      m_Cond;             /*! This integer holds the conditional type (none, zero, carry)   */
  u32                     m_Prefix;           /*! This integer holds prefix flag (REP, LOCK, ...)               */
  };

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_INSTRUCTION_