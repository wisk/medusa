#ifndef MEDUSA_CELL_TEXT_HPP
#define MEDUSA_CELL_TEXT_HPP

#include "medusa/export.hpp"
#include "medusa/namespace.hpp"
#include "medusa/types.hpp"
#include "medusa/address.hpp"
#include "medusa/graph.hpp"

#include <mutex>
#include <sstream>
#include <string>
#include <list>
#include <set>
#include <unordered_map>

MEDUSA_NAMESPACE_BEGIN

class Mark
{
public:
  typedef std::list<Mark> List;

  enum Type
  {
    UnknownType,
    UnprintableType,
    MnemonicType,
    MnemonicPrefixType,
    MnemonicSuffixType,
    RegisterType,
    ImmediateType,
    LabelType,
    KeywordType,
    OperatorType,
    CharacterType,
    StringType,
    CommentType,
  };

  Mark(Type Type = UnknownType, size_t Length = 0)
    : m_Type(Type), m_Length(static_cast<u16>(Length))
  {}

  u16 GetType(void)   const { return m_Type;   }
  u16 GetSize(void) const { return m_Length; }

private:
  u16 m_Type;
  u16 m_Length;
};

class MEDUSA_EXPORT LineData
{
public:
  LineData(Address const& rAddress = Address(), std::string const& rText = "", Mark::List const& rMarks = Mark::List(), std::set<u16> const& rOperandsOffset = std::set<u16>())
    : m_Address(rAddress), m_Text(rText), m_Marks(rMarks), m_OperandsOffset(rOperandsOffset) {}

  Address     const& GetAddress(void)         const { return m_Address; }
  std::string const& GetText(void)            const { return m_Text; }
  Mark::List  const& GetMarks(void)           const { return m_Marks; }

  bool               GetOperandNo(u16 Offset, u8& rOperandNo) const;

private:
  Address       m_Address;
  std::string   m_Text;
  Mark::List    m_Marks;
  std::set<u16> m_OperandsOffset;
};

class MEDUSA_EXPORT PrintData
{
public:
  PrintData(void);

  PrintData(PrintData const& rPD);

  void PrependAddress(bool Flag) { m_PrependAddress = Flag; }
  void SetIndent(u8 Indent) { m_Indent = Indent; }

  PrintData& operator()(Address const& rAddress);
  PrintData& operator=(PrintData const& rPD);

  PrintData& AppendMnemonic (std::string const& rMnemonic)
  { _AppendText(rMnemonic, Mark::MnemonicType);   return *this; }
  PrintData& AppendMnemonicPrefix (std::string const& rMnemonic)
  { _AppendText(rMnemonic, Mark::MnemonicPrefixType);   return *this; }
    PrintData& AppendMnemonicSuffix (std::string const& rMnemonic)
  { _AppendText(rMnemonic, Mark::MnemonicSuffixType);   return *this; }
  PrintData& AppendRegister (std::string const& rRegister)
  { _AppendText(rRegister, Mark::RegisterType);   return *this; }
  PrintData& AppendImmediate(std::string const& rImmediate)
  { _AppendText(rImmediate, Mark::ImmediateType); return *this; }
  PrintData& AppendImmediate(ap_int const& rImmediate, u32 Bits, u8 Base = 0x10);
  PrintData& AppendImmediate(BitVector const& rImmediate, u8 Base = 0x10);
  PrintData& AppendLabel    (std::string const& rLabel)
  { _AppendText(rLabel, Mark::LabelType);         return *this; }
  PrintData& AppendKeyword  (std::string const& rKeyword)
  { _AppendText(rKeyword, Mark::KeywordType);     return *this; }
  PrintData& AppendOperator (std::string const& rOperator)
  { _AppendText(rOperator, Mark::OperatorType);   return *this; }
  PrintData& AppendCharacter(std::string const& rCharacter)
  { _AppendText(rCharacter, Mark::CharacterType); return *this; }
  PrintData& AppendString   (std::string const& rString)
  { _AppendText(rString, Mark::StringType);       return *this; }
  PrintData& AppendComment  (std::string const& rComment)
  { _AppendText(rComment, Mark::CommentType);     return *this; }

  PrintData& AppendAddress(Address const& rAddress);
  PrintData& AppendSpace(u16 SpaceNo = 1);
  PrintData& AppendNewLine(void);

  PrintData& MarkOffset(void);

  Address::Vector GetAddresses(void) const;
  bool          GetFirstAddress(Address& rAddress) const;
  bool          GetLastAddress(Address& rAddress) const;
  std::string   GetTexts(void) const;
  std::vector<std::string> GetTextLines(void) const;
  Mark::List    GetMarks(void) const;
  bool          GetLine(u16 LineNo, u16& rOffset, LineData& rLine) const;
  bool          GetLine(Address const& rAddress, u16 Offset, LineData& rLine) const;
  std::string const& GetCurrentText(void) const { return m_CurrentText; }
  u16           GetLineNo(Address const& rAddress) const;
  bool          GetLineOffset(Address const& rAddress, u16& rOffset) const;

  bool          GetOperandNo(Address const& rAddress, u16 xOffset, u16 yOffset, u8& rOperandNo) const;

  bool          Contains(Address const& rAddress) const;


  u16 GetWidth(void) const
  { return std::max(m_Width, m_LineWidth); }
  u16 GetHeight(void) const
  { return m_Height; }

  typedef std::function<void (
    Address const& rAddress,
    std::string const& rText,
    Mark::List const& rMarks)>
    LineCallback;
  void ForEachLine(LineCallback Callback) const;

  void Clear(void);

private:
  void _AppendText(std::string const& rText, Mark::Type MarkType);

  bool                m_PrependAddress;

  Address             m_CurrentAddress;
  std::string         m_CurrentText;
  Mark::List          m_CurrentMarks;
  std::set<u16>       m_CurrentOperandsOffset;
  u16                 m_CurrentCommenOffsetType;

  std::list<LineData> m_Lines;
  u16                 m_Width;
  u16                 m_LineWidth;
  u16                 m_Height;
  u8                  m_Indent;

  typedef std::mutex MutexType;
  mutable MutexType m_Mutex;
};

class MEDUSA_EXPORT GraphData
{
public:
  typedef std::tuple<PrintData, u16 /* X */, u16 /* Y */> VertexDataType;

  bool AddVertex(PrintData const& rPD, u16 X = 0, u16 Y = 0);
  bool GetVertexPosition(Address const& rVtxAddr, u16& rX, u16& rY) const;
  bool SetVertexPosition(Address const& rVtxAddr, u16 X, u16 Y);

  typedef std::function<void(PrintData const& rPD, u16 X, u16 Y)> VertexCallbackType;
  void ForEachVertex(VertexCallbackType CB) const;

  typedef std::vector<std::tuple<u16 /* X */, u16 /* Y */>> BendsType;
  bool GetBends(Graph::EdgeDescriptor const& rEdgeDesc, BendsType& rBends) const;
  void SetBends(Graph::EdgeDescriptor const& rEdgeDesc, BendsType const& rBends);

private:
  typedef std::unordered_map<Address, VertexDataType> AddressVertexMapType;
  AddressVertexMapType m_Map;

  typedef std::map<Graph::EdgeDescriptor, BendsType> BendsMapType;
  BendsMapType m_Bends;

  typedef std::mutex MutexType;
  mutable MutexType m_Mutex;
};

MEDUSA_NAMESPACE_END

#endif // !MEDUSA_CELL_TEXT_HPP