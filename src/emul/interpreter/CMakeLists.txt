include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/emul/interpreter)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/emul/interpreter)

set(HDR
  ${INCROOT}/interpreter_emulator.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/interpreter_emulator.cpp
)

medusa_add_module(emul interpreter ${HDR} ${SRC})