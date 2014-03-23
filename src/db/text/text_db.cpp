#include "text_db.hpp"

#include <medusa/log.hpp>

#include <sstream>
#include <string>
#include <thread>

namespace
{
  static std::string wcstr2mbstr(std::wstring const& s)
  {
    char *mbs = new char[s.length() + 1];
    std::string result;

    if (wcstombs(mbs, s.c_str(), s.length()) == -1)
      throw std::invalid_argument("convertion failed");

    mbs[s.length()] = '\0';

    result = mbs;

    delete[] mbs;

    return result;
  }

  // ref: http://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
  static std::string Base64Encode(void const *pRawData, u32 Size)
  {
    try
    {
      static const std::string Base64Padding[] = {"", "==","="};
      namespace bai = boost::archive::iterators;
      typedef bai::base64_from_binary<bai::transform_width<char const*, 6, 8>> Base64EncodeType;
      std::stringstream os;

      std::copy(
        Base64EncodeType(pRawData),
        Base64EncodeType(reinterpret_cast<u8 const *>(pRawData) + Size),
        bai::ostream_iterator<char>(os));

      os << Base64Padding[Size % 3];
      return os.str();
    }
    catch (std::exception &e)
    {
      Log::Write("db_text") << "exception: " << e.what() << LogEnd;
    }
    return "";
  }

  static std::string Base64Encode(std::string const &rRawData)
  {
    return Base64Encode(rRawData.data(), static_cast<u32>(rRawData.size()));
  }

}

static std::string Base64Decode(std::string const &rBase64Data)
{
  std::string Res;

  try
  {
    namespace bai = boost::archive::iterators;
    typedef bai::transform_width<bai::binary_from_base64<const char *>, 8, 6> Base64DecodeType;

    auto const End = rBase64Data.size();
    Base64DecodeType itBase64(rBase64Data.c_str());
    for (std::string::size_type Cur = 0; Cur < End; ++Cur)
    {
      Res += *itBase64;
      ++itBase64;
    }
    return Res;
  }
  // NOTE: we assume we got an exception if the decoding is done (which cannot be always true...)
  catch (std::exception &)
  {
  }
  return Res;
}

TextDatabase::TextDatabase(void)
{
}

TextDatabase::~TextDatabase(void)
{
}

std::string TextDatabase::GetName(void) const
{
  return "Text";
}

std::string TextDatabase::GetExtension(void) const
{
  return ".mdt";
}

bool TextDatabase::IsCompatible(std::wstring const& rDatabasePath) const
{
  std::ifstream File(wcstr2mbstr(rDatabasePath));
  if (File.is_open() == false)
    return false;
  std::string Line;
  std::getline(File, Line);
  return Line == "# Medusa Text Database";
}

