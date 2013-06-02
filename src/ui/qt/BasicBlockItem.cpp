#include "BasicBlockItem.hpp"
#include <QObject>
#include <sstream>

BasicBlockItem::BasicBlockItem(QObject * parent, qreal width, qreal height, int id)
  : _parent(parent), _width(width), _height(height), _isPress(false), _id(id), _z(zValue()), _fx(new QGraphicsDropShadowEffect(this))
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
  std::ostringstream oss;
  oss << "bb_" << _id;
  painter->drawText(10, 10, QString::fromStdString(oss.str()));
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