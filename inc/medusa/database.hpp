#ifndef _MEDUSA_DATABASE_
#define _MEDUSA_DATABASE_

#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/export.hpp"
#include "medusa/cell.hpp"
#include "medusa/multicell.hpp"
#include "medusa/memory_area.hpp"
#include "medusa/binary_stream.hpp"
#include "medusa/xref.hpp"
#include "medusa/label.hpp"
#include "medusa/event_queue.hpp"
#include "medusa/view.hpp"

#include <set>
#include <boost/bimap.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Database handles cell, multicell, xref, label and memory area.
class Medusa_EXPORT Database
{
public:
  typedef std::set<MemoryArea*, MemoryArea::Compare> TMemoryAreas;
  typedef TMemoryAreas::iterator        TIterator;
  typedef TMemoryAreas::const_iterator  TConstIterator;
  typedef boost::bimap<Address, Label>  TLabelMap;

  View const& GetView(void) const
  {
    return m_View;
  }

  View & GetView(void)
  {
    return m_View;
  }

                                /*!
                                 * The constructor needs a FileBinaryStream
                                 * \param rBinaryStream must contains the disassembled file.
                                 */
                                Database(FileBinaryStream const& rBinaryStream);
                                ~Database(void);

                                //! This method remove all memory areas.
  void                          RemoveAll(void);

  // Memory Area

                                /*! This method adds a new memory area.
                                 * \param pMemoryArea is the added memory area.
                                 */
  void                          AddMemoryArea(MemoryArea* pMemoryArea);

                                //! This method returns all memory areas.
  TMemoryAreas&                 GetMemoryAreas(void)        { return m_MemoryAreas; }
  TMemoryAreas const&           GetMemoryAreas(void)  const { return m_MemoryAreas; }

                                /*! This method return a specific memory area.
                                 * \param Addr is a address contained in the returned memory area.
                                 */
  MemoryArea*                   GetMemoryArea(Address const& Addr);
  MemoryArea const*             GetMemoryArea(Address const& Addr) const;

  // Binary Stream
  FileBinaryStream const&       GetFileBinaryStream(void) const { return m_rBinaryStream; }

  // Label
                                //! This method returns a label by its address.
  Label                         GetLabelFromAddress(Address const& rAddr) const;

                                //! This method update a label by its address.
  void                          SetLabelToAddress(Address const& rAddr, Label const& rLabel);

                                //! This method returns the address of rLabel.
  Address                       GetAddressFromLabelName(std::string const& rLabel) const;

                                //! This method add a new label.
  void                          AddLabel(Address const& rAddr, Label const& rLabel);

                                //! This method returns all labels.
  TLabelMap const&              GetLabels(void) const { return m_LabelMap; }

  // Xref

                                //! This method returns all cross-references.
  XRefs&                        GetXRefs(void) { return m_XRefs; }
  XRefs const&                  GetXRefs(void) const { return m_XRefs; }

  // Cell

                                /*! This method returns a cell by its address.
                                 * \return A pointer to a cell if the rAddr is valid, NULL otherwise.
                                 */
  Cell*                         RetrieveCell(Address const& rAddr);
  Cell const*                   RetrieveCell(Address const& rAddr) const;

                                /*! This method adds a new cell.
                                 * \param rAddr is the address of the new cell.
                                 * \param pCell is the new cell.
                                 * \param Force makes the old cell to be deleted.
                                 * \param Safe makes this method to avoid "cell overlay" but it's slower.
                                 * \return Returns true if the new cell is added, otherwise it returns false.
                                 */
  bool                          InsertCell(Address const& rAddr, Cell* pCell, bool Force = false, bool Safe = true);

  void                          UpdateCell(Address const& rAddr, Cell* pCell);

                                //! Returns true if rAddr is contained in the Database.
  bool                          IsPresent(Address const& rAddr) const;
  bool                          IsPresent(Address::SharedPtr spAddr) const { return IsPresent(*spAddr.get()); }

                                //! Returns true if rAddr contains code.
  bool                          ContainsCode(Address const& rAddr) const
  {
    Cell const* pCell = RetrieveCell(rAddr);
    if (pCell == NULL) return false;
    return pCell->GetType() == CellData::InstructionType;
  }

                                //! Returns true if rAddr contains data.
  bool                          ContainsData(Address const& rAddr) const
  {
    Cell const* pCell = RetrieveCell(rAddr);
    if (pCell == NULL) return false;
    return pCell->GetType() == CellData::ValueType;
  }

  // Value

