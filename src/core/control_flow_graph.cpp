#include "medusa/control_flow_graph.hpp"
#include <boost/graph/graphviz.hpp>
#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include "medusa/address.hpp"
#include "medusa/instruction.hpp"
#include "medusa/log.hpp"

MEDUSA_NAMESPACE_USE

ControlFlowGraph::ControlFlowGraph(Document const& rDoc)
  : m_rDoc(rDoc)
{
}

void ControlFlowGraph::AddBasicBlockVertex(BasicBlockVertexProperties const& rVertex)
{
  if (m_VertexMap.find(rVertex.GetFirstAddress()) != std::end(m_VertexMap))
    return;
  m_VertexMap[rVertex.GetFirstAddress()] = boost::add_vertex(rVertex, m_Graph);
}

bool ControlFlowGraph::AddBasicBlockEdge(BasicBlockEdgeProperties const& rEdge, Address const& rSrcAddr, Address const& rDstAddr)
{
  BasicBlockVertexDescriptor SrcDesc, DstDesc;
  if (!FindBasicBlock(rSrcAddr, SrcDesc) || !FindBasicBlock(rDstAddr, DstDesc))
    return false;

  auto Edge = boost::edge(SrcDesc, DstDesc, m_Graph);
  if (Edge.second)
  {
    Log::Write("core") << rSrcAddr.ToString() << " and " << rDstAddr.ToString() << " are already connected" << LogEnd;
    return false;
  }

  boost::add_edge(SrcDesc, DstDesc, rEdge, m_Graph);
  return true;
}

bool ControlFlowGraph::FindBasicBlock(Address const& rAddr, BasicBlockVertexDescriptor& BasicBlckDesc)
{
  for (auto itVertexPair = std::begin(m_VertexMap); itVertexPair != std::end(m_VertexMap); ++itVertexPair)
  {
    if (m_Graph[itVertexPair->second].Contains(rAddr))
    {
      BasicBlckDesc = itVertexPair->second;
      return true;
    }
  }
  return false;
}

bool ControlFlowGraph::SplitBasicBlock(Address const& rDstAddr, Address const& rSrcAddr, BasicBlockEdgeProperties::Type Type)
{
  for (auto itVertexPair = std::begin(m_VertexMap); itVertexPair != std::end(m_VertexMap); ++itVertexPair)
  {
    if (m_Graph[itVertexPair->second].Contains(rDstAddr))
    {
      Address::Vector NewBasicBlockAddresses;

      if (!m_Graph[itVertexPair->second].Split(rDstAddr, NewBasicBlockAddresses))
        break;

      if (NewBasicBlockAddresses.empty())
      {
        Log::Write("core") << "empty basic block" << LogEnd;
        return false;
      }

      AddBasicBlockVertex(BasicBlockVertexProperties(m_rDoc, NewBasicBlockAddresses));
      AddBasicBlockEdge(BasicBlockEdgeProperties(BasicBlockEdgeProperties::Unconditional), itVertexPair->second, rDstAddr);
      return true;
    }
  }
  return false;
}

void ControlFlowGraph::Finalize(Document const& rDoc)
{
  for (auto itVertex = std::begin(m_VertexMap); itVertex != std::end(m_VertexMap); ++itVertex)
  {
    auto const& rBscBlk = m_Graph[itVertex->second];
    Address::Vector const& rBlockAddrs = rBscBlk.GetAddresses();

    auto spLastInsn = std::dynamic_pointer_cast<Instruction>(rDoc.GetCell(rBscBlk.GetLastAddress()));
    if (spLastInsn == nullptr)
      continue;

    if (spLastInsn->GetSubType() == Instruction::JumpType)
      continue;

    bool SkipBlock = false;
    for (auto itAddr = std::begin(rBlockAddrs); itAddr != std::end(rBlockAddrs); ++itAddr)
    {
      auto spCell = rDoc.GetCell(*itAddr);
      if (spCell == nullptr)
        continue;
      if (spCell->GetType() != Cell::InstructionType)
        continue;

      u32 OpType = std::static_pointer_cast<Instruction>(spCell)->GetSubType();

      if (!(OpType & Instruction::ReturnType))
        continue;
      if ( (OpType & Instruction::ConditionalType))
        continue;
      SkipBlock = true;
      break;
    }

    if (SkipBlock)
      continue;

    auto itNextVertex = itVertex;
    ++itNextVertex;

    if (itNextVertex == std::end(m_VertexMap))
      return;

    auto CurEdge = boost::edge(itVertex->second, itNextVertex->second, m_Graph);
    if (CurEdge.second)
      continue;

    Log::Write("core") << "Fix orphan basic blocks at " << itVertex->first.ToString() << " and " << itNextVertex->first.ToString() << LogEnd;
    AddBasicBlockEdge(BasicBlockEdgeProperties(BasicBlockEdgeProperties::Next), itVertex->first, itNextVertex->first);
  }
}

