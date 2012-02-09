#include "wxmedusa.hpp"
#include "medusa_frame.hpp"

bool MedusaApp::OnInit(void)
{
  MedusaFrame* pMedusaFrame = new MedusaFrame(NULL, wxSize(1024, 768));
  SetTopWindow(pMedusaFrame);
  pMedusaFrame->Show();

  pMedusaFrame->AddLogMessage(_("wxMedusa started !"));

  //pMedusaFrame->AddDisassemblyLine(wxT("start:"));
  //pMedusaFrame->AddDisassemblyLine(wxT("push rax"));
  //pMedusaFrame->AddDisassemblyLine(wxT("pop ebx"));
  //pMedusaFrame->AddDisassemblyLine(wxT("lea rsi, \"test\" ; pwet"));
  //pMedusaFrame->AddDisassemblyLine(wxT("call [rip + 0x10000]"));

  pMedusaFrame->AddLogMessage(_("Check us out at http://medusa.swap.gs"));

  pMedusaFrame->AddLabel(_("start"), _("code"), _("0x10000"));
  pMedusaFrame->AddLabel(_("str_Hello_World"), _("string"), _("0x220000"));

  //pMedusaFrame->AddDisassemblyLine(wxT("db \"Hello World\", 0"));
  return true;
}

IMPLEMENT_APP_CONSOLE(MedusaApp)