bool TextDatabase::Open(std::wstring const& rDatabasePath)
{
  m_TextFile.open(wcstr2mbstr(rDatabasePath), std::ios_base::in | std::ios_base::out);
  std::string CurLine;
  enum State
  {
    UnknownState,
    BinaryStreamState,
    MemoryAreaState,
    LabelState,
    CrossReferenceState,
    MultiCellState,
    CommentState,
  };

  State CurState = UnknownState;
  static std::unordered_map<std::string, State> StrToState;
  if (StrToState.empty())
  {
    StrToState["## BinaryStream"] = BinaryStreamState;
    StrToState["## MemoryArea"] = MemoryAreaState;
    StrToState["## Label"] = LabelState;
    StrToState["## CrossReference"] = CrossReferenceState;
    StrToState["## MultiCell"] = MultiCellState;
    StrToState["## Comment"] = CommentState;
  }

  MemoryArea *pMemArea = nullptr;
  while (std::getline(m_TextFile, CurLine))
  {
    if (CurLine == "# Medusa Text Database")
      continue;
    if (CurLine.compare(0, 3, "## ") == 0)
    {
      auto itState = StrToState.find(CurLine);
      if (itState == std::end(StrToState))
      {
        Log::Write("db_text") << "malformed database" << LogEnd;
        return false;
      }
      CurState = itState->second;
      continue;
    }
    switch (CurState)
    {
    case BinaryStreamState:
      {
        auto const RawBinStr = Base64Decode(CurLine);
        if (RawBinStr.empty())
          return false;
        SetBinaryStream(std::make_shared<MemoryBinaryStream>(RawBinStr.c_str(), RawBinStr.size()));
      }
      break;
    case MemoryAreaState:
      if (CurLine.compare(0, 3, "ma(") == 0)
      {
        CurLine.erase(std::begin(CurLine),   std::begin(CurLine) + 3); // erase ma(
        CurLine.erase(std::end(CurLine) - 1, std::end(CurLine)      ); // erase )
        char Type;
        std::string Name;
        TOffset FileOffset;
        u32 FileSize;
        Address VirtAddr;
        u32 VirtSize;
        std::string Access;
        auto ParseAccess = [](std::string const& Acc) -> u32
        {
          u32 Res = 0x0;
          if (Acc[0] == 'R') Res |= MemoryArea::Read;
          if (Acc[1] == 'W') Res |=  MemoryArea::Write;
          if (Acc[2] == 'X') Res |= MemoryArea::Execute;
          return Res;
        };
        std::istringstream iss(CurLine);
        iss >> Type >> std::hex;
        switch (Type)
        {
        case 'm': // MappedMemoryArea
          iss >> Name >> FileOffset >> FileSize >> VirtAddr >> VirtSize >> Access;
          pMemArea = new MappedMemoryArea(Name, FileOffset, FileSize, VirtAddr, VirtSize, ParseAccess(Access));
          break;
        case 'v': // VirtualMemoryArea
          iss >> Name >> VirtAddr >> VirtSize >> Access;
          pMemArea = new VirtualMemoryArea(Name, VirtAddr, VirtSize, ParseAccess(Access));
          break;
        default:
          Log::Write("db_text") << "unknown memory area type" << LogEnd;
          return false;
        }
        m_MemoryAreas.insert(pMemArea);
      }
      else if (CurLine[0] == '|')
      {
        u32 DnaOffset;
        u16 Type, SubType; // u8 in fact
        u16 Size;
        u16 FormatStyle;
        u16 Flags; // u8 in fact
        Tag ArchTag;
        u16 Mode; // u8 in fact
        std::istringstream issDna(CurLine);
        issDna.seekg(1, std::ios::cur);
        issDna >> std::hex >> DnaOffset;
        issDna.seekg(::strlen(" dna("), std::ios::cur);
        issDna >> std::hex >> Type >> SubType >> Size >> FormatStyle >> Flags >> Mode >> ArchTag;
        Address::List DelAddr;
        auto spDna = std::make_shared<CellData>(CellData(
          /**/static_cast<u8>(Type), static_cast<u8>(SubType), Size,
          /**/FormatStyle, static_cast<u8>(Flags),
          /**/ArchTag,
          /**/static_cast<u8>(Mode)));
        pMemArea->SetCellData(pMemArea->GetBaseAddress().GetOffset() + DnaOffset, spDna, DelAddr, true); // TODO: check result and pMemArea
      }
      break;
    case LabelState:
      {
        Address LblAddr;
        std::string LblName;
        u16 LblNameLen;
        std::string LblTypeStr;
        u32 LblVer;
        std::istringstream issLbl(CurLine);
        issLbl >> LblAddr;
        issLbl.seekg(::strlen(" lbl("), std::ios::cur);
        issLbl >> std::hex >> LblName >> LblNameLen >> LblTypeStr >> LblVer; // encode label name
        if (LblTypeStr.length() != 3)
        {
          Log::Write("db_text") << "unknown type for label located at " << LblAddr << LogEnd;
          continue;
        }
        auto ParseType = [](std::string const &Type) -> u16
        {
          u16 Res = 0;
          switch (Type[0])
          {
          case 'c': Res |= Label::Code;     break;
          case 's': Res |= Label::String;   break;
          }
          switch (Type[1])
          {
          case 'i': Res |= Label::Imported; break;
          case 'e': Res |= Label::Exported; break;
          case 'g': Res |= Label::Global;   break;
          case 'l': Res |= Label::Local;    break;
          }
          if (Type[2] == 'a') Res |= Label::AutoGenerated;
          return Res;
        };
        if (!AddLabel(LblAddr, Label(LblName, ParseType(LblTypeStr), LblVer)))
          Log::Write("db_text") << "unable to add label: " << LblName << LogEnd;
      }
      break;
    case CrossReferenceState:
      {
        Address To, From;
        std::istringstream issCrossRef(CurLine);
        issCrossRef >> To;
        while (!issCrossRef.eof())
        {
          issCrossRef >> From;
          if (!AddCrossReference(To, From))
            Log::Write("db_text") << "unable to add cross reference to: " << To << ", from: " << From << LogEnd;
        }
      }
      break;
    case MultiCellState:
      {
        Address McAddr;
        char McType;
        u16 McSize;
        std::istringstream issMc(CurLine);
        issMc >> McAddr;
        issMc.seekg(::strlen(" mc("), std::ios::cur);
        issMc >> std::hex >> McType >> McSize;
        auto ParseType = [](char Type) -> u8
        {
          switch (Type)
          {
          case 'f': return MultiCell::FunctionType;
          case 's': return MultiCell::StructType;
          case 'a': return MultiCell::ArrayType;
          default:  return MultiCell::UnknownType;
          }
        };
        m_MultiCells[McAddr] = MultiCell(ParseType(McType), McSize);
      }
      break;
    case CommentState:
      {
        Address CmtAddr;
        std::string CmtBase64;
        std::istringstream issCmt(CurLine);
        issCmt >> CmtAddr >> CmtBase64;
        if (!SetComment(CmtAddr, Base64Decode(CmtBase64)))
          Log::Write("db_text") << "unable to set comment at " << CmtAddr << LogEnd;
      }
      break;
    default:
      Log::Write("db_text") << "unknown state in database" << LogEnd;
      return false;
    }
  }

  return m_TextFile.is_open();
}

