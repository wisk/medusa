#include "text_db.hpp"

#include <medusa/module.hpp>
#include <medusa/log.hpp>
#include <medusa/util.hpp>

#include <sstream>
#include <string>
#include <thread>

#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>


TextDatabase::TextDatabase(void) : m_DirtyLabels(false), m_IsIteratingLabels(false)
{
}

TextDatabase::~TextDatabase(void)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (auto* pMemArea : m_MemoryAreas)
    delete pMemArea;
}

std::string TextDatabase::GetName(void) const
{
  return "Text";
}

std::string TextDatabase::GetExtension(void) const
{
  return ".mdt";
}

bool TextDatabase::IsCompatible(boost::filesystem::path const& rDatabasePath) const
{
  std::ifstream File(rDatabasePath.string());
  if (File.is_open() == false)
    return false;
  std::string Line;
  std::getline(File, Line);
  return Line == "# Medusa Text Database";
}

bool TextDatabase::Open(boost::filesystem::path const& rDatabasePath)
{
  if (!m_DatabasePath.string().empty())
    return false;
  m_DatabasePath = rDatabasePath;
  std::ifstream TextFile(rDatabasePath.string());
  if (!TextFile.is_open())
    return false;
  std::string CurLine;
  enum State
  {
    UnknownState,
    BinaryStreamState,
    ArchitectureState,
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
    StrToState["## Architecture"] = ArchitectureState;
    StrToState["## MemoryArea"] = MemoryAreaState;
    StrToState["## Label"] = LabelState;
    StrToState["## CrossReference"] = CrossReferenceState;
    StrToState["## MultiCell"] = MultiCellState;
    StrToState["## Comment"] = CommentState;
  }

  auto& rModMgr = ModuleManager::Instance();
  MemoryArea *pMemArea = nullptr;
  while (std::getline(TextFile, CurLine))
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
        SetBinaryStream(std::make_shared<MemoryBinaryStream>(RawBinStr.c_str(), static_cast<u32>(RawBinStr.size())));
      }
      break;
    case ArchitectureState:
      {
        Tag CurTag;
        std::istringstream issTag(CurLine);
        while (!issTag.eof())
        {
          if (!(issTag >> std::hex >> CurTag))
            break;
          auto spArch = rModMgr.FindArchitecture(CurTag);
          if (spArch == nullptr)
            Log::Write("core") << "unable to load architecture with tag " << CurTag << LogEnd;
          else
            m_ArchitectureTags.push_back(CurTag);
        }
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
          case 'd': Res |= Label::Data;     break;
          case 'c': Res |= Label::Code;     break;
          case 'f': Res |= Label::Function; break;
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
          if (!(issCrossRef >> From))
            break;
          if (!AddCrossReference(To, From))
            Log::Write("db_text") << "unable to add cross reference to: " << To << ", from: " << From << LogEnd;
        }
      }
      break;
    case MultiCellState:
      {
        Address McAddr;
        MultiCell::Id McId;
        char McType;
        u16 McSize;
        std::istringstream issMc(CurLine);
        issMc >> McAddr;
        issMc.seekg(::strlen(" mc("), std::ios::cur);
        issMc >> std::hex >> McId >> McType >> McSize;
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
        m_MultiCells[McAddr] = MultiCell(McId, ParseType(McType), McSize);
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

  return TextFile.is_open();
}

bool TextDatabase::Create(boost::filesystem::path const& rDatabasePath, bool Force)
{
  if (!m_DatabasePath.string().empty())
  {
    Log::Write("db_text") << "db path is empty" << LogEnd;
    return false;
  }

  // If the user doesn't force and file exists, we return false
  if (!Force && _FileExists(rDatabasePath))
  {
    Log::Write("db_text") << "db already exists and force is false" << LogEnd;
    return false;
  }

  if (Force)
  {
    Log::Write("db_text") << "remove file " << rDatabasePath.string() << LogEnd;
    _FileRemoves(rDatabasePath);
  }

  if (!_FileCanCreate(rDatabasePath))
  {
    Log::Write("db_text") << "unable to create file " << rDatabasePath << LogEnd;
    return false;
  }

  m_DatabasePath = rDatabasePath;

  return true;
}

