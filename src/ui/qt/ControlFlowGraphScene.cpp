#include "ControlFlowGraphScene.hpp"

#include "BasicBlockItem.hpp"
#include "EdgeItem.hpp"

#include <medusa/user_configuration.hpp>

#include <list>

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

ControlFlowGraphScene::ControlFlowGraphScene(QObject* pParent, medusa::Medusa& rCore, medusa::Address const& rCfgAddr)
  : QGraphicsScene(pParent)
  , m_rCore(rCore)
  , m_CfgAddr(rCfgAddr)
{
  if (!_Update())
    medusa::Log::Write("ui_qt") << "failed to build CFG for: " << m_CfgAddr << medusa::LogEnd;
}

bool ControlFlowGraphScene::_Update(void)
{
  ogdf::Graph Graph;
  ogdf::GraphAttributes GraphAttr(Graph, ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics);


  medusa::ControlFlowGraph CFG;
  if (!m_rCore.BuildControlFlowGraph(m_CfgAddr, CFG))
    return false;

  qreal MaxBbWidth = 0.0, MaxBbHeight = 0.0;

  QFontMetrics fm(font());
  std::map<ogdf::node,  BasicBlockItem*> Nodes;
  std::map<ogdf::edge,  EdgeItem*      > Edges;
  std::map<medusa::u64, ogdf::node     > UsedBscBlock;

  auto AddBscBlk = [&](medusa::u64 bbId) -> BasicBlockItem*
  {
    auto itBscBlk = UsedBscBlock.find(bbId);
    if (itBscBlk != std::end(UsedBscBlock))
      return Nodes[ UsedBscBlock[ bbId ] ];
      
    auto pBbItem = new BasicBlockItem(this, m_rCore, CFG.GetGraph()[bbId].GetAddresses());
    auto NewNode = Graph.newNode();
    auto Rect = pBbItem->boundingRect();
    GraphAttr.width()[NewNode]  = Rect.width();
    GraphAttr.height()[NewNode] = Rect.height();
    MaxBbWidth  = std::max(MaxBbWidth,  Rect.width());
    MaxBbHeight = std::max(MaxBbHeight, Rect.height());
    Nodes[NewNode]     = pBbItem;
    UsedBscBlock[bbId] = NewNode;
    return pBbItem;
  };

  auto const& g = CFG.GetGraph();

  auto VertexRange = boost::vertices(g);
  for (auto VertexIter = VertexRange.first; VertexIter != VertexRange.second; ++VertexIter)
    AddBscBlk(*VertexIter);

  auto EdgeRange = boost::edges(g);
  for (auto itEdge = EdgeRange.first; itEdge != EdgeRange.second; ++itEdge)
  {
    auto SrcBb     = AddBscBlk(itEdge->m_source);
    auto TgtBb     = AddBscBlk(itEdge->m_target);
    auto NewEdge   = Graph.newEdge(UsedBscBlock[itEdge->m_source], UsedBscBlock[itEdge->m_target]);
    auto pEdgeItem  = new EdgeItem(SrcBb, TgtBb, g[*itEdge].GetType());
    Edges[NewEdge] = pEdgeItem;
  }

  auto OHL = new ogdf::OptimalHierarchyLayout;
  OHL->nodeDistance(25.0);
  OHL->layerDistance(50.0);
  OHL->weightBalancing(0.0);
  OHL->weightSegments(0.0);

  ogdf::SugiyamaLayout SL;
  SL.setRanking(new ogdf::OptimalRanking);
  SL.setCrossMin(new ogdf::MedianHeuristic);
  SL.alignSiblings(false);
  SL.setLayout(OHL);
  SL.call(GraphAttr);

  for (auto itNode = std::begin(Nodes); itNode != std::end(Nodes); ++itNode)
  {
    addItem(itNode->second);
    QRectF BbRect = itNode->second->boundingRect();
    qreal x = GraphAttr.x(itNode->first) - (BbRect.width()  / 2);
    qreal y = GraphAttr.y(itNode->first) - (BbRect.height() / 2);
    itNode->second->setPos(x, y);
  }

  for (auto itEdge = std::begin(Edges); itEdge != std::end(Edges); ++itEdge)
  {
    auto const& rBends = GraphAttr.bends(itEdge->first);
    itEdge->second->setBends(rBends);
    addItem(itEdge->second);
  }

  setSceneRect(0, 0, GraphAttr.boundingBox().width() + MaxBbWidth, GraphAttr.boundingBox().height() + MaxBbHeight);
  medusa::UserConfiguration UserCfg;
  setBackgroundBrush(QBrush(QColor(QString::fromStdString(UserCfg.GetOption("color.background_listing")))));
  return true;
}