bool TextDatabase::Create(std::wstring const& rDatabasePath, bool Force)
{
  // we return false if we already have a valid file,
  if (m_TextFile.is_open())
    return false;

  // we return false if the file already exists and Force is false,
  auto const DatabasePath = wcstr2mbstr(rDatabasePath);
  m_TextFile.open(DatabasePath, std::ios_base::in | std::ios_base::out);
  if (m_TextFile.is_open() == true && Force == false)
  {
    m_TextFile.close();
    return false;
  }

  // otherwise, we try to truncate it and open it.
  m_TextFile.open(DatabasePath, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
  return m_TextFile.is_open();
}

bool TextDatabase::Flush(void)
{
  if (!m_TextFile.is_open())
    return false;

  m_TextFile.seekp(0, std::ios::beg);
  m_TextFile.seekg(0, std::ios::beg);

  std::streamoff TotalTell = m_TextFile.tellp() + m_TextFile.tellg();

  if (TotalTell != 0)
    return false;

  m_TextFile << std::hex << std::showbase << "# Medusa Text Database\n";

  // Save binary stream
  {
    m_TextFile << "## BinaryStream\n";
    std::string Base64Data = Base64Encode(m_spBinStrm->GetBuffer(), m_spBinStrm->GetSize());
    m_TextFile << Base64Data << "\n" << std::flush;
  }

  // Save memory area
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    m_TextFile << "## MemoryArea\n";
    for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    {
      m_TextFile << (*itMemArea)->Dump() << "\n" << std::flush;
      (*itMemArea)->ForEachCellData([&](TOffset Offset, CellData::SPtr spCellData)
      {
        m_TextFile << "|" << Offset << " " << spCellData->Dump() << "\n" << std::flush;
      });
    }
  }

  // Save label
  {
    std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
    m_TextFile << "## Label\n";
    for (auto itLabel = std::begin(m_LabelMap.left); itLabel != std::end(m_LabelMap.left); ++itLabel)
      m_TextFile << itLabel->first.Dump() << " " << itLabel->second.Dump() << "\n" << std::flush;
  }

  // Save cross reference
  {
    std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
    m_TextFile << "## CrossReference\n";
    for (auto itXref = std::begin(m_CrossReferences.GetAllXRefs().left); itXref != std::end(m_CrossReferences.GetAllXRefs().left); ++itXref)
    {
      m_TextFile << itXref->first.Dump();
      Address::List From;
      m_CrossReferences.From(itXref->first, From);
      for (auto itAddr = std::begin(From); itAddr != std::end(From); ++itAddr)
        m_TextFile << " " << itAddr->Dump() << std::flush;
      m_TextFile << "\n";
    }
  }

  // Save multicell
  {
    std::lock_guard<std::mutex> Lock(m_MultiCellsLock);
    m_TextFile << "## MultiCell\n";
    for (auto itMultiCell = std::begin(m_MultiCells); itMultiCell != std::end(m_MultiCells); ++itMultiCell)
      m_TextFile << itMultiCell->first.Dump() << " " << itMultiCell->second.Dump() << "\n" << std::flush;
  }

  // Save comment
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    m_TextFile << "## Comment\n";
    for (auto itComment = std::begin(m_Comments); itComment != std::end(m_Comments); ++itComment)
    {
      std::string Base64Data = Base64Encode(itComment->second);
      m_TextFile << itComment->first.Dump() << " " << Base64Data << "\n";
    }
  }

  return true;
}

