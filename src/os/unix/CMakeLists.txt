include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/unix)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/unix)

set(HDR
  ${INCROOT}/unix.hpp
)
set(SRC
  ${SRCROOT}/unix.cpp
  ${SRCROOT}/main.cpp
)

medusa_add_module(os unix ${HDR} ${SRC})