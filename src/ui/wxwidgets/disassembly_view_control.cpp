#include "disassembly_view_control.hpp"

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

  Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DisassemblyTextCtrl::OnClick), NULL, this);
}

void DisassemblyTextCtrl::AddDisassemblyLine(wxString const& rMsg)
{
  SetReadOnly(false);
  AppendText(rMsg);
  if (!rMsg.EndsWith(wxT("\n")))
    AppendText(wxT("\n"));
  SetReadOnly(true);
}

void DisassemblyTextCtrl::OnClick(wxMouseEvent& rMouseEvt)
{
  //int Line = LineFromPosition(PositionFromPoint(rMouseEvt.GetPosition()));
  //wxMessageBox(wxString::Format("Line %d", Line));
}
