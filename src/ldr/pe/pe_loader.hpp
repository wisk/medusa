#ifndef _PE_LOADER_
#define _PE_LOADER_

#include <medusa/document.hpp>
#include <medusa/loader.hpp>
#include <medusa/log.hpp>

#include "pe.hpp"

#if defined(_WIN32) || defined(WIN32)
#ifdef ldr_pe_EXPORTS
#  define LDR_PE_EXPORT __declspec(dllexport)
#else
#  define LDR_PE_EXPORT __declspec(dllimport)
#endif
#else
#define LDR_PE_EXPORT
#endif

MEDUSA_NAMESPACE_USE

class PeLoader : public Loader
{
public:
  PeLoader(void);

  virtual std::string             GetName(void) const;
  virtual bool                    IsCompatible(BinaryStream const& rBinStrm);
  virtual void                    Map(Document& rDoc);
  virtual Architecture::SharedPtr GetMainArchitecture(Architecture::VectorSharedPtr const& rArchitectures);
  virtual void                    Configure(Configuration& rCfg);

private:
  u16 m_Machine;
  u16 m_Magic;

  template<typename bit> void Map(Document& rDoc)
  {
    BinaryStream const& rBinStrm = rDoc.GetBinaryStream();
    PeSectionHeader sc;
    u16 NumberOfSections;
    bit ImageBase;
    u32 EntryPoint;
    u16 SizeOfOptionalHeader;
    u32 PeOff, Off, Flags;
    int i;

    if (sizeof(bit) == sizeof(u32))
    {
      rBinStrm.Read(offsetof(PeDosHeader, e_lfanew), PeOff);
      rBinStrm.Read(PeOff + offsetof(PeNtHeaders32, FileHeader.NumberOfSections), NumberOfSections);
      rBinStrm.Read(PeOff + offsetof(PeNtHeaders32, OptionalHeader.ImageBase), ImageBase);
      rBinStrm.Read(PeOff + offsetof(PeNtHeaders32, OptionalHeader.AddressOfEntryPoint), EntryPoint);
      rBinStrm.Read(PeOff + offsetof(PeNtHeaders32, FileHeader.SizeOfOptionalHeader), SizeOfOptionalHeader);
      Off = PeOff + offsetof(PeNtHeaders32, OptionalHeader) + SizeOfOptionalHeader; /* Off = Offset of the first section header */
    }

    else if (sizeof(bit) == sizeof(u64))
    {
      rBinStrm.Read(offsetof(PeDosHeader, e_lfanew), PeOff);
      rBinStrm.Read(Off + offsetof(PeNtHeaders64, FileHeader.NumberOfSections), NumberOfSections);
      rBinStrm.Read(Off + offsetof(PeNtHeaders64, OptionalHeader.ImageBase), ImageBase);
      rBinStrm.Read(Off + offsetof(PeNtHeaders64, OptionalHeader.AddressOfEntryPoint), EntryPoint);
      rBinStrm.Read(Off + offsetof(PeNtHeaders64, FileHeader.SizeOfOptionalHeader), SizeOfOptionalHeader);
      Off = PeOff + offsetof(PeNtHeaders64, OptionalHeader) + SizeOfOptionalHeader; /* Off = Offset of the first section header */
    }

    else
      assert(0);

    Log::Write("ldr_pe")
      << "- ImageBase: "      << ImageBase << ", "
      << "- EntryPoint: "     << ImageBase + EntryPoint << ", "
      << "- Section number: " << NumberOfSections << LogEnd;

    Address EntryPointAddress = Address(Address::FlatType, 0x0, ImageBase + EntryPoint, 0x0, sizeof(bit) * 8);
    rDoc.AddLabel(EntryPointAddress, Label("start", Label::Code | Label::Global | Label::Exported));

    for (i = 0; i < NumberOfSections; i++)
    {
      Flags = MemoryArea::Read;
      if (!rDoc.GetBinaryStream().Read(Off, &sc, sizeof(sc)))
      {
        Log::Write("ldr_pe") << "Can't read SCNHDR" << LogEnd;
        continue;
      }
      auto VirtualSize = sc.Misc.VirtualSize ? sc.Misc.VirtualSize : sc.SizeOfRawData;

      std::string SectionName(reinterpret_cast<const char *>(sc.Name), 0, PE_SIZEOF_SHORT_NAME);

      if (sc.Characteristics & PE_SCN_MEM_EXECUTE)
        Flags |= MemoryArea::Execute;
      else if (EntryPointAddress.GetOffset() >= ImageBase + sc.VirtualAddress
        &&   EntryPointAddress.GetOffset() <  ImageBase + sc.VirtualAddress + VirtualSize)
      {
        Log::Write("ldr_pe") << "Promote section " << SectionName << " to executable since it contains the entry point" << LogEnd;
        Flags |= MemoryArea::Execute;
      }
      if (sc.Characteristics & PE_SCN_MEM_WRITE)
        Flags |= MemoryArea::Write;

      rDoc.AddMemoryArea(new MappedMemoryArea(
        SectionName,
        sc.PointerToRawData, sc.SizeOfRawData,
        Address(Address::FlatType, 0x0, ImageBase + sc.VirtualAddress, 16, sizeof(bit) * 8),  VirtualSize,
        Flags
        ));

      Log::Write("ldr_pe")
        << "-   Section " << SectionName << " : "
        << "    VAddr: " << ImageBase + sc.VirtualAddress
        << "    ROff: "  << sc.PointerToRawData
        << "    Vsize: " << VirtualSize
        << "    Flags: " << sc.Characteristics
        << LogEnd;

      Off += sizeof(sc);
    }

    // Get imports
    struct ImageImportDescriptor  CurImp, EndImp = { 0 };
    u32                           ImportOff, ImportSize;
    std::string                   DllName;
    TOffset                       tmp;
    u8                            c;

    // Find data directory
    Off = PeOff + sizeof(PE_NT_SIGNATURE) + sizeof(struct PeFileHeader);
    Off += (sizeof(bit) == sizeof(u32))
      ? offsetof(PeOptionalHeader32, DataDirectory)
      : offsetof(PeOptionalHeader64, DataDirectory);

    // Find Import
    Off += sizeof(PeDataDirectory);
    if (!rBinStrm.Read(Off, ImportOff))
    {
      Log::Write("ldr_pe") << "Can't read IMPORT" << LogEnd;
      return;
    }
    if (ImportOff == 0x0)
      return;
    if (!rDoc.ConvertAddressToFileOffset((TOffset) ImageBase + ImportOff, tmp))
    {
      Log::Write("ldr_pe") << "Can't convert IMPORT" << LogEnd;
      return;
    }
    ImportOff = static_cast<u32>(tmp);
    if (!rBinStrm.Read(Off + sizeof(((PeDataDirectory*)0)->VirtualAddress), ImportSize))
    {
      Log::Write("ldr_pe") << "Can't read IMPORTVA" << LogEnd;
      return;
    }

    // For each import descriptor (each dll)
    if (!rBinStrm.Read(ImportOff, &CurImp, sizeof(CurImp)))
    {
      Log::Write("ldr_pe") << "Can't read IMPORTDESC" << LogEnd;
      return;
    }
    while (memcmp(&CurImp, &EndImp, sizeof(CurImp)))
    {
      if (!rDoc.ConvertAddressToFileOffset((TOffset)ImageBase + CurImp.Name, tmp))
      {
        Log::Write("ldr_pe") << "Can't convert IMPORTNAME" << LogEnd;
        return;
      }
      CurImp.Name = static_cast<u32>(tmp);
      if (CurImp.Name)
      {
        DllName.clear();
        do
        {
          rBinStrm.Read(CurImp.Name++, c); // TODO: Implement Read string
          if (c)
            DllName += tolower(c);
        } while (c);
      }

      TOffset OriginalIatVa = ImageBase + (CurImp.OriginalFirstThunk ? CurImp.OriginalFirstThunk : CurImp.FirstThunk);
      TOffset IatVa = ImageBase + CurImp.FirstThunk;
      struct ImageThunkData<bit>  CurOriginalThunk, CurThunk, EndThunk = { 0 };
      std::string               FunctionName;
      bit                       OrdinalFlag;
      TOffset                   tmp;
      TOffset                   OriginalIatOff, IatOff;
      u8                        c;

      Log::Write("ldr_pe")
        << "DLL: "    << DllName
        << ", OriginalIAT: " << OriginalIatVa
        << ", IAT: " << IatVa
        << LogEnd;
      OrdinalFlag = (sizeof(bit) == sizeof(u32)) ? PE_ORDINAL_FLAG32 : PE_ORDINAL_FLAG64;

      // Get IAT raw offset
      if (!rDoc.ConvertAddressToFileOffset((TOffset)IatVa, tmp))
      {
        Log::Write("ldr_pe") << "Can't convert IAT" << LogEnd;
        return;
      }
      IatOff = tmp;
      if (!rDoc.ConvertAddressToFileOffset((TOffset)OriginalIatVa, tmp))
      {
        Log::Write("ldr_pe") << "Can't convert OIAT" << LogEnd;
        return;
      }
      OriginalIatOff = tmp;

      // TODO: Check returned value of Read

      // For each thunk data (each imported function) of the dll
      rBinStrm.Read(IatOff, &CurThunk, sizeof(CurThunk));
      rBinStrm.Read(OriginalIatOff, &CurOriginalThunk, sizeof(CurOriginalThunk));
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
          rDoc.ConvertAddressToFileOffset((TOffset)ImageBase + CurOriginalThunk.Function, tmp);
          CurOriginalThunk.Function = static_cast<bit>(tmp + sizeof(((ImageImportByName*)0)->Hint));
          if (CurOriginalThunk.Function)
          {
            do
            {
              rBinStrm.Read(CurOriginalThunk.Function++, c);
              if (c)
                FunctionName += c;
            } while (c);
          }
        }

        Address IatAddr(Address::FlatType, 0x0, IatVa, 0, sizeof(bit) * 8);
        Log::Write("ldr_pe") << IatVa << ":   " << FunctionName << LogEnd;
        rDoc.AddLabel(IatAddr, Label(FunctionName, Label::Code | Label::Imported | Label::Global));
        rDoc.ChangeValueSize(IatAddr, IatAddr.GetOffsetSize(), true);

        //auto spCell = m_rDoc.GetCell(IatVa);
        //if (spCell)
        //  spCell->SetComment(FunctionName);

        IatOff         += sizeof(struct ImageThunkData<bit>);
        OriginalIatOff += sizeof(struct ImageThunkData<bit>);
        IatVa          += sizeof(struct ImageThunkData<bit>);
        rBinStrm.Read(IatOff, &CurThunk, sizeof(CurThunk));
        rBinStrm.Read(OriginalIatOff, &CurOriginalThunk, sizeof(CurOriginalThunk));
      }

      ImportOff += sizeof(struct ImageImportDescriptor);
      rBinStrm.Read(ImportOff, &CurImp, sizeof(CurImp));
    }
  }
};

extern "C" LDR_PE_EXPORT Loader* GetLoader(void);

#endif