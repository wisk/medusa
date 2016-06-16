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
target_link_libraries(os_windows medusa)
set_target_properties(os_windows PROPERTIES
  FOLDER "Operating Systems"
  PREFIX "")

if(WIN32)
  install(TARGETS os_windows RUNTIME DESTINATION lib)
else()
  install(TARGETS os_windows LIBRARY DESTINATION lib)
endif()

install(FILES windows.db DESTINATION share/medusa)