bool ControlFlowGraph::Layout(PositionMap& rPosMap)
{
  PointVector BscBlkPts;
  BscBlkPts.resize(boost::num_vertices(m_Graph));
  rPosMap = PositionMap(BscBlkPts.begin(), boost::get(boost::vertex_index, m_Graph));

  boost::random_graph_layout(m_Graph, rPosMap, boost::square_topology<>(50.0));

  BasicBlockIterator BscBlkIter, BscBlkIterEnd;
  for (boost::tie(BscBlkIter, BscBlkIterEnd) = boost::vertices(m_Graph); BscBlkIter != BscBlkIterEnd; ++BscBlkIter)
  {
    std::cout << rPosMap[*BscBlkIter][0] << " " << rPosMap[*BscBlkIter][1] << std::endl;
  }

  return true;
}

void ControlFlowGraph::ForEachBasicBlock(std::function<void (BasicBlockVertexProperties const&)> Predicat) const
{
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
    Predicat(m_Graph[*it]);
}

void ControlFlowGraph::ForEachBasicBlock(std::function<bool (BasicBlockVertexProperties const&)> Predicat, bool& rResult) const
{
  rResult = false;
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
    if (Predicat(m_Graph[*it]) == true)
    {
      rResult = true;
      return;
    }
}

void ControlFlowGraph::ForEachEdgeIterator(std::function<void (EdgeIterator const&)> Predicat)
{
  //auto itRange = boost::edges(m_Graph);
  //for (auto it = itRange.first; it != itRange.second; ++it)
  //  Predicat(it);
}

void ControlFlowGraph::ForEachBasicBlockIterator(std::function<void (BasicBlockIterator const&)> Predicat) const
{
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
    Predicat(it);
}

void ControlFlowGraph::ForEachBasicBlockIterator(std::function<bool (BasicBlockIterator const&)> Predicat, bool& rResult) const
{
  rResult = false;
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
    if (Predicat(it) == true)
    {
      rResult = true;
      return;
    }
}

void ControlFlowGraph::ForEachInstruction(std::function<void (Address const&)> Predicat) const
{
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
  {
    auto const& rBscBlkAddrs = m_Graph[*it].GetAddresses();
    for (auto itAddr = std::begin(rBscBlkAddrs); itAddr != std::end(rBscBlkAddrs); ++itAddr)
      Predicat(*itAddr);
  }
}

void ControlFlowGraph::ForEachInstruction(std::function<bool (Address const&)> Predicat, bool& rResult) const
{
  rResult = false;
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
  {
    auto const& rBscBlkAddrs = m_Graph[*it].GetAddresses();
    for (auto itAddr = std::begin(rBscBlkAddrs); itAddr != std::end(rBscBlkAddrs); ++itAddr)
      if (Predicat(*itAddr) == true)
      {
        rResult = true;
        return;
      }
  }
}

bool ControlFlowGraph::GetPreviousAddress(Address const& rAddr, Address::Vector& rPrevAddr) const
{
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
  {
    auto BscBlk = m_Graph[*it];
    if (!BscBlk.Contains(rAddr))
      continue;

    if (BscBlk.GetFirstAddress() == rAddr)
    {
      auto AdjBscBlks = boost::in_edges(*it, m_Graph);
      for (auto itInBscBlk = AdjBscBlks.first; itInBscBlk != AdjBscBlks.second; ++itInBscBlk)
      {
        auto SrcBscBlkId = boost::source(*itInBscBlk, m_Graph);
        auto SrcBscBlk = m_Graph[SrcBscBlkId];
        rPrevAddr.push_back(SrcBscBlk.GetLastAddress());
      }
      return true;
    }

    else
    {
      Address PrevAddr;
      if (!BscBlk.GetPreviousAddress(rAddr, PrevAddr))
        return false;
      rPrevAddr.push_back(PrevAddr);
      return true;
    }
  }
  return false;
}

bool ControlFlowGraph::GetNextAddress(Address const& rAddr, Address::Vector& rNextAddr) const
{
  auto itRange = boost::vertices(m_Graph);
  for (auto it = itRange.first; it != itRange.second; ++it)
  {
    auto BscBlk = m_Graph[*it];
    if (!BscBlk.Contains(rAddr))
      continue;

    if (BscBlk.GetLastAddress() == rAddr)
    {
      auto AdjBscBlks = boost::out_edges(*it, m_Graph);
      for (auto itOutBscBlk = AdjBscBlks.first; itOutBscBlk != AdjBscBlks.second; ++itOutBscBlk)
      {
        auto TgtBscBlkId = boost::target(*itOutBscBlk, m_Graph);
        auto TgtBscBlk = m_Graph[TgtBscBlkId];
        rNextAddr.push_back(TgtBscBlk.GetFirstAddress());
      }
      return true;
    }

    else
    {
      Address NextAddr;
      if (!BscBlk.GetNextAddress(rAddr, NextAddr))
        return false;
      rNextAddr.push_back(NextAddr);
      return true;
    }
  }
  return false;
}

bool ControlFlowGraph::Contains(Address const& rAddress) const
{
  bool Result;
  ForEachInstruction([&rAddress](Address const& rAddr){ return rAddr == rAddress; }, Result);
  return Result;
}