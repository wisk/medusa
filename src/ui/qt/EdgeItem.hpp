#ifndef QMEDUSA_EDGE_ITEM_HPP
#define QMEDUSA_EDGE_ITEM_HPP

#include <QAbstractGraphicsShapeItem>
#include <QPen>
#include <QPainter>

#include <medusa/basic_block.hpp>

#include <ogdf/basic/geometry.h>

class EdgeItem : public QAbstractGraphicsShapeItem
{
public:
  typedef medusa::BasicBlockEdgeProperties::Type EdgeType;
  enum { Type = UserType + 1 };

  EdgeItem(QGraphicsItem* startItem, QGraphicsItem* endItem, EdgeType type);

  int type(void) const { return Type; }
  virtual QRectF boundingRect(void) const;
  virtual QPainterPath shape(void) const;
  void setBends(ogdf::DPolyline const& bends) { _bends = bends; }
  void computeCoordinates(void);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  QGraphicsItem * _startItem, *_endItem;
  EdgeType        _type;
  QColor          _clr;
  ogdf::DPolyline _bends;
  QPainterPath    _line, _head;
};

#endif // !QMEDUSA_EDGE_ITEM_HPP