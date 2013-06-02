#ifndef __EDGE_ITEM_HPP__
#define __EDGE_ITEM_HPP__

#include <QAbstractGraphicsShapeItem>
#include <QPen>
#include <QPainter>

class EdgeItem : public QAbstractGraphicsShapeItem
{
public:
  enum { Type = UserType + 1 };

  EdgeItem(QGraphicsItem* startItem, QGraphicsItem* endItem);

  int type(void) const { return Type; }
  virtual QRectF boundingRect(void) const;
  virtual QPainterPath shape(void) const;
  void computeCoordinates(void);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  QGraphicsItem *_startItem, *_endItem;
  QColor _clr;
  QPainterPath _line, _head;
};

#endif // !__EDGE_ITEM_HPP__