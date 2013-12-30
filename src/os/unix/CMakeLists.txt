set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/unix)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/unix)

set(SRC
  ${INCROOT}/unix.hpp

  ${SRCROOT}/unix.cpp
  ${SRCROOT}/main.cpp
  )

add_library(os_unix SHARED ${SRC})
target_link_libraries(os_unix Medusa)

if (WIN32)
  install(TARGETS os_unix RUNTIME DESTINATION .)
else()
  install(TARGETS os_unix LIBRARY DESTINATION .)
endif()
