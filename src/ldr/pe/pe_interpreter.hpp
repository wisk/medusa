#ifndef _PE_INTERPRETER_HPP_
#define _PE_INTERPRETER_HPP_

#include <medusa/medusa.hpp>
#include <medusa/database.hpp>
#include <medusa/endian.hpp>
#include <medusa/function.hpp>
#include <medusa/log.hpp>

#include "pe.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <boost/foreach.hpp>

template<typename n> class PeInterpreter
{
public:
  PeInterpreter(Database& rDatabase)
    : m_rDatabase(rDatabase)
  {
  }

  ~PeInterpreter(void)
  {
  }

  Address GetEntryPoint(void)
  {
    u32 PeOffset;
    u32 EntryPoint;

    m_rDatabase.GetFileBinaryStream().Read(0x3C, PeOffset);
    m_rDatabase.GetFileBinaryStream().Read(PeOffset+40, EntryPoint);
    Log::Write("ldr_pe") << "Entry Point: " << m_ImageBase + EntryPoint << LogEnd;
    return Address(Address::FlatType, 0x0, m_ImageBase + EntryPoint, 0x0, sizeof(n) * 8);
  }

  void Map(void)
  {
    FileBinaryStream const& rBinStrm = m_rDatabase.GetFileBinaryStream();
    PeSectionHeader sc;
    u16 SizeOfOptionalHeader;
    u32 Off, Flags;
    int i;

    m_rDatabase.GetFileBinaryStream().Read(0x3C, Off);
    m_rDatabase.GetFileBinaryStream().Read(Off+0x6, m_NumberOfSections);
    m_rDatabase.GetFileBinaryStream().Read(Off+52, m_ImageBase);
    m_rDatabase.GetFileBinaryStream().Read(Off+0x14, SizeOfOptionalHeader);
    Off += 0x18 + SizeOfOptionalHeader; /* Off = Offset of the first section header */

    Log::Write("ldr_pe")
      << "- ImageBase: "      << m_ImageBase << ", "
      << "- Section number: " << m_NumberOfSections << LogEnd;

    Address EntryPoint = GetEntryPoint();

    for (i = 0; i < m_NumberOfSections; i++)
    {
      Flags = MA_READ;
      m_rDatabase.GetFileBinaryStream().Read(Off, &sc, sizeof(sc));

      std::string SectionName(reinterpret_cast<const char *>(sc.Name), 0, PE_SIZEOF_SHORT_NAME);

      if (sc.Characteristics & PE_SCN_MEM_EXECUTE)
        Flags |= MA_EXEC;
      else if (EntryPoint.GetOffset() >= m_ImageBase + sc.VirtualAddress
          &&   EntryPoint.GetOffset() <  m_ImageBase + sc.VirtualAddress + sc.Misc.VirtualSize)
      {
        Log::Write("ldr_pe") << "Promote section " << SectionName << " to executable since it contains the entry point" << LogEnd;
        Flags |= MA_EXEC;
      }
      if (sc.Characteristics & PE_SCN_MEM_WRITE)
        Flags |= MA_WRITE;

      m_rDatabase.AddMemoryArea(new MappedMemoryArea(
        m_rDatabase.GetFileBinaryStream(), SectionName,
        Address(Address::PhysicalType, sc.PointerToRawData),                      sc.SizeOfRawData,
        Address(Address::FlatType, 0x0, m_ImageBase + sc.VirtualAddress, 16, sizeof(n) * 8),  sc.Misc.VirtualSize,
        Flags
        ));

     Log::Write("ldr_pe")
        << "-   Section " << SectionName << " : "
        << "    VAddr: " << m_ImageBase + sc.VirtualAddress
        << "    ROff: "  << sc.PointerToRawData
        << "    Vsize: " << sc.Misc.VirtualSize
        << "    Flags: " << sc.Characteristics
        << LogEnd;

      Off += sizeof(sc);
    }

    GetImport();
  }

  void GetImport()
  {
    FileBinaryStream const&       rBinStrm = m_rDatabase.GetFileBinaryStream();
    struct ImageImportDescriptor  CurImp, EndImp = { 0 };
    u32                           ImportOff, ImportSize, Off;
    std::string                   DllName;
    TOffset                       tmp;
    u8                            c;

    // Find data directory
    m_rDatabase.GetFileBinaryStream().Read(0x3C, Off);
    Off += sizeof(PE_NT_SIGNATURE) + sizeof(struct PeFileHeader);
    Off += (sizeof(n) == sizeof(u32))
      ? offsetof(PeOptionalHeader32, DataDirectory)
      : offsetof(PeOptionalHeader64, DataDirectory);

    // Find Import
    Off += sizeof(PeDataDirectory);
    m_rDatabase.GetFileBinaryStream().Read(Off, ImportOff);
    m_rDatabase.Translate((TOffset) m_ImageBase + ImportOff, tmp);
    ImportOff = static_cast<u32>(tmp);
    m_rDatabase.GetFileBinaryStream().Read(Off + sizeof(((PeDataDirectory*)0)->VirtualAddress), ImportSize);

    // For each import descriptor (each dll)
    m_rDatabase.GetFileBinaryStream().Read(ImportOff, &CurImp, sizeof(CurImp));
    while (memcmp(&CurImp, &EndImp, sizeof(CurImp)))
    {
      m_rDatabase.Translate((TOffset) m_ImageBase + CurImp.Name, tmp);
      CurImp.Name = static_cast<u32>(tmp);
      if (CurImp.Name)
      {
        DllName.clear();
        do
        {
          m_rDatabase.GetFileBinaryStream().Read(CurImp.Name++, c);
          if (c)
            DllName += c;
        } while (c);
      }

      GetImportedFunctions(DllName, m_ImageBase + CurImp.OriginalFirstThunk);

      ImportOff += sizeof(struct ImageImportDescriptor);
      m_rDatabase.GetFileBinaryStream().Read(ImportOff, &CurImp, sizeof(CurImp));
    }
  }

  void GetImportedFunctions(std::string &DllName, TOffset IatVa)
  {
    struct ImageThunkData<n>  CurThunk, EndThunk = { 0 };
    std::string               FunctionName;
    n                         OrdinalFlag;
    TOffset                   tmp;
    TOffset                   IatOff;
    u8                        c;

    Log::Write("ldr_pe")
      << "DLL: "    << DllName
      <<  ", IAT: " << IatVa
      << LogEnd;
    OrdinalFlag = (sizeof(n) == sizeof(u32)) ? PE_ORDINAL_FLAG32 : PE_ORDINAL_FLAG64;

    // Get IAT raw offset
    m_rDatabase.Translate((TOffset) IatVa, tmp);
    IatOff = tmp;

    // For each thunk data (each imported function) of the dll
    m_rDatabase.GetFileBinaryStream().Read(IatOff, &CurThunk, sizeof(CurThunk));
    while (memcmp(&CurThunk, &EndThunk, sizeof(CurThunk)))
    {
      FunctionName.clear();
      FunctionName = DllName + "!";

      if (CurThunk.Ordinal & OrdinalFlag)
      {
        // Get Ordinal
        std::stringstream OrdinalStream;

        CurThunk.Ordinal &= ~OrdinalFlag;
        OrdinalStream << CurThunk.Ordinal;
        FunctionName += "Ordinal_" + OrdinalStream.str();
      }
      else
      {
        // Get ImageImportByName
        m_rDatabase.Translate((TOffset) m_ImageBase + CurThunk.Function, tmp);
        CurThunk.Function = static_cast<n>(tmp + sizeof(((ImageImportByName*)0)->Hint));
        if (CurThunk.Function)
        {
          do
          {
            m_rDatabase.GetFileBinaryStream().Read(CurThunk.Function++, c);
            if (c)
              FunctionName += c;
          } while (c);
        }
      }

      Address IatAddr(Address::FlatType, 0x0, IatVa, 0, sizeof(n) * 8);
      Log::Write("ldr_pe") << IatVa << ":   " << FunctionName << LogEnd;
      m_rDatabase.AddLabel(IatAddr, Label(FunctionName, Label::LabelCode | Label::LabelImported));
      m_rDatabase.ChangeValueSize(IatAddr, IatAddr.GetOffsetSize(), true);
      m_rDatabase.RetrieveCell(IatVa)->SetComment(FunctionName);

      IatOff += sizeof(struct ImageThunkData<n>);
      IatVa += sizeof(struct ImageThunkData<n>);
      m_rDatabase.GetFileBinaryStream().Read(IatOff, &CurThunk, sizeof(CurThunk));
    }
  }

private:
  Database&               m_rDatabase;
  u16                     m_NumberOfSections;
  n                       m_ImageBase;
};

#endif
