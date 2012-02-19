#include "disassembly_view_control.hpp"

#include <boost/foreach.hpp>

#include <medusa/log.hpp>
#include <medusa/instruction.hpp>

//DEFINE_EVENT_TYPE(DisassemblyEventType)
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
  AddLineAddress(GetLineCount() - 1, rAddr);

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
  AddLineAddress(GetLineCount() - 1, rAddr);
  AddDisassemblyLine(rMultiCell.ToString());
}

void DisassemblyTextCtrl::AddLabel(medusa::Address const& rAddr, medusa::Label const& rLabel)
{
  AddDisassemblyLine(wxString("\n"));

  AddLineAddress(GetLineCount() - 1, rAddr);
  AddDisassemblyLine(wxString(rLabel.GetLabel()) + wxString(":"));
}

void DisassemblyTextCtrl::AddMemoryArea(medusa::MemoryArea const& rMemArea)
{
  AddDisassemblyLine(rMemArea.ToString());
}

bool DisassemblyTextCtrl::GoTo(medusa::Address const& rAddr)
{
  std::list<int> LineList;

  if (!AddressToLine(rAddr, LineList))
    return false;

  if (LineList.empty())
    return false;

  GotoLine(*LineList.begin());
  return true;
}

void DisassemblyTextCtrl::ClearDisassembly(void)
{
  m_AddrToLineMap.erase(m_AddrToLineMap.begin(), m_AddrToLineMap.end());
  m_LineToAddrMap.erase(m_LineToAddrMap.begin(), m_LineToAddrMap.end());
  SetReadOnly(false);
  ClearAll();
  SetReadOnly(true);
}

void DisassemblyTextCtrl::OnDoubleClick(wxStyledTextEvent& rEvt)
{
  medusa::Address Addr;
  LineToAddress(GetCurrentLine(), Addr);

  medusa::Log::Write("ui_wx")
    << "Current address: " << Addr.ToString()
    << medusa::LogEnd;

  std::list<int> LineList;
  if (AddressToLine(Addr, LineList))
    BOOST_FOREACH(int Line, LineList)
    medusa::Log::Write("ui_wx") << "  Line: " << Line << medusa::LogEnd;
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

void DisassemblyTextCtrl::AddLineAddress(int Line, medusa::Address const& rAddr)
{
  m_AddrToLineMap.insert(AddrToLineMap::value_type(rAddr, Line));
  m_LineToAddrMap.insert(LineToAddrMap::value_type(Line, rAddr));
  MarginSetText(Line, rAddr.ToString());
}

bool DisassemblyTextCtrl::AddressToLine(medusa::Address const& rAddr, std::list<int>& rLineList)
{
  std::pair<AddrToLineMap::iterator, AddrToLineMap::iterator> Iit;
  AddrToLineMap::iterator It;

  Iit = m_AddrToLineMap.equal_range(rAddr);
  for (It = Iit.first; It != Iit.second; ++It)
    rLineList.push_back(It->second);

  return !rLineList.empty();
}

bool DisassemblyTextCtrl::LineToAddress(int Line, medusa::Address& rAddr)
{
  LineToAddrMap::iterator It = m_LineToAddrMap.find(Line);

  if (It == m_LineToAddrMap.end())
    return false;

  rAddr = It->second;
  return true;
}