include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/x86)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/x86)

# all source files
set(HDR
  ${INCROOT}/x86.hpp
  ${INCROOT}/x86_const.hpp
  ${INCROOT}/x86_architecture.hpp

  ${INCROOT}/x86_calling_convention.hpp
)
set(SRC
  ${SRCROOT}/main.cpp

  ${SRCROOT}/x86_architecture.cpp
  ${SRCROOT}/x86_calling_convention.cpp
  ${SRCROOT}/x86_cpu.cpp
  ${SRCROOT}/x86_operand.cpp
  ${SRCROOT}/x86_format.cpp
  ${SRCROOT}/x86_decoder.cpp

  ${SRCROOT}/x86_opcode.ipp
  ${SRCROOT}/x86_mnemonic.cpp
  ${SRCROOT}/x86_opcode_table_1.cpp
  ${SRCROOT}/x86_opcode_table_2.cpp
  ${SRCROOT}/x86_opcode_table_3_38.cpp
  ${SRCROOT}/x86_opcode_table_3_3A.cpp
  ${SRCROOT}/x86_opcode_table_3DNOW1.cpp
  ${SRCROOT}/x86_opcode_table_FP1.cpp
  ${SRCROOT}/x86_opcode_table_FP2.cpp
  ${SRCROOT}/x86_opcode_table_FP3.cpp
  ${SRCROOT}/x86_opcode_table_FP4.cpp
  ${SRCROOT}/x86_opcode_table_FP5.cpp
  ${SRCROOT}/x86_opcode_table_FP6.cpp
  ${SRCROOT}/x86_opcode_table_FP7.cpp
  ${SRCROOT}/x86_opcode_table_FP8.cpp
  ${SRCROOT}/x86_opcode_table_sse5a.cpp
  )

medusa_add_module(arch x86 ${INC} ${SRC})