#ifndef MEDUSA_DOCUMENT_HPP
#define MEDUSA_DOCUMENT_HPP

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
#include "medusa/detail.hpp"
#include "medusa/database.hpp"

#include <set>
#include <mutex>
#include <boost/bimap.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

MEDUSA_NAMESPACE_BEGIN

//! Document handles cell, multicell, xref, label and memory area.
class MEDUSA_EXPORT Document
{
  Document(Document const&) = delete;
  Document& operator=(Document const&) = delete;

public:
  typedef boost::signals2::connection ConnectionType;

  class MEDUSA_EXPORT Subscriber
  {
    friend class Document;

  public:
    enum Type
    {
      Quit              = 1 << 0,
      DocumentUpdated   = 1 << 1,
      MemoryAreaUpdated = 1 << 2,
      AddressUpdated    = 1 << 3,
      LabelUpdated      = 1 << 4,
      TaskUpdated       = 1 << 5,
    };

    virtual ~Subscriber(void)
    {
      m_QuitConnection.disconnect();
      m_DocumentUpdatedConnection.disconnect();
      m_MemoryAreaUpdatedConnection.disconnect();
      m_AddressUpdatedConnection.disconnect();
      m_LabelUpdatedConnection.disconnect();
      m_TaskUpdatedConnection.disconnect();
    }

  private:
    typedef boost::signals2::signal<void (void)>                                                       QuitSignalType;
    typedef boost::signals2::signal<void (void)>                                                       DocumentUpdatedSignalType;
    typedef boost::signals2::signal<void (MemoryArea const& rMemArea, bool Removed)>                   MemoryAreaUpdatedSignalType;
    typedef boost::signals2::signal<void (Address::List const& rAddressList)>                          AddressUpdatedSignalType;
    typedef boost::signals2::signal<void (Address const& rAddress, Label const& rLabel, bool Removed)> LabelUpdatedSignalType;
    typedef boost::signals2::signal<void (std::string const& rTaskName, u8 Status)>                    TaskUpdatedSignalType;

    typedef QuitSignalType::slot_type              QuitSlotType;
    typedef DocumentUpdatedSignalType::slot_type   DocumentUpdatedSlotType;
    typedef MemoryAreaUpdatedSignalType::slot_type MemoryAreaUpdatedSlotType;
    typedef AddressUpdatedSignalType::slot_type    AddressUpdatedSlotType;
    typedef LabelUpdatedSignalType::slot_type      LabelUpdatedSlotType;
    typedef TaskUpdatedSignalType                  TaskUpdatedSlotType;

    Document::ConnectionType m_QuitConnection;
    Document::ConnectionType m_DocumentUpdatedConnection;
    Document::ConnectionType m_MemoryAreaUpdatedConnection;
    Document::ConnectionType m_AddressUpdatedConnection;
    Document::ConnectionType m_LabelUpdatedConnection;
    Document::ConnectionType m_TaskUpdatedConnection;

  public:
    virtual void OnQuit(void) {}
    virtual void OnDocumentUpdated(void) {}
    virtual void OnMemoryAreaUpdated(MemoryArea const& rMemArea, bool Removed) {}
    virtual void OnAddressUpdated(Address::List const& rAddressList) {}
    virtual void OnLabelUpdated(Address const& rAddress, Label const& rLabel, bool Removed) {}
    virtual void OnTaskUpdated(std::string const& rTaskName, u8 Status) {}
  };

  Document(void);
  ~Document(void);

  // Database

  bool Open(Database::SPType spDb);
  bool Flush(void);
  bool Close(void);

  // Subscriber

  void Connect(u32 Type, Subscriber* pSubscriber);

  // Image base
  virtual bool GetImageBase(ImageBaseType& rImageBase) const;
  virtual bool SetImageBase(ImageBaseType ImageBase);

