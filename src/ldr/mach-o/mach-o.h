#ifndef _MACH_O_
#define _MACH_O_

#include <medusa/types.hpp>


// official doc: https://developer.apple.com/library/mac/documentation/DeveloperTools/Conceptual/MachORuntime/Reference/reference.html

MEDUSA_NAMESPACE_USE

#define MH_MAGIC    0xfeedface
#define MH_MAGIC_64 0xfeedfacf

#define MH_CIGAM    0xcefaedfe
#define MH_CIGAM_64 0xcffaedfe

#define FAT_MAGIC   0xcafebabe
#define FAT_CIGAM   0xbebafeca

struct mach_header {
    u32  magic;
    u32  cputype;
    u32  cpusubtype;
    u32  filetype;
    u32  ncmds;
    u32  sizeofcmds;
    u32  flags;
};

struct mach_header_64 {
    u32  magic;
    u32  cputype;
    u32  cpusubtype;
    u32  filetype;
    u32  ncmds;
    u32  sizeofcmds;
    u32  flags;
    u32  reserved;
};

struct fat_header {
    u32  magic;
    u32  nfat_arch;
};

struct fat_arch {
    u32  cputype;
    u32  cpusubtype;
    u32  offset;
    u32  size;
    u32  align;
};

#define CPU_ARCH_MASK        0xff000000
#define CPU_ARCH_ABI64       0x01000000

#define CPU_TYPE_ANY        ((u32) -1)
#define CPU_TYPE_VAX        ((u32)  1)
#define CPU_TYPE_MC680x0    ((u32)  6)
#define CPU_TYPE_X86        ((u32)  7)
#define CPU_TYPE_I386        CPU_TYPE_X86
#define CPU_TYPE_X86_64     (CPU_TYPE_X86 | CPU_ARCH_ABI64)
#define CPU_TYPE_MIPS       ((u32)  8)
#define CPU_TYPE_MC98000    ((u32) 10)
#define CPU_TYPE_HPPA       ((u32) 11)
#define CPU_TYPE_ARM        ((u32) 12)
#define CPU_TYPE_MC88000    ((u32) 13)
#define CPU_TYPE_SPARC      ((u32) 14)
#define CPU_TYPE_I860       ((u32) 15)
#define CPU_TYPE_ALPHA      ((u32) 16)
#define CPU_TYPE_POWERPC    ((u32) 18)
#define CPU_TYPE_POWERPC64  (CPU_TYPE_POWERPC | CPU_ARCH_ABI64)

#define MH_OBJECT      0x1
#define MH_EXECUTE     0x2
#define MH_FVMLIB      0x3
#define MH_CORE        0x4
#define MH_PRELOAD     0x5
#define MH_DYLIB       0x6
#define MH_DYLINKER    0x7
#define MH_BUNDLE      0x8
#define MH_DYLIB_STUB  0x9
#define MH_DSYM        0xa
#define MH_KEXT_BUNDLE 0xb

struct load_command {
    u32  cmd;
    u32  cmdsize;
};

#define LC_REQ_DYLD             0x80000000

#define LC_SEGMENT              0x1
#define LC_SYMTAB               0x2
#define LC_SYMSEG               0x3
#define LC_THREAD               0x4
#define LC_UNIXTHREAD           0x5
#define LC_LOADFVMLIB           0x6
#define LC_IDFVMLIB             0x7
#define LC_IDENT                0x8
#define LC_FVMFILE              0x9
#define LC_PREPAGE              0xa
#define LC_DYSYMTAB             0xb
#define LC_LOAD_DYLIB           0xc
#define LC_ID_DYLIB             0xd
#define LC_LOAD_DYLINKER        0xe
#define LC_ID_DYLINKER          0xf
#define LC_PREBOUND_DYLIB       0x10
#define LC_ROUTINES             0x11
#define LC_SUB_FRAMEWORK        0x12
#define LC_SUB_UMBRELLA         0x13
#define LC_SUB_CLIENT           0x14
#define LC_SUB_LIBRARY          0x15
#define LC_TWOLEVEL_HINTS       0x16
#define LC_PREBIND_CKSUM        0x17
#define LC_LOAD_WEAK_DYLIB     (0x18 | LC_REQ_DYLD)
#define LC_SEGMENT_64           0x19
#define LC_ROUTINES_64          0x1a
#define LC_UUID                 0x1b
#define LC_RPATH               (0x1c | LC_REQ_DYLD)
#define LC_CODE_SIGNATURE       0x1d
#define LC_SEGMENT_SPLIT_INFO   0x1e
#define LC_REEXPORT_DYLIB      (0x1f | LC_REQ_DYLD)
#define LC_LAZY_LOAD_DYLIB      0x20
#define LC_ENCRYPTION_INFO      0x21
#define LC_DYLD_INFO            0x22
#define LC_DYLD_INFO_ONLY      (0x22 | LC_REQ_DYLD)
#define LC_LOAD_UPWARD_DYLIB   (0x23 | LC_REQ_DYLD)
#define LC_VERSION_MIN_MACOSX   0x24
#define LC_VERSION_MIN_IPHONEOS 0x25
#define LC_FUNCTION_STARTS      0x26
#define LC_DYLD_ENVIRONMENT     0x27
#define LC_MAIN                (0x28 | LC_REQ_DYLD)
#define LC_DATA_IN_CODE         0x29
#define LC_SOURCE_VERSION       0x2A
#define LC_DYLIB_CODE_SIGN_DRS  0x2B
#define LC_ENCRYPTION_INFO_64   0x2C

