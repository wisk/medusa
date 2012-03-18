#include "disassembly_view_control.hpp"

#include <boost/foreach.hpp>

#include <medusa/log.hpp>
#include <medusa/instruction.hpp>

const wxEventType DisassemblyEventType = wxNewEventType();

BEGIN_EVENT_TABLE(DisassemblyTextCtrl, wxStyledTextCtrl)
  EVT_RIGHT_UP(DisassemblyTextCtrl::OnMouseRightUp)
  EVT_CONTEXT_MENU(DisassemblyTextCtrl::OnContextMenu)
END_EVENT_TABLE()

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
  UsePopUp(false);

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

  SetMarginType(0, wxSTC_MARGIN_TEXT);
  SetMarginWidth(0, 128);
  StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(_("white")));

  Connect(wxEVT_STC_DOUBLECLICK, wxStyledTextEventHandler(DisassemblyTextCtrl::OnDoubleClick), NULL, this);
}

void DisassemblyTextCtrl::AddDisassemblyLine(medusa::Address const& rAddr, wxString const& rMsg, DisassemblyTextCtrl::LineType LineType)
{
  wxASSERT(wxIsMainThread() && "Not called from main thread");

  int TextLine = 0;
  wxString Line = rMsg;

  if (!rMsg.EndsWith(_("\n")))
    Line += _("\n");

  auto LineLen = Line.Len();
  while (LineLen--)
    if (Line.at(LineLen) == _('\n'))
      AddAddress(rAddr);

  if (AddressToLine(rAddr, TextLine) && TextLine > 0)
    TextLine--;

  SetReadOnly(false);
  InsertText(PositionFromLine(TextLine), Line);
  MarginSetText(TextLine, rAddr.ToString());
  SetReadOnly(true);
}

void DisassemblyTextCtrl::DeleteDisassemblyLine(medusa::Address const& rAddr)
{
  wxASSERT(wxIsMainThread() && "Not called from main thread");

  int Line;

  while (AddressToLine(rAddr, Line))
  {
    GotoLine(Line);
    SetEditable(true);
    LineDelete();
    SetEditable(false);
  }
}

bool DisassemblyTextCtrl::AddressToLine(medusa::Address const& rAddr, int& rLine)
{
  bool Res = true;

  rLine = 0;
  auto itAddr = m_Addresses.begin();
  for (; itAddr != m_Addresses.end() && *itAddr < rAddr; ++itAddr)
    rLine++;
  medusa::Log::Write("ui_wx") << "Addr: " << rAddr << ", Line: " << rLine << medusa::LogEnd;

  if (itAddr == m_Addresses.end())
  {
    Res = false;
    medusa::Log::Write("ui_wx") << "Not found !" << medusa::LogEnd;
  }

  if (rLine >= GetLineCount())
    rLine = GetLineCount() - 1;

  return Res;
}

bool DisassemblyTextCtrl::LineToAddress(int Line, medusa::Address& rAddr)
{
  if (Line > m_Addresses.size())
    return false;
  auto itAddr = m_Addresses.begin();
  std::advance(itAddr, Line);
  rAddr = *itAddr;
  return true;
}

void DisassemblyTextCtrl::AddCell(medusa::Address const& rAddr, medusa::Cell const& rCell)
{
  if (rCell.GetType() == medusa::Cell::CharacterType) return;

  wxString CellLine = wxString("  ") + rCell.ToString();

  wxString CellComment = rCell.GetComment();
  if (!CellComment.IsEmpty())
    CellLine += wxString(" ; ") + CellComment;

  if (rCell.GetType() == medusa::Cell::InstructionType &&
      static_cast<medusa::Instruction const&>(rCell).GetOperationType() != medusa::Instruction::OpUnknown)
    CellLine += _("\n");

  AddDisassemblyLine(rAddr, CellLine, CellType);
}

void DisassemblyTextCtrl::AddMultiCell(medusa::Address const& rAddr, medusa::MultiCell const& rMultiCell)
{
  AddDisassemblyLine(rAddr, rMultiCell.ToString(), MultiCellType);
}

void DisassemblyTextCtrl::AddLabel(medusa::Address const& rAddr, medusa::Label const& rLabel)
{
  AddDisassemblyLine(rAddr, wxString(_("\n")) + wxString(rLabel.GetLabel()) + wxString(_(":")), LabelType);
}

void DisassemblyTextCtrl::AddMemoryArea(medusa::MemoryArea const* pMemArea)
{
  if ( pMemArea->GetVirtualBase().GetAddressingType() == medusa::Address::UnknownType
    || pMemArea->GetVirtualBase().GetAddressingType() == medusa::Address::PhysicalType)
    return;
  AddDisassemblyLine(pMemArea->GetVirtualBase(), pMemArea->ToString(), MemoryAreaType);
}

bool DisassemblyTextCtrl::GoTo(medusa::Address const& rAddr)
{
  int Line;
  AddressToLine(rAddr, Line);
  GotoLine(Line);
  return true;
}

void DisassemblyTextCtrl::AddAddress(medusa::Address const& rAddr)
{
  m_Addresses.insert(rAddr);
}

void DisassemblyTextCtrl::DeleteAddress(medusa::Address const& rAddr)
{
  auto itBegAddr = m_Addresses.find(rAddr);
  if (itBegAddr == std::end(m_Addresses)) return;
  auto itEndAddr = itBegAddr;
  std::advance(itEndAddr, m_Addresses.count(rAddr));
  m_Addresses.erase(itBegAddr, itEndAddr);
}

void DisassemblyTextCtrl::ClearDisassembly(void)
{
  SetReadOnly(false);
  ClearAll();
  SetReadOnly(true);
  m_Addresses.erase(m_Addresses.begin(), m_Addresses.end());
}

void DisassemblyTextCtrl::OnDoubleClick(wxStyledTextEvent& rEvt)
{
  int CurLine = GetCurrentLine();
  wxString CurMarginText = MarginGetText(CurLine);

  medusa::Address CurAddr(CurMarginText.ToStdString());
  medusa::Log::Write("ui_wx") << "Current address " << CurAddr.ToString() << medusa::LogEnd;
  GoTo(CurAddr);

  int FoundLine;
  medusa::Address FoundAddr;

  if (LineToAddress(CurLine, FoundAddr))
    medusa::Log::Write("ui_wx") << "FoundAddr: " << FoundAddr.ToString() << medusa::LogEnd;

  if (AddressToLine(FoundAddr, FoundLine))
    medusa::Log::Write("ui_wx") << "FoundLine: " << FoundLine << medusa::LogEnd;

  int Line = 0;
  for (auto itAddr = std::begin(m_Addresses); itAddr != std::end(m_Addresses); ++itAddr)
    MarginSetText(Line++, itAddr->ToString());
}

void DisassemblyTextCtrl::OnMouseRightUp(wxMouseEvent& rEvt)
{
  GetParent()->HandleWindowEvent(rEvt);
  rEvt.Skip();
}

void DisassemblyTextCtrl::OnContextMenu(wxContextMenuEvent& rEvt)
{
  GetParent()->HandleWindowEvent(rEvt);
  rEvt.Skip();
}