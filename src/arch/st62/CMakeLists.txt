include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/st62)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/st62)

# all source files
set(HDR
  ${INCROOT}/st62_architecture.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/st62_architecture.cpp
  ${SRCROOT}/st62_decoder.cpp
  ${SRCROOT}/st62_opcode.cpp
  ${SRCROOT}/st62_operand.cpp
  ${SRCROOT}/st62_mnemonic.cpp
)

medusa_add_module(arch st62 "${HDR}" "${SRC}")
