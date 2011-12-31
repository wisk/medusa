set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/gb)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/gb)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/gameboy_architecture.hpp
  ${SRCROOT}/gameboy_architecture.cpp
  ${INCROOT}/gameboy_instruction.hpp
  ${INCROOT}/gameboy_register.hpp
  ${SRCROOT}/gameboy_opcode.cpp
  )

# define gb architecture target
add_library(arch_gb SHARED ${SRC})
target_link_libraries(arch_gb Medusa)

if(WIN32)
  install(TARGETS arch_gb RUNTIME DESTINATION .)
else()
  install(TARGETS arch_gb LIBRARY DESTINATION .)
endif()
