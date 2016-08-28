include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/bs)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/bs)

# all source files
set(HDR
  ${SRCROOT}/bootsector_loader.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/bootsector_loader.cpp
)

medusa_add_module(ldr bs ${HDR} ${SRC})