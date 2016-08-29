include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/mach-o)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/mach-o)

# all source files
set(HDR
  ${INCROOT}/mach-o_loader.hpp
  ${INCROOT}/mach-o_traits.hpp
  ${INCROOT}/mach-o.h
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/mach-o_loader.cpp
)

medusa_add_module(ldr mach-o "${HDR}" "${SRC}")