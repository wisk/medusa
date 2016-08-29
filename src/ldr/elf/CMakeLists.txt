include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/elf)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/elf)

# all source files
set(HDR
  ${INCROOT}/elf.h
  ${INCROOT}/elf_traits.hpp
  ${INCROOT}/elf_loader.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/elf_loader.cpp
)

medusa_add_module(ldr elf "${HDR}" "${SRC}")