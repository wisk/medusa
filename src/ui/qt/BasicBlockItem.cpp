#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>

BasicBlockItem::BasicBlockItem(QObject * parent, medusa::Medusa& core, medusa::Address::List const& addresses)
  : _parent(parent)
  , _width(10.0), _height(10.0)
  , _isPress(false)
  , _core(core), _printer(core), _view(core, _printer, addresses)
  , _z(zValue()), _fx(new QGraphicsDropShadowEffect(this))
{
  setFlags(ItemIsMovable | ItemIsSelectable);
  _fx->setBlurRadius(25.0);
  setGraphicsEffect(_fx);
}

QRectF BasicBlockItem::boundingRect(void) const
{
  return QRectF(0, 0, _width, _height);
}

void BasicBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  auto fm = painter->fontMetrics();
  u32 viewWidth, viewHeight;
  _view.GetDimension(viewWidth, viewHeight);
  _width = viewWidth * fm.width('M');
  _height = viewHeight * fm.height();

  QRectF rect = boundingRect();
  QColor clr = Qt::lightGray;
  qreal opacity = 1.0;

  if (_isPress)
  {
    clr = Qt::blue;
    opacity = 0.7;
  }

  QBrush brush(clr);

  setOpacity(opacity);
  painter->fillRect(rect, brush);
  _fx->setColor(clr);

  _printer.SetFontMetrics(&fm);
  _printer.SetPainter(painter);
  _view.Print();
  _printer.SetPainter(nullptr);
  _printer.SetFontMetrics(nullptr);
  painter->drawRect(rect);
}

void BasicBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  _isPress = true;
  setZValue(1.0);
  update();
  QGraphicsItem::mousePressEvent(event);
}

void BasicBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mouseReleaseEvent(event);
  _isPress = false;
  setZValue(_z);
  update();
}