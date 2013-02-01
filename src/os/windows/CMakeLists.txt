set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/windows)

# all source files
set(SRC
  ${INCROOT}/windows.hpp

  ${SRCROOT}/main.cpp
  ${SRCROOT}/windows.cpp
  )

# define windows os target
add_library(os_windows SHARED ${SRC})
target_link_libraries(os_windows Medusa)

if(WIN32)
  install(TARGETS os_windows RUNTIME DESTINATION .)
else()
  install(TARGETS os_windows LIBRARY DESTINATION .)
endif()
