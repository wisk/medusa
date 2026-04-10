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
  void wheelEvent       (QWheelEvent *event) override;
  void mousePressEvent  (QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent   (QMouseEvent *event) override;
  void keyPressEvent    (QKeyEvent   *event) override;
  void keyReleaseEvent  (QKeyEvent   *event) override;

private:
  bool    m_IsMoving;
  QPoint  m_LastPos;
  QCursor m_LastCursor;
};

#endif // QMEDUSA_GRAPH_VIEW_HPP