bool TextDatabase::Flush(void)
{
  if (m_DatabasePath.string().empty())
    return false;
  _FileRemoves(m_DatabasePath);

  std::ofstream TextFile(m_DatabasePath.string());
  if (!TextFile.is_open())
    return false;

  TextFile << std::hex << std::showbase << "# Medusa Text Database\n";

  // Save binary stream
  {
    TextFile << "## BinaryStream\n";
    std::string Base64Data = Base64Encode(m_spBinStrm->GetBuffer(), m_spBinStrm->GetSize());
    TextFile << Base64Data << "\n" << std::flush;
  }

  // Save architecture tag
  {
    std::lock_guard<std::mutex> Lock(m_ArchitectureTagLock);
    TextFile << "## Architecture\n";
    char const* pSep = "";
    for (Tag ArchTag : m_ArchitectureTags)
    {
      TextFile << pSep << ArchTag;
      pSep = " ";
    }
    TextFile << "\n";
  }

  // Save memory area
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    TextFile << "## MemoryArea\n";
    for (MemoryArea* pMemArea : m_MemoryAreas)
    {
      TextFile << pMemArea->Dump() << "\n" << std::flush;
      pMemArea->ForEachCellData([&](TOffset Offset, CellData::SPType spCellData)
      {
        TextFile << "|" << Offset << " " << spCellData->Dump() << "\n" << std::flush;
      });
    }
  }

  // Save label
  {
    std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);
    TextFile << "## Label\n";
    for (auto itLabel = std::begin(m_LabelMap.left); itLabel != std::end(m_LabelMap.left); ++itLabel)
      TextFile << itLabel->first.Dump() << " " << itLabel->second.Dump() << "\n" << std::flush;
  }

  // Save cross reference
  {
    std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
    TextFile << "## CrossReference\n";
    for (auto itXref = std::begin(m_CrossReferences.GetAllXRefs().left); itXref != std::end(m_CrossReferences.GetAllXRefs().left); ++itXref)
    {
      TextFile << itXref->first.Dump();
      Address::List From;
      m_CrossReferences.From(itXref->first, From);
      for (Address const& rAddr : From)
        TextFile << " " << rAddr.Dump() << std::flush;
      TextFile << "\n";
    }
  }

  // Save multicell
  {
    std::lock_guard<std::mutex> Lock(m_MultiCellsLock);
    TextFile << "## MultiCell\n";
    for (auto itMultiCell = std::begin(m_MultiCells); itMultiCell != std::end(m_MultiCells); ++itMultiCell)
      TextFile << itMultiCell->first.Dump() << " " << itMultiCell->second.Dump() << "\n" << std::flush;
  }

  // Save comment
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    TextFile << "## Comment\n";
    for (auto itComment = std::begin(m_Comments); itComment != std::end(m_Comments); ++itComment)
    {
      std::string Base64Data = Base64Encode(itComment->second);
      TextFile << itComment->first.Dump() << " " << Base64Data << "\n";
    }
  }
  TextFile.flush();
  return true;
}

bool TextDatabase::Close(void)
{
  bool Res = Flush();
  m_DatabasePath = boost::filesystem::path();
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

bool TextDatabase::SetArchitecture(Address const& rAddress, Tag ArchitectureTag, u8 Mode, Database::SetArchitectureModeType SetArchMode)
{
  switch (SetArchMode)
  {
  case ByCell:
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);

    for (MemoryArea* pMemArea : m_MemoryAreas)
    {
      if (!pMemArea->IsCellPresent(rAddress))
        continue;
      if (!pMemArea->IsCellPresent(rAddress.GetOffset()))
        continue;

      auto spCellData = pMemArea->GetCellData(rAddress.GetOffset());
      if (spCellData == nullptr)
        spCellData = std::make_shared<CellData>();

      spCellData->SetDefaultArchitectureTag(ArchitectureTag);
      spCellData->SetDefaultMode(Mode);
      return pMemArea->SetCellData(rAddress.GetOffset(), spCellData);
    }

    break;
  }

  case ByMemoryArea:
  {
    std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
    for (MemoryArea* pMemArea : m_MemoryAreas)
    {
      if (pMemArea->IsCellPresent(rAddress))
      {
        pMemArea->SetDefaultArchitectureTag(ArchitectureTag);
        pMemArea->SetDefaultArchitectureMode(Mode);
        return true;
      }
    }

    break;
  }

  default:
    break;
  }

  return false;
}

bool TextDatabase::AddMemoryArea(MemoryArea* pMemArea)
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  m_MemoryAreas.insert(pMemArea);
  return true;
}

// TODO: Protect iterating against RemoveMemoryArea
void TextDatabase::ForEachMemoryArea(MemoryAreaCallback Callback) const
{
  for (auto const pMemArea : m_MemoryAreas)
    Callback(*pMemArea);
}

