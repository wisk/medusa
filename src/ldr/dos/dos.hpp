#ifndef DOS_HPP
#define	DOS_HPP

#include <medusa/types.hpp>
#include <medusa/endian.hpp>

MEDUSA_NAMESPACE_USE

#define PE_DOS_SIGNATURE               0x5A4D

#define NAME_OVR "MS-DOS executable (perhaps overlayed)"
#define NAME_EXE "MS-DOS executable (EXE)"
#define NAME_DRV "MS-DOS SYS-file (perhaps device driver)"

struct DosHeader {
    void Swap(EEndianness Endianness);
    enum {
        kMagic = PE_DOS_SIGNATURE
    };
    bool IsValid(void) const {
        return e_magic == kMagic;
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




#endif	/* DOS_HPP */

