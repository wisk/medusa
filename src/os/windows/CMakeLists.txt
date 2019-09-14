include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)

# find_package(SQLite3 CONFIG)

# all source files
set(HDR
  ${INCROOT}/windows.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/windows.cpp
)

medusa_add_module(os windows "${HDR}" "${SRC}")
# add_definitions(os_windows SQLite3)
# target_link_libraries(os_windows sqlite3)