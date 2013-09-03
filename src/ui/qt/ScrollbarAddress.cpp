#include "ScrollbarAddress.hpp"

ScrollbarAddress::ScrollbarAddress(QWidget * parent, medusa::Medusa& core)
  : QWidget(parent), View(medusa::Document::Subscriber::DocumentUpdated, core.GetDocument())
  , _core(core)
{
}

ScrollbarAddress::~ScrollbarAddress(void)
{
}

void ScrollbarAddress::paintEvent(QPainter * p)
{
  QColor clr = QColor(Qt::red);
  p->fillRect(rect(), clr);
}