#include "x86.hpp"
#include "x86_architecture.hpp"

bool X86Architecture::FormatInstruction(
  Database      const& rDatabase,
  BinaryStream  const& rBinStrm,
  Address       const& rAddr,
  Instruction   const& rInsn,
  std::string        & rStrCell,
  Cell::Mark::List   & rMarks) const
{
  char Sep = '\0';
  std::ostringstream oss;

  if (rInsn.GetPrefix())
  {
    if (rInsn.GetPrefix() & X86_Prefix_Lock)
      oss << "lock ";
    else if (rInsn.GetPrefix() & X86_Prefix_RepNz)
      oss << "repnz ";
    else if (rInsn.GetPrefix() & X86_Prefix_Rep)
    {
      // 0xF3 is only used as REPZ prefix for cmps and scas instructions.
      if (rInsn.GetOpcode() == X86_Opcode_Cmps || rInsn.GetOpcode() == X86_Opcode_Scas)
        oss << "repz ";
      else
        oss << "rep ";
    }
  }

  oss << m_Mnemonic[rInsn.GetOpcode()] << " ";
  rMarks.push_back(Cell::Mark(Cell::Mark::MnemonicType, oss.str().length()));

  for (unsigned int i = 0; i < OPERAND_NO; ++i)
  {
    Operand const* pOprd = rInsn.Operand(i);
    if (pOprd == nullptr)
      break;
    if (pOprd->GetType() == O_NONE)
      break;

    if (Sep != '\0')
    {
      oss << Sep << " ";
      rMarks.push_back(Cell::Mark(Cell::Mark::OperatorType, 2));
    }

    FormatOperand(oss, rMarks, rDatabase, rAddr.GetOffset(), rInsn, pOprd);

    Sep = ',';
  }

  //auto rExpr = rInsn.GetSemantic();
  //if (rExpr.empty() == false)
  //{
  //  auto BegMark = oss.str().length();
  //  oss << " [ ";
  //  auto itExpr = std::begin(rExpr);
  //  oss << (*itExpr)->ToString();
  //  ++itExpr;
  //  std::for_each(itExpr, std::end(rExpr), [&oss](Expression const* pExpr)
  //  {
  //    oss << "; " << pExpr->ToString();
  //  });
  //  oss << " ]";
  //  rInsn.AddMark(Cell::Mark::KeywordType, oss.str().length() - BegMark);
  //}

  rStrCell = oss.str();
  return true;
}

void X86Architecture::ApplySegmentOverridePrefix(Instruction& rInsn, Operand* pOprd)
{
  if (rInsn.GetPrefix() && pOprd->GetType() & O_MEM)
  {
    // For instructions with two memory operands (cmps/movs),
    // the segment override is only applied on the source operand (i == 1).
    if (rInsn.Opcode() == X86_Opcode_Cmps || rInsn.Opcode() == X86_Opcode_Movs)
      return;

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