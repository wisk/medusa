include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/pe)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/pe)

# all source files
set(HDR
  ${INCROOT}/pe.hpp
  ${INCROOT}/pe_loader.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/pe.cpp
  ${SRCROOT}/pe_loader.cpp
)

medusa_add_module(ldr pe "${HDR}" "${SRC}")