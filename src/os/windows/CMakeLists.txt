set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)

# all source files
set(SRC
  ${INCROOT}/windows.hpp

  ${SRCROOT}/main.cpp
  ${SRCROOT}/windows.cpp

  ${CMAKE_SOURCE_DIR}/deps/sqlite3/sqlite3.h
  ${CMAKE_SOURCE_DIR}/deps/sqlite3/sqlite3ext.h
  ${CMAKE_SOURCE_DIR}/deps/sqlite3/sqlite3.c
  )

include_directories("${CMAKE_SOURCE_DIR}/deps/sqlite3")

# define windows os target
add_library(os_windows SHARED ${SRC})
target_link_libraries(os_windows Medusa)

if(WIN32)
  install(TARGETS os_windows RUNTIME DESTINATION .)
else()
  install(TARGETS os_windows LIBRARY DESTINATION .)
endif()

install(FILES windows.db DESTINATION .)
