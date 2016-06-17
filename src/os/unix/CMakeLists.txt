set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/unix)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/unix)

set(SRC
  ${INCROOT}/unix.hpp

  ${SRCROOT}/unix.cpp
  ${SRCROOT}/main.cpp
  )

add_library(os_unix SHARED ${SRC})
target_link_libraries(os_unix medusa)
set_target_properties(os_unix PROPERTIES
  FOLDER "Operating Systems"
  PREFIX "")

if (WIN32)
  install(TARGETS os_unix RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS os_unix LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
