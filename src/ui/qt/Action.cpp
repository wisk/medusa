#include "Action.hpp"

UiAction::UiAction(QObject* pParent, medusa::Action::SPtr spAction, QKeySequence const& rShortcut, medusa::FullDisassemblyView const* pView)
  : QAction(QString::fromStdString(spAction->GetName()), pParent), m_spAction(spAction), m_pView(pView)
{
  setShortcut(rShortcut);
  setStatusTip(QString::fromStdString(spAction->GetDescription()));
  setIcon(QIcon(QString(":/icons/%1").arg(spAction->GetIconName().c_str())));

  connect(this, &UiAction::triggered, [this](bool)
  {
    auto FirstAddr = m_pView->GetSelectionFirstAddress();
    auto LastAddr  = m_pView->GetSelectionLastAddress();

    if (FirstAddr > LastAddr)
      std::swap(FirstAddr, LastAddr);

    auto RangeAddr = std::make_pair(FirstAddr, LastAddr);

    auto Index = m_pView->GetSelectionIndex();

    if (!m_spAction->IsCompatible())
      return;
    m_spAction->Do();
  });
}

bool UiAction::IsCompatible(void) const
{
  return m_spAction->IsCompatible();
}

void UiAction::Do(void)
{
  return m_spAction->Do();
}
