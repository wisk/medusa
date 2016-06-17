set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/x86)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/x86)

include_directories(${INCROOT})

# all source files
set(SRC
  ${SRCROOT}/main.cpp

  ${INCROOT}/x86.hpp
  ${INCROOT}/x86_const.hpp
  ${INCROOT}/x86_architecture.hpp

  ${INCROOT}/x86_calling_convention.hpp

  #${INCROOT}/x86_opcode.ipp
  #${INCROOT}/x86_operand.ipp

  ${SRCROOT}/x86_architecture.cpp
  ${SRCROOT}/x86_calling_convention.cpp
  ${SRCROOT}/x86_cpu.cpp
  ${SRCROOT}/x86_operand.cpp
  ${SRCROOT}/x86_format.cpp

  ${SRCROOT}/x86_opcode.ipp
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

# define x86 architecture target
add_library(arch_x86 SHARED ${SRC})
target_link_libraries(arch_x86 medusa)
set_target_properties(arch_x86 PROPERTIES
  FOLDER "Architectures"
  PREFIX "")

if(WIN32)
  install(TARGETS arch_x86 RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS arch_x86 LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()

