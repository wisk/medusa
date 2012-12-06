#include "x86.hpp"
#include "x86_architecture.hpp"

void X86Architecture::FormatInstruction(Database const& rDatabase, BinaryStream const& rBinStrm, Address const& rAddr, Instruction& rInsn) const
{
  char Sep = '\0';
  std::ostringstream oss;

  rInsn.ResetMarks();

  rInsn.SetName(m_Mnemonic[rInsn.Opcode()]);

  if (rInsn.Prefix())
  {
    if (rInsn.Prefix() & X86_Prefix_Lock)
      oss << "lock ";
    else if (rInsn.Prefix() & X86_Prefix_RepNz)
      oss << "repnz ";
    else if (rInsn.Prefix() & X86_Prefix_Rep)
    {
      // 0xF3 is only used as REPZ prefix for cmps and scas instructions.
      if (rInsn.Opcode() == X86_Opcode_Cmps || rInsn.Opcode() == X86_Opcode_Scas)
        oss << "repz ";
      else
        oss << "rep ";
    }
  }

  oss << rInsn.GetName() << " ";
  rInsn.AddMark(Cell::Mark::MnemonicType, oss.str().length());

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand* pOprd = rInsn.Operand(i);
    if (pOprd == NULL)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
    {
      oss << Sep << " ";
      rInsn.AddMark(Cell::Mark::OperatorType, 2);
    }

    FormatOperand(oss, rDatabase, rAddr.GetOffset(), rInsn, pOprd);

    Sep = ',';
  }
  rInsn.UpdateString(oss.str());
}

void X86Architecture::ApplySegmentOverridePrefix(Instruction &rInsn)
{
  for (int i = 0; i < OPERAND_NO; i++)
  {
    Operand *pOprd = rInsn.Operand(i);

    if (rInsn.GetPrefix() && pOprd->GetType() & O_MEM)
    {
      // For instructions with two memory operands (cmps/movs),
      // the segment override is only applied on the source operand (i == 1).
      if ((rInsn.Opcode() == X86_Opcode_Cmps || rInsn.Opcode() == X86_Opcode_Movs) && !i)
        continue;

      // TODO: What do we do for multiple segment override prefix?
      // Intel Vol2A 2.1.1: For each instruction, it is only useful to include
      // up to one prefix code from each of the four groups (Groups 1, 2, 3, 4).
      if (rInsn.GetPrefix() & X86_Prefix_CS || rInsn.GetPrefix() & X86_Prefix_HintNotTaken)
      {
        pOprd->Type() |= O_SEG;
        pOprd->Seg() = X86_Reg_Cs;
      }
      else if (rInsn.GetPrefix() & X86_Prefix_DS || rInsn.GetPrefix() & X86_Prefix_HintTaken)
      {
        pOprd->Type() |= O_SEG;
        pOprd->Seg() = X86_Reg_Ds;
      }
      else if (rInsn.GetPrefix() & X86_Prefix_ES)
      {
        pOprd->Type() |= O_SEG;
        pOprd->Seg() = X86_Reg_Es;
      }
      else if (rInsn.GetPrefix() & X86_Prefix_FS || rInsn.GetPrefix() & X86_Prefix_HintAltTaken)
      {
        pOprd->Type() |= O_SEG;
        pOprd->Seg() = X86_Reg_Fs;
      }
      else if (rInsn.GetPrefix() & X86_Prefix_GS)
      {
        pOprd->Type() |= O_SEG;
        pOprd->Seg() = X86_Reg_Gs;
      }
      else if (rInsn.GetPrefix() & X86_Prefix_SS)
      {
        pOprd->Type() |= O_SEG;
        pOprd->Seg() = X86_Reg_Ss;
      }
    }
  }
}