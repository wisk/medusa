set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/avr8)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/avr8)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/avr8_architecture.hpp
  ${SRCROOT}/avr8_architecture.cpp
  ${INCROOT}/avr8_instruction.hpp
  ${INCROOT}/avr8_register.hpp
  )

# define avr8 architecture target
add_library(arch_avr8 SHARED ${SRC})
target_link_libraries(arch_avr8 medusa)
set_target_properties(arch_avr8 PROPERTIES
  FOLDER "Architectures"
  PREFIX "")

if(WIN32)
  install(TARGETS arch_avr8 RUNTIME DESTINATION lib)
else()
  install(TARGETS arch_avr8 LIBRARY DESTINATION lib)
endif()
