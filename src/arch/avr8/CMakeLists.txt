include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/avr8)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/avr8)

# all source files
set(HDR
  ${INCROOT}/avr8_architecture.hpp
  ${INCROOT}/avr8_instruction.hpp
  ${INCROOT}/avr8_register.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/avr8_architecture.cpp
)

medusa_add_module(arch avr8 ${INC} ${SRC})