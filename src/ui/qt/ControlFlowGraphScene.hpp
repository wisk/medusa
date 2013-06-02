#ifndef ControlFlowGraphScene_h__
#define ControlFlowGraphScene_h__

#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

class ControlFlowGraphScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit ControlFlowGraphScene(QObject * parent = 0) : QGraphicsScene(parent) {}
private:
};

#endif // ControlFlowGraphScene_h__