                                /*! Change size of object Value
                                 *  \param rValueAddr is the address of value
                                 *  \param NewValueSize must be 8 or 16 or 32 or 64
                                 *  \param Force makes this method to erase others cells if needed
                                 */
  bool                          ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force = false);

  // String
  bool                          MakeAsciiString(Address const& rAddr);
  bool                          MakeWindowsString(Address const& rAddr);

  // MultiCell

                                //! \return Returns a pointer to a multicell if rAddr is valid, otherwise NULL.
  MultiCell*                    RetrieveMultiCell(Address const& rAddr);
  MultiCell const*              RetrieveMultiCell(Address const& rAddr) const;

                                /*! This method adds a new MultiCell.
                                 *  \param rAddr is the address of the MultiCell.
                                 *  \param pMultiCell is a the new MultiCell.
                                 *  \param Force removes the old MultiCell if set.
                                 *  \return Returns true if the new multicell is added, otherwise it returns false.
                                 */
  bool                          InsertMultiCell(Address const& rAddr, MultiCell* pMultiCell, bool Force = true);

                                /*! This method returns all couple Address and MultiCell
                                */
  MultiCell::Map const&         GetMultiCells(void) const { return m_MultiCells; }

  // Address

                                /*! This method makes an Address.
                                 *  \param Base is the base address.
                                 *  \param Offset is the offset address.
                                 *  \return Returns a shared pointer to a new Address with correct information if base and offset are associated to a memory area, otherwise it returns an empty shared pointer Address.
                                 */
  Address                       MakeAddress(TBase Base, TOffset Offset) const
  {
    MemoryArea const* ma = GetMemoryArea(Address(Base, Offset));
    if (ma == NULL)
      return Address();
    return ma->MakeAddress(Offset);
  }

                                /*! This method translates an Address to a offset (file relative).
                                 * \return Returns true if the translation is possible, otherwise it returns false.
                                 */
  bool                          Translate(Address const& rAddr, TOffset& rRawOffset) const;

                                /*! This method converts an Address to a offset (memory area relative).
                                 * \return Returns true if the conversion is possible, otherwise it returns false.
                                 */
  bool                          Convert(Address const& rAddr, TOffset& rMemAreaOffset) const;

  // Data

  // Event
  void                          StartsEventHandling(EventHandler* pEvtHdl);
  void                          StopsEventHandling(void);

  // Iterator
  TIterator                     Begin(void)       { return m_MemoryAreas.begin(); }
  TIterator                     End(void)         { return m_MemoryAreas.end();   }

  TConstIterator                Begin(void) const { return m_MemoryAreas.begin(); }
  TConstIterator                End(void)   const { return m_MemoryAreas.end();   }

  bool                          Read(Address const& rAddress, void* pBuffer, u32 Size) const;
  bool                          Write(Address const& rAddress, void const* pBuffer, u32 Size);

  u16                           GetNumberOfAddress(void) const;
  bool                          NextAddress(Address const& rAddress, Address& rNextAddress) const;

  Database& operator=(Database const& rDatabase)
  {
    if (this == &rDatabase) return *this;

    const_cast<FileBinaryStream&>(m_rBinaryStream) = rDatabase.m_rBinaryStream;
    m_MemoryAreas                                  = rDatabase.m_MemoryAreas;
    m_MultiCells                                   = rDatabase.m_MultiCells;
    m_LabelMap                                     = rDatabase.m_LabelMap;
    m_XRefs                                        = rDatabase.m_XRefs;
    return *this;
  }

  Database(Database const& rDatabase)
    : m_rBinaryStream(rDatabase.m_rBinaryStream)
    , m_MemoryAreas(rDatabase.m_MemoryAreas)
    , m_MultiCells(rDatabase.m_MultiCells)
    , m_LabelMap(rDatabase.m_LabelMap)
    , m_XRefs(rDatabase.m_XRefs)
  {
  }

private:
  void ProcessEventQueue(EventHandler* pEvtHdl);

  typedef boost::mutex          MutexType;

  FileBinaryStream const&       m_rBinaryStream;
  TMemoryAreas                  m_MemoryAreas;
  MultiCell::Map                m_MultiCells;
  TLabelMap                     m_LabelMap;
  XRefs                         m_XRefs;
  EventQueue                    m_EventQueue;
  mutable MutexType             m_MemoryAreaMutex;
  mutable MutexType             m_CellMutex;
  View                          m_View;
  boost::thread                 m_Thread;
};

MEDUSA_NAMESPACE_END

#endif // _MEDUSA_DATABASE_
