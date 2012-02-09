#include "wxmedusa.hpp"
#include "medusa_frame.hpp"

#include <boost/bind.hpp>

#include <medusa/log.hpp>

bool MedusaApp::OnInit(void)
{
  MedusaFrame* pMedusaFrame = new MedusaFrame(NULL, wxSize(1024, 768));
  medusa::Log::SetLog(boost::bind(&MedusaFrame::AddLogMessage, pMedusaFrame, _1));
  medusa::Log::Write("ui_wx") << "Thank for using wxMedusa ! For further information, take a look at https://github.com/wisk/medusa." << medusa::LogEnd;
  SetTopWindow(pMedusaFrame);
  pMedusaFrame->Show();
  return true;
}

IMPLEMENT_APP_CONSOLE(MedusaApp)