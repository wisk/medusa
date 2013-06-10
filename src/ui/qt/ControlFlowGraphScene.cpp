#include "ControlFlowGraphScene.hpp"

#include "BasicBlockItem.hpp"

#include <list>

ControlFlowGraphScene::ControlFlowGraphScene(QObject * parent, medusa::Medusa& core, medusa::Function const& func)
  : QGraphicsScene(parent)
  , _core(core), _func(func)
{
  auto cfg = func.GetControlFlowGraph();
  cfg.ForEachBasicBlock([this](medusa::BasicBlockVertexProperties const& vertex)
  {
    auto item = new BasicBlockItem(this, _core, vertex.GetAddresses());
    addItem(item);
  });
}