MemoryArea const* TextDatabase::GetMemoryArea(Address const& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (MemoryArea* pMemArea : m_MemoryAreas)
    if (pMemArea->IsCellPresent(rAddress))
      return pMemArea;
  return nullptr;
}

bool TextDatabase::GetFirstAddress(Address& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  if (m_MemoryAreas.empty())
    return false;
  rAddress = (*m_MemoryAreas.begin())->GetBaseAddress();
  return true;
}

bool TextDatabase::GetLastAddress(Address& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  if (m_MemoryAreas.empty())
    return false;
  auto itLastMemArea = m_MemoryAreas.end();
  --itLastMemArea;
  rAddress = (*itLastMemArea)->GetBaseAddress() + ((*itLastMemArea)->GetSize() - 1);
  if (!(*itLastMemArea)->GetNearestAddress(rAddress, rAddress))
    return false;
  return true;
}

bool TextDatabase::MoveAddress(Address const& rAddress, Address& rMovedAddress, s64 Offset) const
{
  if (Offset < 0)
    return _MoveAddressBackward(rAddress, rMovedAddress, Offset);
  if (Offset > 0)
    return _MoveAddressForward(rAddress, rMovedAddress, Offset);

  auto pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return _MoveAddressBackward(rAddress, rMovedAddress, -1);

  return pMemArea->GetNearestAddress(rAddress, rMovedAddress);
}
bool TextDatabase::ConvertAddressToPosition(Address const& rAddress, u32& rPosition) const
{
  rPosition = 0;
  auto const *pMemArea = GetMemoryArea(rAddress);
  if (pMemArea == nullptr)
    return false;

  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (MemoryArea* pMemArea : m_MemoryAreas)
  {
    if (pMemArea->IsCellPresent(rAddress))
    {
      rPosition += static_cast<u32>(rAddress.GetOffset() - pMemArea->GetBaseAddress().GetOffset());
      return true;
    }
    else
      rPosition += pMemArea->GetSize();
  }
  return false;
}

