# add architecture directories
if (NOT MEDUSA_BUILD_X86_ARCH_AND_LOADER_ONLY)
  add_subdirectory(gb)	  # GameBoy
  add_subdirectory(bs)	  # Boot Sector
  add_subdirectory(raw)   # Raw
  add_subdirectory(mach-o)# Mach-o (Mach Object file format)
  add_subdirectory(st62)  # ST62 ROM
endif()
add_subdirectory(pe)	  # PE  (Portable Executable)
add_subdirectory(elf)	  # ELF (eXecutable Linking Format)
