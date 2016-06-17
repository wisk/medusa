set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/z80)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/z80)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/z80_architecture.hpp
  ${SRCROOT}/z80_architecture.cpp
  ${SRCROOT}/z80_opcode.cpp
  )

# define z80 architecture target
add_library(arch_z80 SHARED ${SRC})
target_link_libraries(arch_z80 medusa)
set_target_properties(arch_z80 PROPERTIES
  FOLDER "Architectures"
  PREFIX "")

if(WIN32)
  install(TARGETS arch_z80 RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS arch_z80 LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
