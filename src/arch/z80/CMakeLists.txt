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
target_link_libraries(arch_z80 Medusa)

if(WIN32)
  install(TARGETS arch_z80 RUNTIME DESTINATION .)
else()
  install(TARGETS arch_z80 LIBRARY DESTINATION .)
endif()