  // Memory Area
  bool GetMemoryArea(Address const& rAddress, MemoryArea& rMemArea) const;
  void ForEachMemoryArea(Database::MemoryAreaCallback Callback) const;
  bool AddMemoryArea(MemoryArea const& rMemArea);
  bool RemoveMemoryArea(MemoryArea const& rMemArea);
  bool MoveMemoryArea(MemoryArea const& rMemArea, Address const& rBaseAddress);

  // Binary Stream

  BinaryStream& GetBinaryStream(void) const { return m_spDatabase->GetBinaryStream(); }

  // Label

        //! This method returns a label by its address.
  Label GetLabelFromAddress(Address const& rAddr) const;

          //! This method returns the address of rLabel.
  Address GetAddressFromLabelName(std::string const& rLabel) const;

       //! This method add a new label.
  bool AddLabel(Address const& rAddr, Label const& rLabel, bool Force = true);
  bool RemoveLabel(Address const& rAddr);
  void ForEachLabel(Database::LabelCallback Callback) const;

  // CrossRef

  bool AddCrossReference(Address const& rTo, Address const& rFrom);
  bool RemoveCrossReference(Address const& rFrom);
  bool RemoveCrossReferences(void);

  bool HasCrossReferenceFrom(Address const& rTo) const;
  bool GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const;

  bool HasCrossReferenceTo(Address const& rFrom) const;
  bool GetCrossReferenceTo(Address const& rFrom, Address::List& rToList) const;

  // Comment
  bool GetComment(Address const& rAddress, std::string& rComment) const;
  bool SetComment(Address const& rAddress, std::string const& rComment);

  // Cell
                     /*! This method returns a cell by its address.
                      * \return A pointer to a cell if the rAddr is valid, nullptr otherwise.
                      */
  Cell::SPType        GetCell(Address const& rAddr);
  Cell::SPType const  GetCell(Address const& rAddr) const;
                     
  u8                  GetCellType(Address const& rAddr) const;
  u8                  GetCellSubType(Address const& rAddr) const;
                     
                      /*! This method adds a new cell.
                       * \param rAddr is the address of the new cell.
                       * \param pCell is the new cell.
                       * \param Force makes the old cell to be deleted.
                       * \return Returns true if the new cell is added, otherwise it returns false.
                       */
  bool                SetCell(Address const& rAddr, Cell::SPType spCell, bool Force = false);
  bool                SetCellWithLabel(Address const& rAddr, Cell::SPType spCell, Label const& rLabel, bool Force = false);
  bool                SetArchitecture(Address const& rAddr, Tag TagArch, u8 Mode, Database::SetArchitectureModeType SetArchMode);
                     
  bool                DeleteCell(Address const& rAddr);

  // Value

  /*! Change size of object Value
   *  \param rValueAddr is the address of value
   *  \param NewValueSize must be 8 or 16 or 32 or 64
   *  \param Force makes this method to erase others cells if needed
   */
  bool ChangeValueSize(Address const& rValueAddr, u8 NewValueSize, bool Force = false);

  // String
  bool MakeString(Address const& rAddress, u8 StringType, u16 StringLength = -1, bool Force = false);

  // MultiCell

                   //! \return Returns a pointer to a multicell if rAddr is valid, otherwise nullptr.
  MultiCell::SPType GetMultiCell(Address const& rAddr) const;

                   /*! This method adds a new MultiCell.
                    *  \param rAddr is the address of the MultiCell.
                    *  \param pMultiCell is a the new MultiCell.
                    *  \param Force removes the old MultiCell if set.
                    *  \return Returns true if the new multicell is added, otherwise it returns false.
                    */
  bool             SetMultiCell(Address const& rAddr, MultiCell::SPType spMultiCell, bool Force = true);

                   /*! This method returns all couple Address and MultiCell
                   */
  // Detail

  bool GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const;
  bool SetValueDetail(Id ConstId, ValueDetail const& rConstDtl);

