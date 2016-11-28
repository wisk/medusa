#ifndef QMEDUSA_GRAPH_VIEW_HPP
#define QMEDUSA_GRAPH_VIEW_HPP

#include <QtCore>
#include <QtGui>
#include <QGraphicsView>
#include <QScrollBar>

// src: http://www.qtcentre.org/threads/35738-How-to-zooming-like-in-AutoCAD-with-QGraphicsView
class GraphView : public QGraphicsView
{
  Q_OBJECT
public:
  GraphView(QWidget* pParent = 0);
  void Zoom(qreal Factor, QPointF CenterPt);

protected:
  virtual void wheelEvent       (QWheelEvent *event);
  virtual void mousePressEvent  (QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent   (QMouseEvent *event);
  virtual void keyPressEvent    (QKeyEvent   *event);
  virtual void keyReleaseEvent  (QKeyEvent   *event);

private:
  bool    m_IsMoving;
  QPoint  m_LastPos;
  QCursor m_LastCursor;
};

#endif // QMEDUSA_GRAPH_VIEW_HPP