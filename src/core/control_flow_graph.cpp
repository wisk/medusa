#include "medusa/control_flow_graph.hpp"
#include <boost/graph/graphviz.hpp>
#include "medusa/address.hpp"

// Workaround from http://stackoverflow.com/questions/9669109/print-a-constified-subgraph-with-write-graphviz
template<typename Graph> struct prop_writer
{
  prop_writer(Graph const& g) : g_(g) {}
  template<typename Vertex> void operator()(std::ostream & out, Vertex v) const
  {
    out << "[shape=box] [label=\"Address: ";
    for (auto itAddr = std::begin(g_[v].GetAddresses()); itAddr != std::end(g_[v].GetAddresses()); ++itAddr)
      out << *itAddr << " ";
    out << "\"]";
  }

  Graph const& g_;
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
      return true;
    }
  }
  return false;
}

void ControlFlowGraph::Dump(std::string const& rFileName)
{
  boost::write_graphviz(std::ofstream(rFileName), m_Graph, prop_writer<Type>(m_Graph));
}