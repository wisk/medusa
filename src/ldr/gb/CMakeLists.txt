include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/gb)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/gb)

# all source files
set(HDR
  ${INCROOT}/gameboy.h
  ${INCROOT}/gameboy_loader.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/gameboy_loader.cpp
)

medusa_add_module(ldr gb "${HDR}" "${SRC}")