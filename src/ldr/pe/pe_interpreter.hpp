#ifndef _PE_INTERPRETER_HPP_
#define _PE_INTERPRETER_HPP_

#include <medusa/medusa.hpp>
#include <medusa/document.hpp>
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
  PeInterpreter(Document& rDoc)
    : m_rDoc(rDoc)
  {
  }

  ~PeInterpreter(void)
  {
  }

  Address GetEntryPoint(void)
  {
    u32 PeOffset;
    u32 EntryPoint;

    m_rDoc.GetFileBinaryStream().Read(0x3C, PeOffset);
    m_rDoc.GetFileBinaryStream().Read(PeOffset+40, EntryPoint);
    Log::Write("ldr_pe") << "Entry Point: " << m_ImageBase + EntryPoint << LogEnd;
    return Address(Address::FlatType, 0x0, m_ImageBase + EntryPoint, 0x0, sizeof(n) * 8);
  }

  void Map(void)
  {
    FileBinaryStream const& rBinStrm = m_rDoc.GetFileBinaryStream();
    PeSectionHeader sc;
    u16 SizeOfOptionalHeader;
    u32 Off, Flags;
    int i;

    if (sizeof(n) == sizeof(u32))
    {
      m_rDoc.GetFileBinaryStream().Read(offsetof(PeDosHeader, e_lfanew), Off);
      m_rDoc.GetFileBinaryStream().Read(Off + offsetof(PeNtHeaders32, FileHeader.NumberOfSections), m_NumberOfSections);
      m_rDoc.GetFileBinaryStream().Read(Off + offsetof(PeNtHeaders32, OptionalHeader.ImageBase), m_ImageBase);
      m_rDoc.GetFileBinaryStream().Read(Off + offsetof(PeNtHeaders32, FileHeader.SizeOfOptionalHeader), SizeOfOptionalHeader);
      Off += offsetof(PeNtHeaders32, OptionalHeader) + SizeOfOptionalHeader; /* Off = Offset of the first section header */
    }

    else if (sizeof(n) == sizeof(u64))
    {
      m_rDoc.GetFileBinaryStream().Read(offsetof(PeDosHeader, e_lfanew), Off);
      m_rDoc.GetFileBinaryStream().Read(Off + offsetof(PeNtHeaders64, FileHeader.NumberOfSections), m_NumberOfSections);
      m_rDoc.GetFileBinaryStream().Read(Off + offsetof(PeNtHeaders64, OptionalHeader.ImageBase), m_ImageBase);
      m_rDoc.GetFileBinaryStream().Read(Off + offsetof(PeNtHeaders64, FileHeader.SizeOfOptionalHeader), SizeOfOptionalHeader);
      Off += offsetof(PeNtHeaders64, OptionalHeader) + SizeOfOptionalHeader; /* Off = Offset of the first section header */
    }

    else
      assert(0);

    Log::Write("ldr_pe")
      << "- ImageBase: "      << m_ImageBase << ", "
      << "- Section number: " << m_NumberOfSections << LogEnd;

    Address EntryPoint = GetEntryPoint();

    for (i = 0; i < m_NumberOfSections; i++)
    {
      Flags = MemoryArea::Read;
      m_rDoc.GetFileBinaryStream().Read(Off, &sc, sizeof(sc));
      auto VirtualSize = sc.Misc.VirtualSize ? sc.Misc.VirtualSize : sc.SizeOfRawData;

      std::string SectionName(reinterpret_cast<const char *>(sc.Name), 0, PE_SIZEOF_SHORT_NAME);

      if (sc.Characteristics & PE_SCN_MEM_EXECUTE)
        Flags |= MemoryArea::Execute;
      else if (EntryPoint.GetOffset() >= m_ImageBase + sc.VirtualAddress
        &&   EntryPoint.GetOffset() <  m_ImageBase + sc.VirtualAddress + VirtualSize)
      {
        Log::Write("ldr_pe") << "Promote section " << SectionName << " to executable since it contains the entry point" << LogEnd;
        Flags |= MemoryArea::Execute;
      }
      if (sc.Characteristics & PE_SCN_MEM_WRITE)
        Flags |= MemoryArea::Write;

      m_rDoc.AddMemoryArea(new MappedMemoryArea(
        SectionName,
        sc.PointerToRawData, sc.SizeOfRawData,
        Address(Address::FlatType, 0x0, m_ImageBase + sc.VirtualAddress, 16, sizeof(n) * 8),  VirtualSize,
        Flags
        ));

      Log::Write("ldr_pe")
        << "-   Section " << SectionName << " : "
        << "    VAddr: " << m_ImageBase + sc.VirtualAddress
        << "    ROff: "  << sc.PointerToRawData
        << "    Vsize: " << VirtualSize
        << "    Flags: " << sc.Characteristics
        << LogEnd;

      Off += sizeof(sc);
    }

    GetImport();
  }

  void GetImport()
  {
    FileBinaryStream const&       rBinStrm = m_rDoc.GetFileBinaryStream();
    struct ImageImportDescriptor  CurImp, EndImp = { 0 };
    u32                           ImportOff, ImportSize, Off;
    std::string                   DllName;
    TOffset                       tmp;
    u8                            c;

    // Find data directory
    m_rDoc.GetFileBinaryStream().Read(0x3C, Off);
    Off += sizeof(PE_NT_SIGNATURE) + sizeof(struct PeFileHeader);
    Off += (sizeof(n) == sizeof(u32))
      ? offsetof(PeOptionalHeader32, DataDirectory)
      : offsetof(PeOptionalHeader64, DataDirectory);

    // Find Import
    Off += sizeof(PeDataDirectory);
    m_rDoc.GetFileBinaryStream().Read(Off, ImportOff);
    if (ImportOff == 0x0)
      return;
    m_rDoc.ConvertAddressToFileOffset((TOffset) m_ImageBase + ImportOff, tmp);
    ImportOff = static_cast<u32>(tmp);
    m_rDoc.GetFileBinaryStream().Read(Off + sizeof(((PeDataDirectory*)0)->VirtualAddress), ImportSize);

    // For each import descriptor (each dll)
    m_rDoc.GetFileBinaryStream().Read(ImportOff, &CurImp, sizeof(CurImp));
    while (memcmp(&CurImp, &EndImp, sizeof(CurImp)))
    {
      m_rDoc.ConvertAddressToFileOffset((TOffset) m_ImageBase + CurImp.Name, tmp);
      CurImp.Name = static_cast<u32>(tmp);
      if (CurImp.Name)
      {
        DllName.clear();
        do
        {
          m_rDoc.GetFileBinaryStream().Read(CurImp.Name++, c);
          if (c)
            DllName += tolower(c);
        } while (c);
      }

      GetImportedFunctions(DllName, m_ImageBase + (CurImp.OriginalFirstThunk ? CurImp.OriginalFirstThunk : CurImp.FirstThunk), m_ImageBase + CurImp.FirstThunk);

      ImportOff += sizeof(struct ImageImportDescriptor);
      m_rDoc.GetFileBinaryStream().Read(ImportOff, &CurImp, sizeof(CurImp));
    }
  }

  void GetImportedFunctions(std::string &DllName, TOffset OriginalIatVa, TOffset IatVa)
  {
    struct ImageThunkData<n>  CurOriginalThunk, CurThunk, EndThunk = { 0 };
    std::string               FunctionName;
    n                         OrdinalFlag;
    TOffset                   tmp;
    TOffset                   OriginalIatOff, IatOff;
    u8                        c;

    Log::Write("ldr_pe")
      << "DLL: "    << DllName
      << ", OriginalIAT: " << OriginalIatVa
      << ", IAT: " << IatVa
      << LogEnd;
    OrdinalFlag = (sizeof(n) == sizeof(u32)) ? PE_ORDINAL_FLAG32 : PE_ORDINAL_FLAG64;

    // Get IAT raw offset
    m_rDoc.ConvertAddressToFileOffset((TOffset)IatVa, tmp);
    IatOff = tmp;
    m_rDoc.ConvertAddressToFileOffset((TOffset)OriginalIatVa, tmp);
    OriginalIatOff = tmp;

    // For each thunk data (each imported function) of the dll
    m_rDoc.GetFileBinaryStream().Read(IatOff, &CurThunk, sizeof(CurThunk));
    m_rDoc.GetFileBinaryStream().Read(OriginalIatOff, &CurOriginalThunk, sizeof(CurOriginalThunk));
    while (memcmp(&CurThunk, &EndThunk, sizeof(CurThunk)) && memcmp(&CurOriginalThunk, &EndThunk, sizeof(CurOriginalThunk)))
    {
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
        m_rDoc.ConvertAddressToFileOffset((TOffset) m_ImageBase + CurOriginalThunk.Function, tmp);
        CurOriginalThunk.Function = static_cast<n>(tmp + sizeof(((ImageImportByName*)0)->Hint));
        if (CurOriginalThunk.Function)
        {
          do
          {
            m_rDoc.GetFileBinaryStream().Read(CurOriginalThunk.Function++, c);
            if (c)
              FunctionName += c;
          } while (c);
        }
      }

      Address IatAddr(Address::FlatType, 0x0, IatVa, 0, sizeof(n) * 8);
      Log::Write("ldr_pe") << IatVa << ":   " << FunctionName << LogEnd;
      m_rDoc.AddLabel(IatAddr, Label(FunctionName, Label::Code | Label::Imported | Label::Global));
      m_rDoc.ChangeValueSize(IatAddr, IatAddr.GetOffsetSize(), true);

      auto spCell = m_rDoc.GetCell(IatVa);
      if (spCell)
        spCell->SetComment(FunctionName);

      IatOff         += sizeof(struct ImageThunkData<n>);
      OriginalIatOff += sizeof(struct ImageThunkData<n>);
      IatVa          += sizeof(struct ImageThunkData<n>);
      m_rDoc.GetFileBinaryStream().Read(IatOff, &CurThunk, sizeof(CurThunk));
      m_rDoc.GetFileBinaryStream().Read(OriginalIatOff, &CurOriginalThunk, sizeof(CurOriginalThunk));
    }
  }

private:
  Document&               m_rDoc;
  u16                     m_NumberOfSections;
  n                       m_ImageBase;
};

#endif
