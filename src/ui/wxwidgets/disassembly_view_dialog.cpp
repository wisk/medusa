#include "disassembly_view_comtrol.hpp"

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
  SetEditable(false);

  wxColour AsmDefClr      = wxColour("black"          );
  wxColour AsmCmtClr      = wxColour("sea green"      );
  wxColour AsmNbrClr      = wxColour("magenta"        );
  wxColour AsmStrClr      = wxColour("indian red"     );
  wxColour AsmOpClr       = wxColour("purple"         );
  wxColour AsmIdClr       = wxColour("cornflower blue");
  wxColour AsmCpuInsnClr  = wxColour("blue violet"    );
  wxColour AsmMathInsnClr = wxColour("blue violet"    );
  wxColour AsmRegClr      = wxColour("red"            );
  wxColour AsmDirClr      = wxColour("red"            );
  wxColour AsmDirOprClr   = wxColour("red"            );
  wxColour AsmCmtBlkClr   = wxColour("sea green"      );
  wxColour AsmChrClr      = wxColour("orange red"     );
  wxColour AsmOelClr      = wxColour("red"            );
  wxColour AsmExtInsnClr  = wxColour("red"            );

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
}

void DisassemblyTextCtrl::AddDisassemblyLine(wxString const& rMsg)
{
  SetEditable(true);
  AppendText(rMsg);
  if (!rMsg.EndsWith(wxT("\n")))
    AppendText(wxT("\n"));
  SetEditable(false);
}