  bool GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const;
  bool SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl);

  bool GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const;
  bool SetStructureDetail(Id StructId, StructureDetail const& rStructDtl);

  bool RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const;
  bool BindDetailId(Address const& rAddress, u8 Index, Id DtlId);
  bool UnbindDetailId(Address const& rAddress, u8 Index);

  // Address

  bool GetDefaultAddressingType(Address::Type& rAddressType) const;
  bool SetDefaultAddressingType(Address::Type AddressType);
          /*! This method makes an Address.
           *  \param Base is the base address.
           *  \param Offset is the offset address.
           *  \return Returns a shared pointer to a new Address with correct information if base and offset are associated to a memory area, otherwise it returns an empty shared pointer Address.
           */
  Address MakeAddress(BaseType Base, OffsetType Offset) const;

  bool    GetPreviousAddressInHistory(Address& rAddress);
  bool    GetNextAddressInHistory(Address& rAddress);
  void    InsertAddressInHistory(Address const& rAddress);

          /*! This method converts an Address to a offset (memory area relative).
           * \return Returns true if the conversion is possible, otherwise it returns false.
           */
  bool    ConvertAddressToFileOffset(Address const& rAddr, OffsetType& rFileOffset) const;

  bool    ConvertAddressToPosition(Address const& rAddr, u32& rPosition) const;
  bool    ConvertPositionToAddress(u32 Position, Address& rAddr) const;

  // Implemented possibilities:
  // - Logical Address → Virtual Address → (Relative Address → Physical address)
  // - (Physical Address → Relative Address) → Virtual Address → Logical Address (might not normalized)
  // - Architecture Address → ? (depends on architecture)
  // - ? → Architecture Address (might not be normalized)
  bool    TranslateAddress(Address const& rAddress, Address::Type ToConvert, Address& rTranslatedAddress) const;
  Address GetStartAddress(void) const;
  Address GetFirstAddress(void) const;
  Address GetLastAddress(void)  const;
  u32     GetNumberOfAddress(void) const;

  bool    MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const;
  bool    GetPreviousAddress(Address const& rAddress, Address& rPreviousAddress) const;
  bool    GetNextAddress(Address const& rAddress, Address& rNextAddress) const;
  bool    GetNearestAddress(Address const& rAddress, Address& rNearestAddress) const;

  // Helper

  bool ContainsData(Address const& rAddress) const;
  bool ContainsCode(Address const& rAddress) const;
  bool ContainsUnknown(Address const& rAddress) const;

  Tag            GetArchitectureTag(Address const& rAddress) const;
  std::list<Tag> GetArchitectureTags(void) const;
  u8             GetMode(Address const& rAddress) const;

  // Operating system

  std::string GetOperatingSystemName(void) const;

private:
  void RemoveLabelIfNeeded(Address const& rAddr);

  bool _ApplyStructure(Address const& rAddr, StructureDetail const& rStructDtl);
  bool _ApplyTypedValue(Address const& rParentAddr, Address const& rTpValAddr, TypedValueDetail const& rTpValDtl);
  bool _ApplyType(Address const& rAddr, TypeDetail::SPType const& rspTpDtl);
  bool _ApplyValue(Address const& rAddr, ValueDetail const& rValDtl);

  typedef std::mutex MutexType;

  Database::SPType                        m_spDatabase;
  mutable MutexType                       m_CellMutex;

  std::deque<Address>                     m_AddressHistory;
  std::deque<Address>::size_type          m_AddressHistoryIndex;
  MutexType                               m_AddressHistoryMutex;

  Subscriber::QuitSignalType              m_QuitSignal;
  Subscriber::DocumentUpdatedSignalType   m_DocumentUpdatedSignal;
  Subscriber::MemoryAreaUpdatedSignalType m_MemoryAreaUpdatedSignal;
  Subscriber::AddressUpdatedSignalType    m_AddressUpdatedSignal;
  Subscriber::LabelUpdatedSignalType      m_LabelUpdatedSignal;
  Subscriber::TaskUpdatedSignalType       m_TaskUpdatedSignal;
};

MEDUSA_NAMESPACE_END

#endif // MEDUSA_DOCUMENT_HPP
