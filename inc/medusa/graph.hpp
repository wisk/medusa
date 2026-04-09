#ifndef MEDUSA_GRAPH_HPP
#define MEDUSA_GRAPH_HPP

#include "medusa/export.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"

#include <boost/graph/adjacency_list.hpp>

namespace medusa
{

  class MEDUSA_EXPORT Graph
  {
  public:

    using SPType = std::shared_ptr<Graph>;

    struct MEDUSA_EXPORT VertexProperties
    {
      enum Type
      {
        None   = 0,
        Head   = 1 << 0,
        End    = 1 << 1,
        InLoop = 1 << 2,
      };

    public:

      VertexProperties();
      explicit VertexProperties(Address::Vector const& rAddrs);

      VertexProperties& operator=(VertexProperties const& rVtxProp);

      Address::Vector const& GetAddresses()              const { return m_Addresses; }
      bool                   GetFirstAddress(Address& rAddr) const;
      bool                   GetLastAddress(Address& rAddr)  const;
      size_t                 GetSize()                   const { return m_Addresses.size(); }
      bool                   Contains(Address const& rAddr)  const;
      bool                   Split(Address const& rAddr, Address::Vector& rSplittedAddrs, Address* pPrevAddr = nullptr);

      bool                   GetPreviousAddress(Address const& rAddr, Address& rPrevAddr) const;
      bool                   GetNextAddress(Address const& rAddr, Address& rNextAddr) const;

      bool                   IsHead()   const { return m_Flags & Head   ? true : false; }
      bool                   IsEnd()    const { return m_Flags & End    ? true : false; }
      bool                   IsInLoop() const { return m_Flags & InLoop ? true : false; }

      void                   MarkAsHead()   { m_Flags |= Head;   }
      void                   MarkAsEnd()    { m_Flags |= End;    }
      void                   MarkAsInLoop() { m_Flags |= InLoop; }

      Address::Vector m_Addresses;
      u32 m_Flags;
    };

    struct MEDUSA_EXPORT EdgeProperties
    {
    public:
      enum Type
      {
        Unknown,
        Unconditional,
        True,
        False,
        Next,
        Multiple,
      };

      EdgeProperties(Type Type = Unknown) : m_Type(Type) {}

      Type GetType() const { return static_cast<Type>(m_Type); }

      u32 m_Type;
    };

    class MEDUSA_EXPORT GraphProperties
    {
    public:

    };

    using Type = boost::adjacency_list <
      boost::vecS, boost::vecS, boost::bidirectionalS,
      VertexProperties,
      EdgeProperties,
      GraphProperties
    >;

    Type      & operator ()()       { return m_Graph; }
    Type const& operator ()() const { return m_Graph; }

    using VertexDescriptor = boost::graph_traits<Type>::vertex_descriptor;
    using EdgeDescriptor   = boost::graph_traits<Type>::edge_descriptor;

    using VertexIterator   = boost::graph_traits<Type>::vertex_iterator;
    using EdgeIterator     = boost::graph_traits<Type>::edge_descriptor;

    bool ToGraphViz(std::string& rGraphVizCode) const;
    static bool FromGraphViz(Graph& rGraph, std::string const& rGraphVizCode);

    void AddVertex(VertexProperties const& rVertex);
    bool AddEdge(EdgeProperties const& rEdge, Address const& rSrcAddr, Address const& rDstAddr);
    bool FindVertex(Address const& rAddr, VertexDescriptor& rVtxDesc) const;
    bool FindVertexConnections(Address const& rSrcVtxAddr, Address::Vector& rDstVstAddrs) const;
    bool SplitVertex(Address const& rDstAddr, Address const& rSrcAddr, EdgeProperties::Type EdgeType, Address* pPrevAddr = nullptr);

    void ForEachVertex(std::function<void(VertexProperties const&)> Predicat) const;
    void ForEachVertex(std::function<bool(VertexProperties const&)> Predicat, bool& rResult) const;

    void ForEachEdgeIterator(std::function<void(EdgeIterator const&)> Predicat);

    void ForEachVertexIterator(std::function<void(VertexIterator const&)> Predicat) const;
    void ForEachVertexIterator(std::function<bool(VertexIterator const&)> Predicat, bool& rResult) const;

    void ForEachAddress(std::function<void(Address const&)> Predicat) const;
    void ForEachAddress(std::function<bool(Address const&)> Predicat, bool& rResult) const;

    bool GetPreviousAddress(Address const& rAddr, Address::Vector& rPrevAddr) const;
    bool GetNextAddress(Address const& rAddr, Address::Vector& rNextAddr) const;

    bool Contains(Address const& rAddress) const;

  private:
    using VertexMap = std::map<Address, VertexDescriptor>;

    Type            m_Graph;
    VertexMap       m_VertexMap;
  };
}

#endif // !MEDUSA_GRAPH_HPP