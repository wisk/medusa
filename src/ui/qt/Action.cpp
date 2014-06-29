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

    if (!m_spAction->IsCompatible(RangeAddr))
      return;
    m_spAction->Do(RangeAddr);
  });
}

bool UiAction::IsCompatible(medusa::Action::RangeAddress const& rRangeAddress) const
{
  return m_spAction->IsCompatible(rRangeAddress);
}

void UiAction::Do(medusa::Action::RangeAddress const& rRangeAddress)
{
  return m_spAction->Do(rRangeAddress);
}
