#ifndef __MEDUSA_CONTROL_FLOW_GRAPH_HPP__
#define __MEDUSA_CONTROL_FLOW_GRAPH_HPP__

#include "medusa/namespace.hpp"
#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/basic_block.hpp"
#include "medusa/database.hpp"

#include <boost/graph/adjacency_list.hpp>

MEDUSA_NAMESPACE_BEGIN

class Medusa_EXPORT ControlFlowGraph
{
  struct ControlFlowGraphProperties{};

public:
  typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS,
    boost::property<boost::vertex_bundle_t, BasicBlockVertexProperties>,
    boost::property<boost::edge_bundle_t,   BasicBlockEdgeProperties  >,
    boost::property<boost::graph_bundle_t,  ControlFlowGraphProperties>
  > Type;

  typedef boost::graph_traits<Type>::vertex_descriptor BasicBlockVertexDescriptor;
  typedef boost::graph_traits<Type>::edge_descriptor   BasicBlockEdgeDescriptor;

  ControlFlowGraph(void);

  void AddBasicBlockVertex(BasicBlockVertexProperties const& rVertex);
  bool AddBasicBlockEdge(BasicBlockEdgeProperties const& rEdge, Address const& rSrcAddr, Address const& rDstAddr);

  bool FindBasicBlock(Address const& rAddr, BasicBlockVertexDescriptor& BasicBlckDesc);

  bool SplitBasicBlock(Address const& rDstAddr, Address const& rSrcAddr, BasicBlockEdgeProperties::Type Type);

  void Finalize(Database const& rDb);

  void Dump(std::string const& rFilename);
  void Dump(std::string const& rFilename, Database const& rDb);

private:
  typedef std::map<Address, BasicBlockVertexDescriptor> VertexMap;

  Type      m_Graph;
  VertexMap m_VertexMap;
};

MEDUSA_NAMESPACE_END

#endif // !__MEDUSA_CONTROL_FLOW_GRAPH_HPP__