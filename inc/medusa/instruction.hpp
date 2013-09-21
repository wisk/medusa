#ifndef _MEDUSA_INSTRUCTION_
#define _MEDUSA_INSTRUCTION_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/cell.hpp"
#include "medusa/operand.hpp"
#include "medusa/document.hpp"
#include "medusa/expression.hpp"

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
  typedef std::shared_ptr<Instruction> SPtr;

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

  /*! Instruction construction
   * \param Name is the name of the instruction, it must not be allocated.
   * \param Opcode is the unique id for a kind of instruction.
   * \param Length is the length of this instruction.
   */
  Instruction(char const* Name = nullptr, u32 Opcode = I_NONE, u16 Length = 0)
    : Cell(CellData::InstructionType)
    , m_OperationType(OpUnknown)
    , m_pName(nullptr)
    , m_Opcd(Opcode)
    , m_Prefix()
    , m_TestedFlags()
    , m_UpdatedFlags()
    , m_ClearedFlags()
    , m_FixedFlags()
    , m_Expressions()
  {
    m_spDna->Length() = Length;
  }

  Instruction(CellData::SPtr spDna, std::string const& rComment = "")
    : Cell(spDna, rComment) 
    , m_OperationType(OpUnknown)
    , m_pName(nullptr)
    , m_Opcd(0x0)
    , m_Prefix()
    , m_TestedFlags()
    , m_UpdatedFlags()
    , m_ClearedFlags()
    , m_FixedFlags()
    , m_Expressions()
  {}

  Instruction(Instruction const& rInsn);
  Instruction&            operator=(Instruction const& rInsn);

  ~Instruction(void);


  char const*             GetName(void) const         { return m_pName;           }
  u32                     GetOperationType(void) const{ return m_OperationType;   }

  void                    SetName(char const* pName)  { m_pName = pName;          }
  void                    SetOpcode(u32 Opcd)         { m_Opcd = Opcd;            }
  void                    SetOperationType(u8 OperationType) { m_OperationType = OperationType; }
  void                    SetTestedFlags(u32 Flags)   { m_TestedFlags = Flags;    }
  void                    SetUpdatedFlags(u32 Flags)  { m_UpdatedFlags = Flags;   }
  void                    SetClearedFlags(u32 Flags)  { m_ClearedFlags = Flags;   }
  void                    SetFixedFlags(u32 Flags)    { m_FixedFlags = Flags;     }
  void                    SetSemantic(Expression::List const& rExprList);
  void                    SetSemantic(Expression* pExpr);
  void                    AddPreSemantic(Expression* pExpr);
  void                    AddPostSemantic(Expression* pExpr);

  medusa::Operand*        Operand(unsigned int Oprd)
  { return Oprd > OPERAND_NO ? nullptr : &m_Oprd[Oprd];                           }
  medusa::Operand const*  Operand(unsigned int Oprd) const
  { return Oprd > OPERAND_NO ? nullptr : &m_Oprd[Oprd];                           }
  medusa::Operand&        FirstOperand(void)          { return m_Oprd[0];         }
  medusa::Operand&        SecondOperand(void)         { return m_Oprd[1];         }
  medusa::Operand&        ThirdOperand(void)          { return m_Oprd[2];         }
  medusa::Operand&        FourthOperand(void)         { return m_Oprd[3];         }

  u32&                    Opcode(void)                { return m_Opcd;            }
  u8 &                    OperationType(void)         { return m_OperationType;   }
  u16&                    Length(void)                { return m_spDna->Length(); }
  u32&                    Prefix(void)                { return m_Prefix;          }
  u32&                    TestedFlags(void)           { return m_TestedFlags;     }
  u32&                    UpdatedFlags(void)          { return m_UpdatedFlags;    }
  u32&                    ClearedFlags(void)          { return m_ClearedFlags;    }
  u32&                    FixedFlags(void)            { return m_FixedFlags;      }

  u32                     GetOpcode(void) const       { return m_Opcd;            }
  u32                     GetPrefix(void) const       { return m_Prefix;          }
  u32                     GetTestedFlags(void) const  { return m_TestedFlags;     }
  u32                     GetUpdatedFlags(void) const { return m_UpdatedFlags;    }
  u32                     GetClearedFlags(void) const { return m_ClearedFlags;    }
  u32                     GetFixedFlags(void) const   { return m_FixedFlags;      }
  Expression::List const& GetSemantic(void) const     { return m_Expressions;     }

  /*! This method gives the offset of a specified operand
   * \param Oprd The operand number between 0 (included) and OPERAND_NO (excluded).
   * \return Returns 0 if the specified operand doesn't hold an offset or the offset.
   */
  u8                      GetOperandOffset(u8 Oprd) const;
  bool                    GetOperandReference(Document const& rDoc, u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const;
  u8                      GetOperandReferenceLength(u8 Oprd) const;
  bool                    GetOperandAddress(u8 Oprd, Address const& rAddrSrc, Address& rAddrDst) const;
  bool                    GetIndirectReferences(Document const& rDoc, u8 Oprd, Address::List& rRefAddr) const;

private:
  u8                      m_OperationType;    /*! This integer holds jmp/branch type (call, ret, ...)                 */
  char const*             m_pName;            /*! This string holds the instruction name ("call", "lsl", ...)         */
  medusa::Operand         m_Oprd[OPERAND_NO]; /*! This array holds all operands                                       */
  u32                     m_Opcd;             /*! This integer holds the current opcode (ARM_Ldr, GB_Swap, ...)       */
  u32                     m_Prefix;           /*! This integer holds prefix flag (REP, LOCK, ...)                     */
  u32                     m_TestedFlags;      /*! This integer holds flags that are tested by the instruction         */
  u32                     m_UpdatedFlags;     /*! This integer holds flags that could be modified by the instruction  */
  u32                     m_ClearedFlags;     /*! This integer holds flags that are unset by the instruction          */
  u32                     m_FixedFlags;       /*! This integer holds flags that are set by the instruction            */
  Expression::List        m_Expressions;      /*! This list contains semantic for this instruction if not empty       */
  };

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_INSTRUCTION_