bool TextDatabase::Close(void)
{
  bool Res = Flush();
  m_TextFile.close();
  return Res;
}

bool TextDatabase::RegisterArchitectureTag(Tag ArchitectureTag)
{
  std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
  m_ArchitectureTags.push_back(ArchitectureTag);
  return true;
}

bool TextDatabase::UnregisterArchitectureTag(Tag ArchitectureTag)
{
  std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
  m_ArchitectureTags.remove(ArchitectureTag);
  return true;
}

std::list<Tag> TextDatabase::GetArchitectureTags(void) const
{
  std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
  return m_ArchitectureTags;
}

bool TextDatabase::AddMemoryArea(MemoryArea* pMemArea)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  m_MemoryAreas.insert(pMemArea);
  return true;
}

void TextDatabase::ForEachMemoryArea(std::function<void (MemoryArea const& rMemoryArea)> MemoryAreaPredicat) const
{
  std::for_each(std::begin(m_MemoryAreas), std::end(m_MemoryAreas), [&](MemoryArea const* pMemArea) { MemoryAreaPredicat(*pMemArea); });
}

MemoryArea const* TextDatabase::GetMemoryArea(Address const& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
    if ((*itMemArea)->IsCellPresent(rAddress))
      return *itMemArea;
  return nullptr;
}

bool TextDatabase::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset < 0)
    return _MoveAddressBackward(rAddress, rMovedAddress, Offset);
  if (Offset > 0)
    return _MoveAddressForward(rAddress, rMovedAddress, Offset);

  // FIXME in this case, we have to get the nearest address
  rMovedAddress = rAddress;
  return true;
}
bool TextDatabase::ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const
{
  rPosition = 0;
  auto const *pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;

  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsCellPresent(rAddress))
    {
      rPosition += static_cast<u32>(rAddress.GetOffset() - (*itMemArea)->GetBaseAddress().GetOffset());
      return true;
    }
    else
      rPosition += (*itMemArea)->GetSize();
  }
  return false;
}

bool TextDatabase::ConvertPositionToAddress(u32 Position, Address& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (auto itMemArea = std::begin(m_MemoryAreas); itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    u32 Size = (*itMemArea)->GetSize();
    if (Position < Size)
    {
      rAddress = (*itMemArea)->GetBaseAddress() + Position;
      return true;
    }
    Position -= Size;
  }
  return false;
}

