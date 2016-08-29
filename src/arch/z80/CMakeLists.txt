include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/z80)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/z80)

# all source files
set(HDR
  ${INCROOT}/z80_architecture.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/z80_architecture.cpp
  ${SRCROOT}/z80_opcode.cpp
)

medusa_add_module(arch z80 "${HDR}" "${SRC}")
