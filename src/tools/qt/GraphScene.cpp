#include "GraphScene.hpp"

#include "VertexItem.hpp"
#include "EdgeItem.hpp"

#include <medusa/user_configuration.hpp>

#include <list>

#ifdef MEDUSA_HAS_OGDF
# include <ogdf/basic/Graph.h>
# include <ogdf/basic/GraphAttributes.h>
# include <ogdf/layered/SugiyamaLayout.h>
# include <ogdf/layered/MedianHeuristic.h>
# include <ogdf/layered/OptimalRanking.h>
# include <ogdf/layered/OptimalHierarchyLayout.h>
#endif

GraphScene::GraphScene(QObject* pParent, medusa::Medusa& rCore, medusa::Address const& rGraphAddr)
  : QGraphicsScene(pParent)
  , m_rCore(rCore)
  , m_GraphAddr(rGraphAddr)
{
  if (!_Update())
    medusa::Log::Write("ui_qt") << "failed to build graph for: " << m_GraphAddr << medusa::LogEnd;
}

bool GraphScene::_Update(void)
{
  medusa::UserConfiguration UserCfg;
  setBackgroundBrush(QBrush(QColor(QString::fromStdString(UserCfg.GetOption("color.background_listing")))));

#ifndef MEDUSA_HAS_OGDF
  medusa::Log::Write("ui_qt").Level(medusa::LogError) << "you must compiled medusa with OGDF library to display graph" << medusa::LogEnd;
  return false;
#else
  auto pMC = m_rCore.GetDocument().GetMultiCell(m_GraphAddr);
  if (pMC == nullptr)
  {
    medusa::Log::Write("ui_qt").Level(medusa::LogError) << "null multicell" << medusa::LogEnd;
    return false;
  }

  auto spGraph = pMC->GetGraph();
  if (spGraph == nullptr)
  {
    medusa::Log::Write("ui_qt").Level(medusa::LogError) << "no graph attached to multicell" << medusa::LogEnd;
    return false;
  }

  medusa::GraphData GD;
  if (!m_rCore.FormatGraph(*spGraph, GD))
  {
    medusa::Log::Write("ui_qt").Level(medusa::LogError) << "failed to format graph" << medusa::LogEnd;
    return false;
  }

  QString fontInfo = QString::fromStdString(UserCfg.GetOption("font.listing"));
  QFont Font(fontInfo);
  QFontMetrics fm(Font);

  auto FontWidth  = static_cast<medusa::u16>(fm.width('M'));
  auto FontHeight = static_cast<medusa::u16>(fm.height());

  std::unordered_map<medusa::u64, ogdf::node>  OgdfNodes;
  std::unordered_map<medusa::u64, VertexItem*> QtNodes;

  auto const& rBoostGraph = (*spGraph)();

  auto Vertices = boost::vertices(rBoostGraph);
  // Add vertices
  auto VerticesRange = boost::vertices(rBoostGraph);
  for (auto itVtx = VerticesRange.first; itVtx != Vertices.second; ++itVtx)
  {
    auto const& rVtxDesc = rBoostGraph[*itVtx];

    medusa::Address VtxAddr;
    if (!rVtxDesc.GetFirstAddress(VtxAddr))
      return false;

    medusa::u16 X, Y;
    if (!GD.GetVertexPosition(VtxAddr, X, Y))
      return false;

    auto pVtxItem = new VertexItem(this, m_rCore, rVtxDesc.GetAddresses());

    medusa::Graph::VertexDescriptor VtxDesc;
    if (spGraph->FindVertex(VtxAddr, VtxDesc))
    {
      auto& rVtxProp = rBoostGraph[VtxDesc];

      if (rVtxProp.IsHead())
        pVtxItem->SetBackgroundColor(QString::fromStdString(UserCfg.GetOption("color.background_node_begin")));
      else if (rVtxProp.IsEnd())
        pVtxItem->SetBackgroundColor(QString::fromStdString(UserCfg.GetOption("color.background_node_end")));
    }

    QtNodes[*itVtx] = pVtxItem;
    pVtxItem->setPos(X * FontWidth, Y * FontHeight);
    addItem(pVtxItem);
  }

  // Bind them with edges
  auto EdgesRange = boost::edges(rBoostGraph);
  for (auto itEdge = EdgesRange.first; itEdge != EdgesRange.second; ++itEdge)
  {
    auto pQtVtxSrc = QtNodes[itEdge->m_source];
    auto pQtVtxTgb = QtNodes[itEdge->m_target];

    auto pEdgeItem = new EdgeItem(pQtVtxSrc, pQtVtxTgb, rBoostGraph[*itEdge].GetType());

    medusa::GraphData::BendsType Bends;
    if (GD.GetBends(*itEdge, Bends))
    {
      // Convert medusa metric (per character) to Qt metric (using font size)
      for (auto& rPtPos : Bends)
      {
        std::get<0>(rPtPos) *= FontWidth;
        std::get<1>(rPtPos) *= FontHeight;
      }
      pEdgeItem->SetBends(Bends);
    }

    addItem(pEdgeItem);
  }

  return true;
#endif
}