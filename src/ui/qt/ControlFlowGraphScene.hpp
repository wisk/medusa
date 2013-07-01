#ifndef ControlFlowGraphScene_h__
#define ControlFlowGraphScene_h__

#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>

#include <medusa/medusa.hpp>
#include <medusa/function.hpp>

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

class ControlFlowGraphScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit ControlFlowGraphScene(QObject * parent, medusa::Medusa& core, medusa::Function const& func);

private:
  medusa::Medusa&         _core;
  medusa::Function const& _func;
};

#endif // ControlFlowGraphScene_h__