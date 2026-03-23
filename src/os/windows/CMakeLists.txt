include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)

find_package(unofficial-sqlite3 CONFIG REQUIRED)

# all source files
set(HDR
  ${INCROOT}/windows.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/windows.cpp
)

medusa_add_module(os windows "${HDR}" "${SRC}")
target_link_libraries(os_windows unofficial::sqlite3::sqlite3)