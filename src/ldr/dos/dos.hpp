#ifndef DOS_HPP
#define	DOS_HPP

#include <medusa/types.hpp>
#include <medusa/endian.hpp>

MEDUSA_NAMESPACE_USE

#define PE_DOS_SIGNATURE               0x5A4D

#define NAME_OVR "MS-DOS executable (perhaps overlayed)"
#define NAME_EXE "MS-DOS executable (EXE)"
#define NAME_DRV "MS-DOS SYS-file (perhaps device driver)"
#define NAME_COM "MS-DOS COM-file"
#define TYPE_OVR 1
#define TYPE_EXE 2
#define TYPE_DRV 3
#define TYPE_COM 4

enum overlay_type {
    overlay_noexe, overlay_pascal, overlay_cpp, overlay_ms
};

struct DosHeader {
    typedef std::shared_ptr<DosHeader> SPType;
    void Swap(EEndianness Endianness);

    enum {
        kMagic = PE_DOS_SIGNATURE
    };

    bool IsValid(void) const {
        return e_magic == kMagic;
    }

    u32 exe_Length(void) {
        u32 len = e_cp * 512L - e_cparhdr * 16;
        if (e_cblp != 0)
            len -= 512 - e_cblp;
        return len;
    }

    u16 e_magic; //0x5A4D (ASCII for 'M' and 'Z') 
    u16 e_cblp; // Number of bytes in the last page. 
    u16 e_cp; // Number of whole/partial pages. 
    u16 e_crlc; // Number of entries in the relocation table. 
    u16 e_cparhdr; // The number of paragraphs taken up by the header. 
    u16 e_minalloc; // The number of paragraphs required by the program, excluding the PSP and program image. If no free block is big enough, the loading stops.
    u16 e_maxalloc; //The number of paragraphs requested by the program. If no free block is big enough, the biggest one possible is allocated. 
    u16 e_ss; //Relocatable segment address for SS. 
    u16 e_sp; //Initial value for SP. 
    u16 e_csum; // When added to the sum of all other words in the file, the result should be zero.
    u16 e_ip; //Initial value for IP. 
    u16 e_cs; //Relocatable segment address for CS. 
    u16 e_lfarlc; // The (absolute) offset to the relocation table. 
    u16 e_ovno; // Value used for overlay management. If zero, this is the main executable. 
};

struct fbov {
    u16 fb;
#define FB_MAGIC 0x4246
    u16 ov;
#define OV_MAGIC 0x564F
    u32 ovrsize;
    u32 exeinfo;
    s32 segnum;
};

struct seginfo {
    u16 seg;
    u16 maxoff; // FFFF - unknown
    u16 flags;
#define SI_COD  0x0001
#define SI_OVR  0x0002
#define SI_DAT  0x0004
    u16 minoff;
};

struct stub_t {
    u8 CDh; // 0
    u8 intnum; // 1
    u16 memswap; // 2
    s32 fileoff; // 4
    u16 codesize; // 8
    u16 relsize; // 10
    u16 nentries; // 12
    u16 prevstub; // 14
#define STUBUNK_SIZE            (0x20-0x10)
    u8 unknown[STUBUNK_SIZE];
};

struct ovrentry {
    u16 int3f;
    u16 off;
    s8 segc;
};

struct ms_entry {
    u8 CDh;
    u8 intnum; //normally 3Fh
    u16 ovr_index;
    u16 entry_off;
};




#endif	/* DOS_HPP */

