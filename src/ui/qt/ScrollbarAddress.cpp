#include "ScrollbarAddress.hpp"

#include <medusa/log.hpp>

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::AddressUpdated, core.GetDocument())
  , _core(core)
{
  setFixedWidth(40);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::OnAddressUpdated(medusa::Address::List const& rAddressList)
{
  /*
  std::ostringstream out;

  out << "addresses:";
  std::for_each(std::begin(rAddressList), std::end(rAddressList), [&](medusa::Address const& addr)
  {
    out << " " << addr.ToString();
  });

  medusa::Log::Write("ui_qt") << out.str() << medusa::LogEnd;
  */
}

void ScrollbarAddress::paintEvent(QPaintEvent * p)
{
  QColor clr = QColor(Qt::red);
  QPainter painter(this);
  painter.fillRect(rect(), clr);
}