struct segment_command {
    u32  cmd;
    u32  cmdsize;
    u8   segname[16];
    u32  vmaddr;
    u32  vmsize;
    u32  fileoff;
    u32  filesize;
    u32  maxprot;
    u32  initprot;
    u32  nsects;
    u32  flags;
};

struct segment_command_64 {
    u32  cmd;
    u32  cmdsize;
    u8   segname[16];
    u64  vmaddr;
    u64  vmsize;
    u64  fileoff;
    u64  filesize;
    u32  maxprot;
    u32  initprot;
    u32  nsects;
    u32  flags;
};

#define VM_PROT_NONE    0x00
#define VM_PROT_READ    0x01
#define VM_PROT_WRITE   0x02
#define VM_PROT_EXECUTE 0x04

struct section {
    u8   sectname[16];
    u8   segname[16];
    u32  addr;
    u32  size;
    u32  offset;
    u32  align;
    u32  reloff;
    u32  nreloc;
    u32  flags;
    u32  reserved1;
    u32  reserved2;
};

struct section_64 {
    u8   sectname[16];
    u8   segname[16];
    u64  addr;
    u64  size;
    u32  offset;
    u32  align;
    u32  reloff;
    u32  nreloc;
    u32  flags;
    u32  reserved1;
    u32  reserved2;
    u32  reserved3;
};

enum {
  // TypeAndAttributes bitmasks.
  SECTION_TYPE       = 0x000000FFU,
  SECTION_ATTRIBUTES = 0xFFFFFF00U,
  
  // Valid section types.
  
  /// S_REGULAR - Regular section.
  S_REGULAR                    = 0x00U,
  /// S_ZEROFILL - Zero fill on demand section.
  S_ZEROFILL                   = 0x01U,
  /// S_CSTRING_LITERALS - Section with literal C strings.
  S_CSTRING_LITERALS           = 0x02U,
  /// S_4BYTE_LITERALS - Section with 4 byte literals.
  S_4BYTE_LITERALS             = 0x03U,
  /// S_8BYTE_LITERALS - Section with 8 byte literals.
  S_8BYTE_LITERALS             = 0x04U,
  /// S_LITERAL_POINTERS - Section with pointers to literals.
  S_LITERAL_POINTERS           = 0x05U,
  /// S_NON_LAZY_SYMBOL_POINTERS - Section with non-lazy symbol pointers.
  S_NON_LAZY_SYMBOL_POINTERS   = 0x06U,
  /// S_LAZY_SYMBOL_POINTERS - Section with lazy symbol pointers.
  S_LAZY_SYMBOL_POINTERS       = 0x07U,
  /// S_SYMBOL_STUBS - Section with symbol stubs, byte size of stub in
  /// the Reserved2 field.
  S_SYMBOL_STUBS               = 0x08U,
  /// S_SYMBOL_STUBS - Section with only function pointers for
  /// initialization.
  S_MOD_INIT_FUNC_POINTERS     = 0x09U,
  /// S_MOD_INIT_FUNC_POINTERS - Section with only function pointers for
  /// termination.
  S_MOD_TERM_FUNC_POINTERS     = 0x0AU,
  /// S_COALESCED - Section contains symbols that are to be coalesced.
  S_COALESCED                  = 0x0BU,
  /// S_GB_ZEROFILL - Zero fill on demand section (that can be larger than 4
  /// gigabytes).
  S_GB_ZEROFILL                = 0x0CU,
  /// S_INTERPOSING - Section with only pairs of function pointers for
  /// interposing.
  S_INTERPOSING                = 0x0DU,
  /// S_16BYTE_LITERALS - Section with only 16 byte literals.
  S_16BYTE_LITERALS            = 0x0EU,
  /// S_DTRACE_DOF - Section contains DTrace Object Format.
  S_DTRACE_DOF                 = 0x0FU,
  /// S_LAZY_DYLIB_SYMBOL_POINTERS - Section with lazy symbol pointers to
  /// lazy loaded dylibs.
  S_LAZY_DYLIB_SYMBOL_POINTERS = 0x10U,

  LAST_KNOWN_SECTION_TYPE = S_LAZY_DYLIB_SYMBOL_POINTERS,
  

  // Valid section attributes.
  
