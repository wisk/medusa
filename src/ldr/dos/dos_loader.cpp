#include "medusa/medusa.hpp"
#include <boost/foreach.hpp>

#include "dos_loader.hpp"

std::string DosLoader::GetName(void) const {
    switch (DosFiletype) {
        case TYPE_OVR: return NAME_OVR;
        case TYPE_EXE: return NAME_EXE;
        case TYPE_DRV: return NAME_DRV;
        case TYPE_COM: return NAME_COM;
        default: return "MS-DOS (unknown)";
    }
}

bool DosLoader::IsCompatible(BinaryStream const& rBinStrm) {
    //type = TYPE_EXE;
    if (rBinStrm.GetSize() < sizeof (DosHeader))
        return false;
    DosHeader DosHdr;
    if (!rBinStrm.Read(0x0, &DosHdr, sizeof (DosHdr)))
        return false;
    DosHdr.Swap(LittleEndian);
    if (!DosHdr.IsValid()) return false;
    if ((DosHdr.e_cparhdr * 16)<sizeof (DosHdr))
        return false;

    if (DosHdr.e_crlc != 0) {
        if (DosHdr.e_lfarlc + (DosHdr.e_crlc * 4) > DosHdr.e_cparhdr * 16 ||
                (DosHdr.e_lfarlc && DosHdr.e_lfarlc < sizeof (DosHdr)) ||
                !DosHdr.e_lfarlc) return false;
    }

    std::string file_ext = rBinStrm.GetPath().extension().string();
    Log::Write("ldr_dos") << "file_ext: " << file_ext << LogEnd;



    return true;
}

void DosLoader::Map(Document& rDoc, Architecture::VSPType const& rArchs) {

    BinaryStream const& rBinStrm = rDoc.GetBinaryStream();

    DosHeader DosHdr;

    if (!rBinStrm.Read(0x0, &DosHdr, sizeof (DosHdr))) {
        Log::Write("ldr_pe") << "unable to read DOS header" << LogEnd;
        return;
    }
    DosHdr.Swap(LittleEndian);

    rDoc.AddMemoryArea(new MappedMemoryArea(
            "raw",
            0x0, rDoc.GetBinaryStream().GetSize(),
            Address(Address::FlatType, 0x0), rDoc.GetBinaryStream().GetSize(),
            MemoryArea::Execute | MemoryArea::Read | MemoryArea::Write
            ));
}

void DosLoader::FilterAndConfigureArchitectures(Architecture::VSPType& rArchs) const {

}

overlay_type DosLoader::_PrepareOverlayType(Document& rDoc, DosHeader::SPType& DHeader) {
    BinaryStream const& rBinStrm = rDoc.GetBinaryStream();
    u32 flen = rBinStrm.GetSize();
    u32 fbovoff;
    fbov fbov;


    //rBinStrm.Read(fbovoff, &DosHdr, sizeof (DosHdr)


}
