set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/st62)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/st62)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/st62_architecture.hpp
  ${SRCROOT}/st62_architecture.cpp
  ${SRCROOT}/st62_opcode.cpp
  ${SRCROOT}/st62_operand.cpp
  )

# define z80 architecture target
add_library(arch_st62 SHARED ${SRC})
target_link_libraries(arch_st62 medusa)
set_target_properties(arch_st62 PROPERTIES
  FOLDER "Architectures"
  PREFIX "")

if(WIN32)
  install(TARGETS arch_st62 RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS arch_st62 LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
