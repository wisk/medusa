#include "x86.hpp"
#include "x86_architecture.hpp"

void X86Architecture::FormatInstruction(Database const& rDatabase, Address const& rAddr, Instruction& rInsn)
{
  char Sep = '\0';
  std::ostringstream oss;

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

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand* pOprd = rInsn.Operand(i);
    if (pOprd == NULL)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
      oss << Sep << " ";

    if (pOprd->GetType() & O_REL || pOprd->GetType() & O_ABS)
    {
      Address DstAddr;
      if (rInsn.GetOperandReference(0, rAddr, DstAddr))
      {
        std::string Label = "";

        //if (pCell != NULL) Label = pCell->GetLabel();
        Label = rDatabase.GetLabelFromAddress(DstAddr).GetName();

        std::string OprdName;
        if (Label.empty())  OprdName = DstAddr.ToString();
        else                OprdName = Label;

        pOprd->SetName(OprdName.c_str());
        oss << OprdName;
      }
      else
        oss << pOprd->GetName();
    }
    else if (pOprd->GetType() & O_DISP || pOprd->GetType() & O_IMM)
    {
      Address OprdAddr(Address::UnknownType, pOprd->GetSegValue(), pOprd->GetValue());

      std::string MultiCellName = rDatabase.GetLabelFromAddress(OprdAddr).GetName();

      if (MultiCellName.empty())
      {
        oss << pOprd->GetName();
        Sep = ',';
        continue;
      }

      rInsn.SetComment(MultiCellName);
      oss << pOprd->GetName();
    }
    else
      oss << pOprd->GetName();

    Sep = ',';
  }
  rInsn.SetStringForm(oss.str());
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

