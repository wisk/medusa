#ifndef ControlFlowGraphView_h__
#define ControlFlowGraphView_h__

#include <QtCore>
#include <QtGui>
#include <QGraphicsView>
#include <QScrollBar>

// src: http://www.qtcentre.org/threads/35738-How-to-zooming-like-in-AutoCAD-with-QGraphicsView
class ControlFlowGraphView : public QGraphicsView
{
  Q_OBJECT
public:
  ControlFlowGraphView(QWidget * parent = 0);
  void zoom(qreal factor, QPointF centerPt);

protected:
  virtual void wheelEvent       (QWheelEvent *event);
  virtual void mousePressEvent  (QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseMoveEvent   (QMouseEvent *event);
  virtual void keyPressEvent    (QKeyEvent   *event);
  virtual void keyReleaseEvent  (QKeyEvent   *event);

private:
  bool    _isMoving;
  QPoint  _lastPos;
  QCursor _lastCursor;
};

#endif // ControlFlowGraphView_h__