  /// S_ATTR_PURE_INSTRUCTIONS - Section contains only true machine
  /// instructions.
  S_ATTR_PURE_INSTRUCTIONS   = 1U << 31,
  /// S_ATTR_NO_TOC - Section contains coalesced symbols that are not to be
  /// in a ranlib table of contents.
  S_ATTR_NO_TOC              = 1U << 30,
  /// S_ATTR_STRIP_STATIC_SYMS - Ok to strip static symbols in this section
  /// in files with the MY_DYLDLINK flag.
  S_ATTR_STRIP_STATIC_SYMS   = 1U << 29,
  /// S_ATTR_NO_DEAD_STRIP - No dead stripping.
  S_ATTR_NO_DEAD_STRIP       = 1U << 28,
  /// S_ATTR_LIVE_SUPPORT - Blocks are live if they reference live blocks.
  S_ATTR_LIVE_SUPPORT        = 1U << 27,
  /// S_ATTR_SELF_MODIFYING_CODE - Used with i386 code stubs written on by
  /// dyld.
  S_ATTR_SELF_MODIFYING_CODE = 1U << 26,
  /// S_ATTR_DEBUG - A debug section.
  S_ATTR_DEBUG               = 1U << 25,
  /// S_ATTR_SOME_INSTRUCTIONS - Section contains some machine instructions.
  S_ATTR_SOME_INSTRUCTIONS   = 1U << 10,
  /// S_ATTR_EXT_RELOC - Section has external relocation entries.
  S_ATTR_EXT_RELOC           = 1U << 9,
  /// S_ATTR_LOC_RELOC - Section has local relocation entries.
  S_ATTR_LOC_RELOC           = 1U << 8
};

struct symtab_command {
    u32  cmd;
    u32  cmdsize;
    u32  symoff;
    u32  nsyms;
    u32  stroff;
    u32  strsize;
};

struct nlist {
    u32  n_strx; // according to the official doc, it's a s32, but it's easier to assume u32
    u8   n_type;
    u8   n_sect;
    s16  n_desc;
    u32  n_value;
};

struct nlist_64 {
    u32  n_strx;
    u8   n_type;
    u8   n_sect;
    u16  n_desc;
    u64  n_value;
};

union lc_str {
    u32 offset;
};

struct dylib {
    union lc_str  name;
    u32           timestamp;
    u32           current_version;
    u32           compatibility_version;
};

struct dylib_command {
    u32           cmd;
    u32           cmdsize;
    struct dylib  dylib;
};

struct dysymtab_command {
    u32  cmd;
    u32  cmdsize;
    u32  ilocalsym;
    u32  nlocalsym;
    u32  iextdefsym;
    u32  nextdefsym;
    u32  iundefsym;
    u32  nundefsym;
    u32  tocoff;
    u32  ntoc;
    u32  modtaboff;
    u32  nmodtab;
    u32  extrefsymoff;
    u32  nextrefsyms;
    u32  indirectsymoff;
    u32  nindirectsyms;
    u32  extreloff;
    u32  nextrel;
    u32  locreloff;
    u32  nlocrel;
};

struct thread_command {
    u32  cmd;
    u32  cmdsize;
};

#define x86_THREAD_STATE32    1
#define x86_FLOAT_STATE32     2
#define x86_EXCEPTION_STATE32 3
#define x86_THREAD_STATE64    4
#define x86_FLOAT_STATE64     5
#define x86_EXCEPTION_STATE64 6
#define x86_THREAD_STATE      7
#define x86_FLOAT_STATE       8
#define x86_EXCEPTION_STATE   9
#define x86_DEBUG_STATE32     10
#define x86_DEBUG_STATE64     11
#define x86_DEBUG_STATE       12
#define THREAD_STATE_NONE     13
#define x86_AVX_STATE32       16
#define x86_AVX_STATE64       17
#define x86_AVX_STATE         18

struct x86_thread_state32_t {
    u32  eax;
    u32  ebx;
    u32  ecx;
    u32  edx;
    u32  edi;
    u32  esi;
    u32  ebp;
    u32  esp;
    u32  ss;
    u32  eflags;
    u32  eip;
    u32  cs;
    u32  ds;
    u32  es;
    u32  fs;
    u32  gs;
};

struct x86_thread_state64_t {
    u64  rax;
    u64  rbx;
    u64  rcx;
    u64  rdx;
    u64  rdi;
    u64  rsi;
    u64  rbp;
    u64  rsp;
    u64  r8;
    u64  r9;
    u64  r10;
    u64  r11;
    u64  r12;
    u64  r13;
    u64  r14;
    u64  r15;
    u64  rip;
    u64  rflags;
    u64  cs;
    u64  fs;
    u64  gs;
};

struct x86_state_hdr {
    u32  flavor;
    u32  count;
};

struct entry_point_command {
    u32  cmd;
    u32  cmdsize;
    u64  entryoff;
    u64  stacksize;
};

#endif // _MACH_O_
