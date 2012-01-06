#include "medusa/architecture.hpp"

MEDUSA_NAMESPACE_BEGIN

void Architecture::FormatInstruction(Database const& rDatabase, Address const& rAddr, Instruction& rInsn)
{
  char Sep = '\0';
  std::ostringstream oss;

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
    else if (pOprd->GetType() & O_DISP || pOprd->GetType() & O_IMM && !(pOprd->GetType() & O_NO_REF))
    {
      Address OprdAddr(Address::UnknownType, pOprd->GetSegValue(), pOprd->GetValue());

      std::string MultiCellName = rDatabase.GetLabelFromAddress(OprdAddr).GetName();

      if (MultiCellName.empty())
      {
        oss << " " << pOprd->GetName();
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

MEDUSA_NAMESPACE_END
