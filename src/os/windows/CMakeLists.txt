include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)

# all source files
set(HDR
  ${INCROOT}/windows.hpp
  ${CMAKE_SOURCE_DIR}/deps/sqlite3/sqlite3.h
  ${CMAKE_SOURCE_DIR}/deps/sqlite3/sqlite3ext.h
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/windows.cpp

  ${CMAKE_SOURCE_DIR}/deps/sqlite3/sqlite3.c
)

medusa_add_module(os windows ${HDR} ${SRC})