bool TextDatabase::ConvertPositionToAddress(u32 Position, Address& rAddress) const
{
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (MemoryArea* pMemArea : m_MemoryAreas)
  {
    u32 Size = pMemArea->GetSize();
    if (Position < Size)
    {
      rAddress = pMemArea->GetBaseAddress() + Position;
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
  if (m_IsIteratingLabels)
    m_VisitedLabels[rAddress] = false;
  return true;
}

bool TextDatabase::RemoveLabel(Address const& rAddress)
{
  std::lock_guard<std::recursive_mutex> Lock(m_LabelLock);

  auto itLbl = m_LabelMap.left.find(rAddress);
  if (itLbl == std::end(m_LabelMap.left))
    return false;

  m_LabelMap.left.erase(itLbl);
  if (m_IsIteratingLabels)
  {
    m_VisitedLabels[rAddress] = false;
    m_DirtyLabels = true;
  }
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
void TextDatabase::ForEachLabel(LabelCallback Callback)
{
  static std::mutex s_ForEachLabelMutex;
  std::lock_guard<std::mutex> Lock(s_ForEachLabelMutex);

  m_IsIteratingLabels = true;
  for (auto itCurAddrLbl = std::begin(m_LabelMap.left), itEndAddrLbl = std::end(m_LabelMap.left); itCurAddrLbl != itEndAddrLbl; ++itCurAddrLbl)
  {
    Address Addr = itCurAddrLbl->first;
    Label   Lbl  = itCurAddrLbl->second;

    if (m_VisitedLabels[Addr])
      continue;

    Callback(Addr, Lbl);
    m_VisitedLabels[Addr] = true;
    if (m_DirtyLabels)
    {
      itCurAddrLbl = std::begin(m_LabelMap.left);
      m_DirtyLabels = false;
    }
  }
  m_IsIteratingLabels = false;
  m_VisitedLabels.clear();
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

bool TextDatabase::GetCrossReferenceTo(Address const& rFrom, Address::List& rToList) const
{
  std::lock_guard<std::mutex> Lock(m_CrossReferencesLock);
  return m_CrossReferences.To(rFrom, rToList);
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
  MemoryArea* pCurMemArea = nullptr;
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (MemoryArea* pMemArea : m_MemoryAreas)
    if (pMemArea->IsCellPresent(rAddress))
    {
      pCurMemArea = pMemArea;
      break;
    }
  if (pCurMemArea == nullptr)
    return false;
  CellData::SPType spCellData = std::make_shared<CellData>(rCellData);
  return pCurMemArea->SetCellData(rAddress.GetOffset(), spCellData, rDeletedCellAddresses, Force);
}

bool TextDatabase::DeleteCellData(Address const& rAddress)
{
  MemoryArea* pCurMemArea = nullptr;
  std::lock_guard<std::mutex> Lock(m_MemoryAreaLock);
  for (MemoryArea* pMemArea : m_MemoryAreas)
    if (pMemArea->IsCellPresent(rAddress))
    {
      pCurMemArea = pMemArea;
      break;
    }
  if (pCurMemArea == nullptr)
    return false;
  Address::List DelCellAddrs;
  return pCurMemArea->SetCellData(rAddress.GetOffset(), nullptr, DelCellAddrs, true);
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

bool TextDatabase::_FileExists(boost::filesystem::path const& rFilePath)
{
  return boost::filesystem::exists(rFilePath);
}

bool TextDatabase::_FileRemoves(boost::filesystem::path const& rFilePath)
{
  return boost::filesystem::remove(rFilePath);
  // truncate the file
  //std::fstream File(rFilePath.string(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
  //return File.good(); // TODO: this is not what we're expecting
}

bool TextDatabase::_FileCanCreate(boost::filesystem::path const& rFilePath)
{
  std::ofstream File(rFilePath.string());
  return File.is_open();
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
    {
      if (!(*itMemArea)->MoveAddressBackward(rAddress, rMovedAddress, Offset))
	{
	  // TODO(wisk): this behavior is incorrect...
	  rMovedAddress = (*itMemArea)->GetBaseAddress();
	}
      return true;
    }
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

  if ((*itMemArea)->MoveAddressBackward(CurAddr, rMovedAddress, Offset))
    return true;
  // TODO(wisk): this behavior is incorrect...
  rMovedAddress = (*itMemArea)->GetBaseAddress();
  return true;
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

bool TextDatabase::GetValueDetail(Id ConstId, ValueDetail& rConstDtl) const
{
  auto itValDtl = m_ValuesDetail.find(ConstId);
  if (itValDtl == std::end(m_ValuesDetail))
    return false;
  rConstDtl = itValDtl->second;
  return true;
}

bool TextDatabase::SetValueDetail(Id ConstId, ValueDetail const& rConstDtl)
{
  m_ValuesDetail[ConstId] = rConstDtl;
  return true;
}

bool TextDatabase::GetFunctionDetail(Id FuncId, FunctionDetail& rFuncDtl) const
{
  auto itFuncDtl = m_FunctionsDetail.find(FuncId);
  if (itFuncDtl == std::end(m_FunctionsDetail))
    return false;
  rFuncDtl = itFuncDtl->second;
  return true;
}

bool TextDatabase::SetFunctionDetail(Id FuncId, FunctionDetail const& rFuncDtl)
{
  m_FunctionsDetail[FuncId] = rFuncDtl;
  return true;
}

bool TextDatabase::GetStructureDetail(Id StructId, StructureDetail& rStructDtl) const
{
  auto itStructDtl = m_StructuresDetail.find(StructId);
  if (itStructDtl == std::end(m_StructuresDetail))
    return false;
  rStructDtl = itStructDtl->second;
  return true;
}

bool TextDatabase::SetStructureDetail(Id StructId, StructureDetail const& rStructDtl)
{
  m_StructuresDetail[StructId] = rStructDtl;
  return true;
}

bool TextDatabase::RetrieveDetailId(Address const& rAddress, u8 Index, Id& rDtlId) const
{
  auto itId = m_Ids.find(rAddress);
  if (itId == std::end(m_Ids))
    return false;
  if ((Index + 1) > itId->second.size())
    return false;
  auto const & rCurId = itId->second[Index];
  if (rCurId.is_nil())
    return false;
  rDtlId = rCurId;
  return true;
}

bool TextDatabase::BindDetailId(Address const& rAddress, u8 Index, Id DtlId)
{
  auto itId = m_Ids.find(rAddress);
  if (itId == std::end(m_Ids))
  {
    m_Ids[rAddress] = std::vector<Id>();
    itId = m_Ids.find(rAddress);
  }
  if ((Index + 1) > itId->second.size())
    itId->second.resize((Index + 1));
  itId->second[Index] = DtlId;
  return true;
}

bool TextDatabase::UnbindDetailId(Address const& rAddress, u8 Index)
{
  auto itId = m_Ids.find(rAddress);
  if (itId == std::end(m_Ids))
  {
    m_Ids[rAddress] = std::vector<Id>();
    itId = m_Ids.find(rAddress);
  }
  if ((Index + 1) > itId->second.size())
    itId->second.resize((Index + 1));
  itId->second[Index] = Id();
  return true;
}
