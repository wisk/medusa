include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/raw)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/raw)

# all source files
set(HDR
  ${INCROOT}/raw_loader.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/raw_loader.cpp
)

medusa_add_module(ldr raw "${HDR}" "${SRC}")