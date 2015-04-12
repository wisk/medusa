#include "BindingView.hpp"
#include <medusa/module.hpp>
#include <medusa/log.hpp>

BindingView::BindingView(medusa::Medusa& rCore) : m_rCore(rCore), m_pCurBinding(nullptr)
{
  setupUi(this);

  connect(BindingCombo, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [&](QString const& rText)
  {
    auto& rModMgr = medusa::ModuleManager::Instance();
    auto pGetBinding = rModMgr.GetBinding(rText.toStdString());
    if (pGetBinding == nullptr)
    {
      medusa::Log::Write("ui_qt") << "unable to find binding module " << rText.toStdString() << medusa::LogEnd;
      return;
    }

    // FIXME: beware race condition...
    delete m_pCurBinding;

    m_pCurBinding = pGetBinding();
    m_pCurBinding->Bind(m_rCore);
  });

  connect(ExecuteButton, &QPushButton::clicked, [&]()
  {
    if (m_pCurBinding == nullptr)
    {
      medusa::Log::Write("ui_qt") << "please, select a binding first" << medusa::LogEnd;
      return;
    }

    if (!m_pCurBinding->Execute(ScriptCode->toPlainText().toStdString()))
    {
      medusa::Log::Write("ui_qt") << "script execution failed" << medusa::LogEnd;
    }
  });

  // FIXME: At this time, only python is supported...
  BindingCombo->addItem("python");
}

BindingView::~BindingView(void)
{
}