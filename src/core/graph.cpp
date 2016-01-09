#include "medusa/graph.hpp"
#include "medusa/log.hpp"

namespace medusa
{
  Graph::VertexProperties::VertexProperties(void)
    : m_Flags()
  {

  }

  Graph::VertexProperties::VertexProperties(Address::Vector const& rAddrs)
    : m_Addresses(rAddrs), m_Flags()
  {

  }

  Graph::VertexProperties& Graph::VertexProperties::operator=(VertexProperties const& rVtxProp)
  {
    m_Addresses = rVtxProp.m_Addresses;
    m_Flags = rVtxProp.m_Flags;
    return *this;
  }

  bool Graph::VertexProperties::GetFirstAddress(Address& rAddr) const
  {
    if (m_Addresses.empty())
      return false;
    rAddr = m_Addresses.front();
    return true;
  }

  bool Graph::VertexProperties::GetLastAddress(Address& rAddr) const
  {
    if (m_Addresses.empty())
      return false;
    rAddr = m_Addresses.back();
    return true;
  }

  bool Graph::VertexProperties::Contains(Address const& rAddr) const
  {
    return std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr) != std::end(m_Addresses);
  }

  bool Graph::VertexProperties::Split(Address const& rAddr, Address::Vector& rSplittedAddrs)
  {
    if (m_Addresses.empty())
      return false;
    if (m_Addresses.front() == rAddr)
      return false;
    if (m_Addresses.size() < 2)
      return false;

    auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
    if (itAddr == std::end(m_Addresses))
      return false;

    // LATER(wisk): optimize this if possible
    rSplittedAddrs = Address::Vector{ itAddr, std::end(m_Addresses) };
    m_Addresses.erase(itAddr, std::end(m_Addresses));

    return true;
  }

  bool Graph::VertexProperties::GetPreviousAddress(Address const& rAddr, Address& rPrevAddr) const
  {
    auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
    if (itAddr == std::end(m_Addresses))
      return false;
    if (itAddr == std::begin(m_Addresses))
      return false;
    --itAddr;
    rPrevAddr = *itAddr;
    return true;
  }

  bool Graph::VertexProperties::GetNextAddress(Address const& rAddr, Address& rNextAddr) const
  {
    auto itAddr = std::find(std::begin(m_Addresses), std::end(m_Addresses), rAddr);
    if (itAddr == std::end(m_Addresses))
      return false;
    ++itAddr;
    if (itAddr == std::end(m_Addresses))
      return false;
    rNextAddr = *itAddr;
    return true;
  }

  void Graph::AddVertex(Graph::VertexProperties const& rVertex)
  {
    Address FirstAddr;

    if (!rVertex.GetFirstAddress(FirstAddr))
      return;

    if (m_VertexMap.find(FirstAddr) != std::end(m_VertexMap))
      return;
    m_VertexMap[FirstAddr] = boost::add_vertex(rVertex, m_Graph);
  }

  bool Graph::AddEdge(Graph::EdgeProperties const& rEdge, Address const& rSrcAddr, Address const& rDstAddr)
  {
    VertexDescriptor SrcDesc, DstDesc;
    if (!FindVertex(rSrcAddr, SrcDesc) || !FindVertex(rDstAddr, DstDesc))
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

  bool Graph::FindVertex(Address const& rAddr, Graph::VertexDescriptor& rVtxDesc) const
  {
    for (auto const& rVtxPair : m_VertexMap)
    {
      if (m_Graph[rVtxPair.second].Contains(rAddr))
      {
        rVtxDesc = rVtxPair.second;
        return true;
      }
    }
    return false;
  }

  bool Graph::FindVertexConnections(Address const& rSrcVtxAddr, Address::Vector& rDstVstAddrs) const
  {
    auto itVtxPair = m_VertexMap.find(rSrcVtxAddr);

    VertexDescriptor VtxDesc;
    if (itVtxPair != std::end(m_VertexMap))
    {
      VtxDesc = itVtxPair->second;
    }
    else
    {
      if (!FindVertex(rSrcVtxAddr, VtxDesc))
        return false;
    }

    auto OutEdgesPair = boost::out_edges(VtxDesc, m_Graph);
    auto itCur = OutEdgesPair.first, itEnd = OutEdgesPair.second;
    for (; itCur != itEnd; ++itCur)
    {
      auto const& rDstVtx = m_Graph[boost::target(*itCur, m_Graph)];
      Address VtxAddr;
      if (!rDstVtx.GetFirstAddress(VtxAddr))
        return false;
      rDstVstAddrs.push_back(VtxAddr);
    }

    return true;
  }

  bool Graph::SplitVertex(Address const& rDstAddr, Address const& rSrcAddr, Graph::EdgeProperties::Type EdgeType)
  {
    for (auto const& rVtxPair : m_VertexMap)
    {
      if (m_Graph[rVtxPair.second].Contains(rDstAddr))
      {
        Address::Vector NewVtxAddrs;

        if (!m_Graph[rVtxPair.second].Split(rDstAddr, NewVtxAddrs))
          break;

        if (NewVtxAddrs.empty())
        {
          Log::Write("core") << "empty basic block" << LogEnd;
          return false;
        }

        AddVertex(VertexProperties(NewVtxAddrs));
        AddEdge(EdgeProperties(EdgeProperties::Unconditional), rVtxPair.second, rDstAddr);
        return true;
      }
    }
    return false;
  }

  void Graph::Finalize(void)
  {
    // TODO(wisk):
  }

  void Graph::ForEachVertex(std::function<void(Graph::VertexProperties const&)> Predicat) const
  {
    auto itRange = boost::vertices(m_Graph);
    for (auto it = itRange.first; it != itRange.second; ++it)
      Predicat(m_Graph[*it]);
  }

  void Graph::ForEachVertex(std::function<bool(Graph::VertexProperties const&)> Predicat, bool& rResult) const
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

  void Graph::ForEachEdgeIterator(std::function<void(Graph::EdgeIterator const&)> Predicat)
  {
    //auto itRange = boost::edges(m_Graph);
    //for (auto it = itRange.first; it != itRange.second; ++it)
    //  Predicat(it);
  }

  void Graph::ForEachVertexIterator(std::function<void(Graph::VertexIterator const&)> Predicat) const
  {
    auto itRange = boost::vertices(m_Graph);
    for (auto it = itRange.first; it != itRange.second; ++it)
      Predicat(it);
  }

  void Graph::ForEachVertexIterator(std::function<bool(Graph::VertexIterator const&)> Predicat, bool& rResult) const
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

  void Graph::ForEachAddress(std::function<void(Address const&)> Predicat) const
  {
    auto itRange = boost::vertices(m_Graph);
    for (auto it = itRange.first; it != itRange.second; ++it)
    {
      auto const& rVtxAddrs = m_Graph[*it].GetAddresses();
      for (auto itAddr = std::begin(rVtxAddrs); itAddr != std::end(rVtxAddrs); ++itAddr)
        Predicat(*itAddr);
    }
  }

  void Graph::ForEachAddress(std::function<bool(Address const&)> Predicat, bool& rResult) const
  {
    rResult = false;
    auto itRange = boost::vertices(m_Graph);
    for (auto it = itRange.first; it != itRange.second; ++it)
    {
      auto const& rVtxAddrs = m_Graph[*it].GetAddresses();
      for (auto itAddr = std::begin(rVtxAddrs); itAddr != std::end(rVtxAddrs); ++itAddr)
        if (Predicat(*itAddr) == true)
        {
          rResult = true;
          return;
        }
    }
  }

  bool Graph::GetPreviousAddress(Address const& rAddr, Address::List& rPrevAddr) const
  {
    auto itRange = boost::vertices(m_Graph);
    for (auto it = itRange.first; it != itRange.second; ++it)
    {
      auto Vtx = m_Graph[*it];
      if (!Vtx.Contains(rAddr))
        continue;

      Address FirstAddr;
      if (!Vtx.GetFirstAddress(FirstAddr))
        continue;

      if (FirstAddr == rAddr)
      {
        auto AdjVtxs = boost::in_edges(*it, m_Graph);
        for (auto itInVtx = AdjVtxs.first; itInVtx != AdjVtxs.second; ++itInVtx)
        {
          auto SrcVtxId = boost::source(*itInVtx, m_Graph);
          auto SrcVtx = m_Graph[SrcVtxId];

          Address TgtLastAddr;
          if (!SrcVtx.GetLastAddress(TgtLastAddr))
            return false;
          rPrevAddr.push_back(TgtLastAddr);
        }
        return true;
      }

      else
      {
        Address PrevAddr;
        if (!Vtx.GetPreviousAddress(rAddr, PrevAddr))
          return false;
        rPrevAddr.push_back(PrevAddr);
        return true;
      }
    }
    return false;
  }

  bool Graph::GetNextAddress(Address const& rAddr, Address::List& rNextAddr) const
  {
    auto itRange = boost::vertices(m_Graph);
    for (auto it = itRange.first; it != itRange.second; ++it)
    {
      auto Vtx = m_Graph[*it];
      if (!Vtx.Contains(rAddr))
        continue;

      Address FirstAddr;
      if (!Vtx.GetFirstAddress(FirstAddr))
        continue;

      if (FirstAddr == rAddr)
      {
        auto AdjVtxs = boost::out_edges(*it, m_Graph);
        for (auto itOutVtx = AdjVtxs.first; itOutVtx != AdjVtxs.second; ++itOutVtx)
        {
          auto TgtVtxId = boost::target(*itOutVtx, m_Graph);
          auto TgtVtx = m_Graph[TgtVtxId];

          Address TgtFirstAddr;
          if (!TgtVtx.GetFirstAddress(TgtFirstAddr))
            return false;
          rNextAddr.push_back(TgtFirstAddr);
        }
        return true;
      }

      else
      {
        Address NextAddr;
        if (!Vtx.GetNextAddress(rAddr, NextAddr))
          return false;
        rNextAddr.push_back(NextAddr);
        return true;
      }
    }
    return false;
  }

  bool Graph::Contains(Address const& rAddress) const
  {
    bool Result;
    ForEachAddress([&rAddress](Address const& rAddr){ return rAddr == rAddress; }, Result);
    return Result;
  }
}
