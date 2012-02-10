#include "disassembly_view_control.hpp"

#include <medusa/log.hpp>
#include <medusa/instruction.hpp>

DisassemblyTextCtrl::DisassemblyTextCtrl(
  wxWindow *pParent, wxWindowID WinId,
  wxPoint const& rPos, wxSize const& rSize,
  long Style, wxString const& rName)
  : wxStyledTextCtrl(pParent, WinId, rPos, rSize, Style, rName)
{
  SetWrapMode(wxSTC_WRAP_NONE);
  StyleClearAll();
  SetLexer(wxSTC_LEX_ASM);
  SetUseAntiAliasing(true);
  SetReadOnly(true);

  wxColour AsmDefClr      = wxColour(_("black")          );
  wxColour AsmCmtClr      = wxColour(_("sea green")      );
  wxColour AsmNbrClr      = wxColour(_("magenta")        );
  wxColour AsmStrClr      = wxColour(_("indian red")     );
  wxColour AsmOpClr       = wxColour(_("purple")         );
  wxColour AsmIdClr       = wxColour(_("cornflower blue"));
  wxColour AsmCpuInsnClr  = wxColour(_("blue violet")    );
  wxColour AsmMathInsnClr = wxColour(_("blue violet")    );
  wxColour AsmRegClr      = wxColour(_("red")            );
  wxColour AsmDirClr      = wxColour(_("red")            );
  wxColour AsmDirOprClr   = wxColour(_("red")            );
  wxColour AsmCmtBlkClr   = wxColour(_("sea green")      );
  wxColour AsmChrClr      = wxColour(_("orange red")     );
  wxColour AsmOelClr      = wxColour(_("red")            );
  wxColour AsmExtInsnClr  = wxColour(_("red")            );

  StyleSetForeground(wxSTC_ASM_DEFAULT,          AsmDefClr     );
  StyleSetForeground(wxSTC_ASM_COMMENT,          AsmCmtClr     );
  StyleSetForeground(wxSTC_ASM_NUMBER,           AsmNbrClr     );
  StyleSetForeground(wxSTC_ASM_STRING,           AsmStrClr     );
  StyleSetForeground(wxSTC_ASM_OPERATOR,         AsmOpClr      );
  StyleSetForeground(wxSTC_ASM_IDENTIFIER,       AsmIdClr      );
  StyleSetForeground(wxSTC_ASM_CPUINSTRUCTION,   AsmCpuInsnClr );
  StyleSetForeground(wxSTC_ASM_MATHINSTRUCTION,  AsmMathInsnClr);
  StyleSetForeground(wxSTC_ASM_REGISTER,         AsmRegClr     );
  StyleSetForeground(wxSTC_ASM_DIRECTIVE,        AsmDirClr     );
  StyleSetForeground(wxSTC_ASM_DIRECTIVEOPERAND, AsmDirOprClr  );
  StyleSetForeground(wxSTC_ASM_COMMENTBLOCK,     AsmCmtBlkClr  );
  StyleSetForeground(wxSTC_ASM_CHARACTER,        AsmChrClr     );
  StyleSetForeground(wxSTC_ASM_STRINGEOL,        AsmOelClr     );
  StyleSetForeground(wxSTC_ASM_EXTINSTRUCTION,   AsmExtInsnClr );

  Connect(wxEVT_STC_DOUBLECLICK, wxStyledTextEventHandler(DisassemblyTextCtrl::OnDoubleClick), NULL, this);
}

void DisassemblyTextCtrl::AddDisassemblyLine(wxString const& rMsg)
{
  SetReadOnly(false);
  AppendText(rMsg);
  if (!rMsg.EndsWith(wxT("\n")))
    AppendText(wxT("\n"));
  SetReadOnly(true);
}

void DisassemblyTextCtrl::AddCell(medusa::Address const& rAddr, medusa::Cell const& rCell)
{
  AddLineAddress(GetLineCount(), rAddr);

  wxString CellLine = wxString("  ") + rCell.ToString();

  wxString CellComment = rCell.GetComment();
  if (!CellComment.IsEmpty())
    CellLine += wxString(" ; ") + CellComment;

  AddDisassemblyLine(CellLine);

  if (rCell.GetType() == medusa::Cell::InstructionType &&
      static_cast<medusa::Instruction const&>(rCell).GetOperationType() != medusa::Instruction::OpUnknown)
    AddDisassemblyLine("\n");
}

void DisassemblyTextCtrl::AddMultiCell(medusa::Address const& rAddr, medusa::MultiCell const& rMultiCell)
{
  AddLineAddress(GetLineCount(), rAddr);
  AddDisassemblyLine(rMultiCell.ToString());
}

void DisassemblyTextCtrl::AddLabel(medusa::Address const& rAddr, medusa::Label const& rLabel)
{
  AddLineAddress(GetLineCount(), rAddr);
  AddDisassemblyLine(wxString("\n") + rLabel.GetLabel() + wxString(":"));
}

void DisassemblyTextCtrl::AddMemoryArea(medusa::MemoryArea const& rMemArea)
{
  AddDisassemblyLine(rMemArea.ToString());
}

void DisassemblyTextCtrl::OnDoubleClick(wxStyledTextEvent& rEvt)
{
  medusa::Address Addr;
  LineToAddress(GetCurrentLine(), Addr);

  medusa::Log::Write("ui_wx")
    << "Current line: " << GetCurrentLine()
    << "\nCurrent address: " << Addr.ToString()
    << medusa::LogEnd;
}

void DisassemblyTextCtrl::AddLineAddress(int Line, medusa::Address const& rAddr)
{
  m_Map.insert(Map::value_type(Line, rAddr));
}

bool DisassemblyTextCtrl::AddressToLine(medusa::Address const& rAddr, std::list<int>& rLineList)
{
  for (Map::const_iterator It = m_Map.begin(); It != m_Map.end(); ++It)
    if (It->second == rAddr)
    {
      rLineList.push_back(It->first);
    }

  return !rLineList.size();
}

bool DisassemblyTextCtrl::LineToAddress(int Line, medusa::Address& rAddr)
{
  Map::iterator It = m_Map.find(Line);

  if (It == m_Map.end())
    return false;

  rAddr = It->second;
  return true;
}
