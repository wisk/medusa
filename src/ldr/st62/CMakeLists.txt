include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/st62)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/st62)

# all source files
set(HDR
  ${INCROOT}/st62_loader.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/st62_loader.cpp
)

medusa_add_module(ldr st62 "${HDR}" "${SRC}")