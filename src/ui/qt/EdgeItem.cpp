#include "EdgeItem.hpp"

EdgeItem::EdgeItem(QGraphicsItem* startItem, QGraphicsItem* endItem)
  : _startItem(startItem), _endItem(endItem)
  , _clr(Qt::blue)
  , _bends()
{
  setZValue(1.0);
}

QPainterPath EdgeItem::shape(void) const
{
  QPainterPath path;
  path.addPath(_line);
  path.addPath(_head);
  return path;
}

QRectF EdgeItem::boundingRect(void) const
{
  return shape().boundingRect();
}

void EdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
  computeCoordinates();
  std::vector<QPointF> points;
  bool revLine = (_startItem->y() > _endItem->y()) ? true : false;
  _clr = revLine ? Qt::red : Qt::blue;

  painter->setRenderHint(QPainter::Antialiasing);
  QPen pen(_clr);
  pen.setWidth(2);
  QBrush brs(_clr);

  painter->setPen(pen);
  painter->drawPath(_line);
  painter->setBrush(brs);
  painter->drawPath(_head);
}

void EdgeItem::computeCoordinates(void)
{
  prepareGeometryChange();
  std::vector<QPointF> points;
  points.reserve(2);
  auto startRect = _startItem->boundingRect();
  auto endRect   = _endItem->boundingRect();
  bool revLine = (_startItem->y() > _endItem->y()) ? true : false;

  // Retrieve points
  if (revLine)
    points.push_back(QPointF(_endItem->x() + endRect.width() / 2, _endItem->y() + endRect.height()));
  else
    points.push_back(QPointF(_endItem->x() + endRect.width() / 2, _endItem->y()));

  for (auto it = _bends.begin(); it.valid(); ++it)
    points.push_back(QPointF((*it).m_x, (*it).m_y));
  // Why?!?!
  std::reverse(std::begin(points) + 1, std::end(points));

  if (revLine)
    points.push_back(QPointF(_startItem->x() + startRect.width() / 2, _startItem->y()));
  else
    points.push_back(QPointF(_startItem->x() + startRect.width() / 2, _startItem->y() + startRect.height()));

  // Retrieve lines and boundingRect
  std::list<QLineF> lines;
  auto iterPt = std::begin(points);
  QPointF startPt;
  startPt = *iterPt;
  ++iterPt;
  for (; iterPt != std::end(points); ++iterPt)
  {
    lines.push_back(QLineF(startPt, *iterPt));
    startPt = *iterPt;
  }

  // Generate path for line
  _line = QPainterPath();
  if (points.size() == 2)
  {
    QPolygonF polyLine;
    polyLine << points.front() << points.back();
    _line.addPolygon(polyLine);
  }

  else
  {
    // NOTE: bézier curves rendering is not that fancy
    //QPointF lastPoint = points.front();
    //for (size_t i = 1; i + 1 < points.size(); ++i)
    //{
    //  _line.moveTo(lastPoint);
    //  _line.quadTo(points[i], points[i + 1]);
    //  lastPoint = points[i + 1];
    //}

    for (size_t i = 1; i < points.size(); ++i)
    {
      _line.moveTo(points[i - 1]);
      _line.quadTo(points[i - 1], points[i]);
    }
  }

  // Generate path for head
  static const qreal Pi = 3.14;
  static const qreal arrowSize = 10.0;
  auto refLine = revLine ? lines.front() : lines.front();
  double angle = ::acos(refLine.dx() / refLine.length());
  if (revLine)
    angle = (Pi * 2) - angle;
  QPointF arrowPt = refLine.p1();
  QPointF arrowP1 = arrowPt + QPointF(::sin(angle + Pi / 3)      * arrowSize, ::cos(angle + Pi / 3)      * arrowSize);
  QPointF arrowP2 = arrowPt + QPointF(::sin(angle + Pi - Pi / 3) * arrowSize, ::cos(angle + Pi - Pi / 3) * arrowSize);
  QPolygonF head;
  head << arrowPt << arrowP1 << arrowP2;
  _head = QPainterPath();
  _head.addPolygon(head);
  _head.setFillRule(Qt::WindingFill);
}