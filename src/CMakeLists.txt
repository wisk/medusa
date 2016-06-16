macro(medusa_include_module_if_needed module_type module_name)
  set(module_path "${CMAKE_CURRENT_SOURCE_DIR}/${module_type}/${module_name}")
  if ("${MEDUSA_MODULES_TO_BUILD}" MATCHES "all")
    add_subdirectory("${module_path}")
  else()
    list(FIND MEDUSA_MODULES_TO_BUILD "${module_type}_${module_name}" module_found)
    if (NOT "${module_found}" EQUAL "-1")
      add_subdirectory("${module_path}")
      list(REMOVE_AT MEDUSA_MODULES_TO_BUILD ${module_found})
      set(MEDUSA_MODULES_TO_BUILD "${MEDUSA_MODULES_TO_BUILD}")
    endif()
  endif()
endmacro()

add_subdirectory(core)
add_subdirectory(tools)

# architectures

medusa_include_module_if_needed(arch arm)          # ARM
#medusa_include_module_if_needed(arch avr8)         # AVR8
medusa_include_module_if_needed(arch st62)         # ST62
medusa_include_module_if_needed(arch x86)          # X86
medusa_include_module_if_needed(arch z80)          # Zilog80 (GameBoy)

# binding

medusa_include_module_if_needed(bind python)       # Python

# database

medusa_include_module_if_needed(db text)           # Text

# emulation

medusa_include_module_if_needed(emul interpreter)  # Interpreter
medusa_include_module_if_needed(emul llvm)         # LLVM

# loader

medusa_include_module_if_needed(ldr bs)            # BootSector
medusa_include_module_if_needed(ldr elf)           # ELF
medusa_include_module_if_needed(ldr gb)            # GameBoy ROM
medusa_include_module_if_needed(ldr mach-o)        # Mach-O
medusa_include_module_if_needed(ldr pe)            # PE
medusa_include_module_if_needed(ldr raw)           # Raw
medusa_include_module_if_needed(ldr st62)          # ST62

# operating system

medusa_include_module_if_needed(os unix)           # UNIX
medusa_include_module_if_needed(os windows)        # Windows

# test

if (MEDUSA_BUILD_TESTS)
  add_subdirectory(test)
endif()

if (NOT "${MEDUSA_MODULES_TO_BUILD}" MATCHES "all")
  foreach (module_name ${MEDUSA_MODULES_TO_BUILD})
    message(WARNING "Unable to find module ${module_name}")
  endforeach()
endif()
