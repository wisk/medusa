#include "medusa/control_flow_graph.hpp"
#include <boost/graph/graphviz.hpp>
#include "medusa/address.hpp"
#include "medusa/instruction.hpp"
#include "medusa/log.hpp"

// Workaround from http://stackoverflow.com/questions/9669109/print-a-constified-subgraph-with-write-graphviz
template<typename Graph> struct prop_writer
{
  prop_writer(Graph const& g) : g_(g) {}
  template<typename Vertex> void operator()(std::ostream & out, Vertex v) const
  {
    out << "[shape=box] [label=\"";
    for (auto itAddr = std::begin(g_[v].GetAddresses()); itAddr != std::end(g_[v].GetAddresses()); ++itAddr)
      out << *itAddr << "\\n";
    out << "\"]";
  }

  Graph const& g_;
};

// Workaround from http://stackoverflow.com/questions/9669109/print-a-constified-subgraph-with-write-graphviz
template<typename Graph> struct prop_writer_db
{
  prop_writer_db(Graph const& g, medusa::Database const& rDb) : g_(g), db_(rDb) {}
  template<typename Vertex> void operator()(std::ostream & out, Vertex v) const
  {
    out << "[shape=box] [label=\"";
    for (auto itAddr = std::begin(g_[v].GetAddresses()); itAddr != std::end(g_[v].GetAddresses()); ++itAddr)
    {
      std::string LineString = "Unknown";
      auto pCell = db_.RetrieveCell(*itAddr);
      if (pCell != nullptr)
        LineString = pCell->ToString();

      out << *itAddr << ": " << LineString << "\\n";
    }
    out << "\"]";
  }

  Graph const& g_;
  medusa::Database const& db_;
};

MEDUSA_NAMESPACE_USE

ControlFlowGraph::ControlFlowGraph(void)
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

  //auto itDesc = m_VertexMap.find(rAddr);
  //if (itDesc == std::end(m_VertexMap))
  //  return false;

  //BasicBlckDesc = itDesc->second;
  //return true;
}

bool ControlFlowGraph::SplitBasicBlock(Address const& rDstAddr, Address const& rSrcAddr, BasicBlockEdgeProperties::Type Type)
{
  for (auto itVertexPair = std::begin(m_VertexMap); itVertexPair != std::end(m_VertexMap); ++itVertexPair)
  {
    if (m_Graph[itVertexPair->second].Contains(rDstAddr))
    {
      Address::List NewBasicBlockAddresses;
      if (!m_Graph[itVertexPair->second].Split(rDstAddr, NewBasicBlockAddresses))
        break;

      AddBasicBlockVertex(BasicBlockVertexProperties(NewBasicBlockAddresses));
      AddBasicBlockEdge(BasicBlockEdgeProperties(BasicBlockEdgeProperties::Unconditional), itVertexPair->second, rDstAddr);
      return true;
    }
  }
  return false;
}

void ControlFlowGraph::Finalize(Database const& rDb)
{
  for (auto itVertex = std::begin(m_VertexMap); itVertex != std::end(m_VertexMap); ++itVertex)
  {
    Address::List const& rBlockAddrs = m_Graph[itVertex->second].GetAddresses();
    bool SkipBlock = false;
    for (auto itAddr = std::begin(rBlockAddrs); itAddr != std::end(rBlockAddrs); ++itAddr)
    {
      Cell const* pCell = rDb.RetrieveCell(*itAddr);
      if (pCell == nullptr)                                                                 continue;
      if (pCell->GetType() != Cell::InstructionType)                                        continue;
      if (static_cast<Instruction const*>(pCell)->GetOperationType() != Instruction::OpRet) continue;
      SkipBlock = true;
    }

    if (SkipBlock) continue;

    auto itNextVertex = itVertex;
    ++itNextVertex;

    if (itNextVertex == std::end(m_VertexMap)) return;

    auto CurEdge = boost::edge(itVertex->second, itNextVertex->second, m_Graph);
    if (CurEdge.second) continue;

    Log::Write("core") << "Fix orphan basic blocks at " << itVertex->first.ToString() << " and " << itNextVertex->first.ToString() << LogEnd;
    AddBasicBlockEdge(BasicBlockEdgeProperties(BasicBlockEdgeProperties::Next), itVertex->first, itNextVertex->first);
  }
}

void ControlFlowGraph::Dump(std::string const& rFileName)
{
  boost::write_graphviz(std::ofstream(rFileName), m_Graph, prop_writer<Type>(m_Graph));
}

void ControlFlowGraph::Dump(std::string const& rFileName, Database const& rDb)
{
  boost::write_graphviz(std::ofstream(rFileName), m_Graph, prop_writer_db<Type>(m_Graph, rDb));
}