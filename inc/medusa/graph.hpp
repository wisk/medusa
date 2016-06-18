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

    typedef std::shared_ptr<Graph> SPType;

    class MEDUSA_EXPORT VertexProperties
    {
      enum Type
      {
        None   = 0,
        Head   = 1 << 0,
        End    = 1 << 1,
        InLoop = 1 << 2,
      };

    public:

      VertexProperties(void);
      VertexProperties(Address::Vector const& rAddrs);

      VertexProperties& operator=(VertexProperties const& rVtxProp);

      Address::Vector const& GetAddresses(void)              const { return m_Addresses; }
      bool                   GetFirstAddress(Address& rAddr) const;
      bool                   GetLastAddress(Address& rAddr)  const;
      size_t                 GetSize(void)                   const { return m_Addresses.size(); }
      bool                   Contains(Address const& rAddr)  const;
      bool                   Split(Address const& rAddr, Address::Vector& rSplittedAddrs, Address* pPrevAddr = nullptr);

      bool                   GetPreviousAddress(Address const& rAddr, Address& rPrevAddr) const;
      bool                   GetNextAddress(Address const& rAddr, Address& rNextAddr) const;

      bool                   IsHead(void)   const { return m_Flags & Head   ? true : false; }
      bool                   IsEnd(void)    const { return m_Flags & End    ? true : false; }
      bool                   IsInLoop(void) const { return m_Flags & InLoop ? true : false; }

      void                   MarkAsHead(void)   { m_Flags |= Head;   }
      void                   MarkAsEnd(void)    { m_Flags |= End;    }
      void                   MarkAsInLoop(void) { m_Flags |= InLoop; }

    private:
      Address::Vector m_Addresses;
      u32 m_Flags;
    };

    class MEDUSA_EXPORT EdgeProperties
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

      Type GetType(void) const { return m_Type; }

    private:
      Type m_Type;
    };

    class MEDUSA_EXPORT GraphProperties
    {
    public:

    };

    typedef boost::adjacency_list <
      boost::vecS, boost::vecS, boost::bidirectionalS,
      VertexProperties,
      EdgeProperties,
      GraphProperties
    > Type;

    Type      & operator ()(void)       { return m_Graph; }
    Type const& operator ()(void) const { return m_Graph; }

    typedef boost::graph_traits<Type>::vertex_descriptor VertexDescriptor;
    typedef boost::graph_traits<Type>::edge_descriptor   EdgeDescriptor;

    typedef boost::graph_traits<Type>::vertex_iterator   VertexIterator;
    typedef boost::graph_traits<Type>::edge_descriptor   EdgeIterator;

    void AddVertex(VertexProperties const& rVertex);
    bool AddEdge(EdgeProperties const& rEdge, Address const& rSrcAddr, Address const& rDstAddr);
    bool FindVertex(Address const& rAddr, VertexDescriptor& rVtxDesc) const;
    bool FindVertexConnections(Address const& rSrcVtxAddr, Address::Vector& rDstVstAddrs) const;
    bool SplitVertex(Address const& rDstAddr, Address const& rSrcAddr, EdgeProperties::Type EdgeType, Address* pPrevAddr = nullptr);
    void Finalize(void);

    void ForEachVertex(std::function<void(VertexProperties const&)> Predicat) const;
    void ForEachVertex(std::function<bool(VertexProperties const&)> Predicat, bool& rResult) const;

    void ForEachEdgeIterator(std::function<void(EdgeIterator const&)> Predicat);

    void ForEachVertexIterator(std::function<void(VertexIterator const&)> Predicat) const;
    void ForEachVertexIterator(std::function<bool(VertexIterator const&)> Predicat, bool& rResult) const;

    void ForEachAddress(std::function<void(Address const&)> Predicat) const;
    void ForEachAddress(std::function<bool(Address const&)> Predicat, bool& rResult) const;

    bool GetPreviousAddress(Address const& rAddr, Address::List& rPrevAddr) const;
    bool GetNextAddress(Address const& rAddr, Address::List& rNextAddr) const;

    bool Contains(Address const& rAddress) const;

  private:
    typedef std::map<Address, VertexDescriptor> VertexMap;

    Type            m_Graph;
    VertexMap       m_VertexMap;
  };
}

#endif // !MEDUSA_GRAPH_HPP