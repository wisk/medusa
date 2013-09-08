#include "ScrollbarAddress.hpp"

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::DocumentUpdated, core.GetDocument())
  , _core(core)
{
  setFixedWidth(40);
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::paintEvent(QPaintEvent * p)
{
  QColor clr = QColor(Qt::red);
  QPainter painter(this);
  painter.fillRect(rect(), clr);
}