bool TextDatabase::AddLabel(Address const& rAddress, Label const& rLabel)
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  m_LabelMap.left.insert(LabelBimapType::left_value_type(rAddress, rLabel));
  return true;
}

bool TextDatabase::RemoveLabel(Address const& rAddress)
{
  boost::lock_guard<std::recursive_mutex> Lock(m_LabelLock);

  auto itLbl = m_LabelMap.left.find(rAddress);
  if (itLbl == std::end(m_LabelMap.left))
    return false;

  m_LabelMap.left.erase(itLbl);
  return true;
}

bool TextDatabase::GetLabel(Address const& rAddress, Label& rLabel) const
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  auto itLbl = m_LabelMap.left.find(rAddress);
  if (itLbl == std::end(m_LabelMap.left))
    return false;

  rLabel = itLbl->second;
  return true;
}

bool TextDatabase::GetLabelAddress(Label const& rLabel, Address& rAddress) const
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
  auto itLbl = m_LabelMap.right.find(rLabel);
  if (itLbl == std::end(m_LabelMap.right))
    return false;

  rAddress = itLbl->second;
  return true;
}

// This function is not entirely thread-safe
// This function could crash if the predicat remove the next label...
void TextDatabase::ForEachLabel(std::function<void (Address const& rAddress, Label const& rLabel)> LabelPredicat)
{
  static std::mutex s_ForEachLabelMutex;
  std::lock_guard<std::mutex> Lock(s_ForEachLabelMutex);
  auto itLbl = std::begin(m_LabelMap.left);
  auto itLblEnd = std::end(m_LabelMap.left);
  while (itLbl != itLblEnd)
  {
    auto itCurLbl      = itLbl++;
    Address const Addr = itCurLbl->first;
    Label const Lbl    = itCurLbl->second;
    LabelPredicat(Addr, Lbl);
  }
}

bool TextDatabase::AddCrossReference(Address const& rTo, Address const& rFrom)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.AddXRef(rTo, rFrom);
}

bool TextDatabase::RemoveCrossReference(Address const& rFrom)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.RemoveRef(rFrom);
}

bool TextDatabase::RemoveCrossReferences(void)
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  m_CrossReferences.EraseAll();
  return true;
}

bool TextDatabase::HasCrossReferenceFrom(Address const& rTo) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.HasXRefFrom(rTo);
}

bool TextDatabase::GetCrossReferenceFrom(Address const& rTo, Address::List& rFromList) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.From(rTo, rFromList);
}

bool TextDatabase::HasCrossReferenceTo(Address const& rFrom) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.HasXRefTo(rFrom);
}

bool TextDatabase::GetCrossReferenceTo(Address const& rFrom, Address& rTo) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.To(rFrom, rTo);
}

bool TextDatabase::AddMultiCell(Address const& rAddress, MultiCell const& rMultiCell)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  m_MultiCells[rAddress] = rMultiCell;
  return true;
}

bool TextDatabase::RemoveMultiCell(Address const& rAddress)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);

  auto itMultiCell = m_MultiCells.find(rAddress);
  if (itMultiCell == std::end(m_MultiCells))
    return false;
  m_MultiCells.erase(itMultiCell);
  return true;
}

bool TextDatabase::GetMultiCell(Address const& rAddress, MultiCell& rMultiCell) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  auto itMultiCell = m_MultiCells.find(rAddress);
  if (itMultiCell == std::end(m_MultiCells))
    return false;
  rMultiCell = itMultiCell->second;
  return true;
}

bool TextDatabase::GetCellData(Address const& rAddress, CellData& rCellData)
{
  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;
  auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
  if (spCellData == nullptr)
    return false;
  rCellData = *spCellData;
  return true;
}

