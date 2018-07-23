#ifndef QMEDUSA_EDGE_ITEM_HPP
#define QMEDUSA_EDGE_ITEM_HPP

#include <QAbstractGraphicsShapeItem>
#include <QPen>
#include <QPainter>

#include <medusa/config.hpp>
#include <medusa/basic_block.hpp>
#include <medusa/cell_text.hpp>

#ifdef MEDUSA_HAS_OGDF
#include <ogdf/basic/geometry.h>
#endif

// TODO(wisk): take a look at https://doc.qt.io/qt-5/qtwidgets-graphicsview-elasticnodes-example.html
class EdgeItem : public QAbstractGraphicsShapeItem
{
public:
  typedef medusa::Graph::EdgeProperties::Type EdgeType;
  enum { Type = UserType + 1 };

  EdgeItem(QGraphicsItem* startItem, QGraphicsItem* endItem, EdgeType type);

#ifdef MEDUSA_HAS_OGDF
  int type(void) const { return Type; }
  virtual QRectF boundingRect(void) const;
  virtual QPainterPath shape(void) const;
  void setBends(ogdf::DPolyline const& bends) { _bends = bends; }
  void SetBends(medusa::GraphData::BendsType const& rBends) { m_Bends = rBends; }
  void computeCoordinates(void);

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
#endif

private:
  QGraphicsItem * _startItem, *_endItem;
  EdgeType        _type;
  QColor          _clr;
#ifdef MEDUSA_HAS_OGDF
  ogdf::DPolyline _bends;
#endif
  medusa::GraphData::BendsType m_Bends;
  QPainterPath    _line, _head;
};

#endif // !QMEDUSA_EDGE_ITEM_HPP