bool TextDatabase::SetCellData(Address const& rAddress, CellData const& rCellData, Address::List& rDeletedCellAddresses, bool Force)
{
  MemoryArea* pMemArea = nullptr;
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  auto itEndMemArea = std::end(m_MemoryAreas);
  for (auto itCurMemArea = std::begin(m_MemoryAreas); itCurMemArea != itEndMemArea; ++itCurMemArea)
    if ((*itCurMemArea)->IsCellPresent(rAddress))
    {
      pMemArea = *itCurMemArea;
      break;
    }
  if (pMemArea == nullptr)
    return false;
  CellData::SPtr spCellData = std::make_shared<CellData>(rCellData);
  return pMemArea->SetCellData(rAddress.GetOffset(), spCellData, rDeletedCellAddresses, Force);
}

bool TextDatabase::GetComment(Address const& rAddress, std::string& rComment) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  auto itCmt = m_Comments.find(rAddress);
  if (itCmt == std::end(m_Comments))
    return false;
  rComment = itCmt->second;
  return true;
}

bool TextDatabase::SetComment(Address const& rAddress, std::string const& rComment)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);

  if (rComment.empty())
  {
    m_Comments.erase(rAddress);
    return true;
  }

  m_Comments[rAddress] = rComment;
  return true;
}

bool TextDatabase::_MoveAddressBackward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  // FIXME: Handle Offset
  if (m_MemoryAreas.empty())
    return false;

  if (rAddress <= (*m_MemoryAreas.begin())->GetBaseAddress())
  {
    rMovedAddress = rAddress;
    return true;
  }

  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsCellPresent(rAddress))
      break;
  }
  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  u64 CurMemAreaOff = (rAddress.GetOffset() - (*itMemArea)->GetBaseAddress().GetOffset());
  if (static_cast<u64>(-Offset) <= CurMemAreaOff)
    return (*itMemArea)->MoveAddressBackward(rAddress, rMovedAddress, Offset);
  Offset += CurMemAreaOff;

  if (itMemArea == std::begin(m_MemoryAreas))
    return false;
  --itMemArea;

  bool Failed = false;
  Address CurAddr = ((*itMemArea)->GetBaseAddress() + ((*itMemArea)->GetSize() - 1));
  while (itMemArea != std::begin(m_MemoryAreas))
  {
    u64 MemAreaSize = (*itMemArea)->GetSize();
    if (static_cast<u64>(-Offset) < MemAreaSize)
      break;
    Offset += MemAreaSize;
    CurAddr = ((*itMemArea)->GetBaseAddress() + ((*itMemArea)->GetSize() - 1));

    if (itMemArea == std::begin(m_MemoryAreas))
      return false;

    --itMemArea;
  }

  return (*itMemArea)->MoveAddressBackward(CurAddr, rMovedAddress, Offset);
}

bool TextDatabase::_MoveAddressForward(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (m_MemoryAreas.empty())
    return false;

  auto itMemArea = std::begin(m_MemoryAreas);
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    if ((*itMemArea)->IsCellPresent(rAddress))
      break;
  }
  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  u64 CurMemAreaOff = (rAddress.GetOffset() - (*itMemArea)->GetBaseAddress().GetOffset());
  if (CurMemAreaOff + Offset < (*itMemArea)->GetSize())
    if ((*itMemArea)->MoveAddressForward(rAddress, rMovedAddress, Offset) == true)
      return true;

  s64 DiffOff = ((*itMemArea)->GetSize() - CurMemAreaOff);
  if (DiffOff >= Offset)
    Offset = 0;
  else
    Offset -= DiffOff;
  ++itMemArea;

  if (itMemArea == std::end(m_MemoryAreas))
    return false;

  Address CurAddr = (*itMemArea)->GetBaseAddress();
  for (; itMemArea != std::end(m_MemoryAreas); ++itMemArea)
  {
    u64 MemAreaSize = (*itMemArea)->GetSize();
    if (static_cast<u64>(Offset) < MemAreaSize)
      if ((*itMemArea)->MoveAddressForward(CurAddr, rMovedAddress, Offset) == true)
        return true;
    Offset -= MemAreaSize;
    CurAddr = (*itMemArea)->GetBaseAddress();
  }

  return false;
}