set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/soci)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/soci)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/soci_db.hpp
  ${SRCROOT}/soci_db.cpp
  )

ExternalProject_add(soci
  PREFIX          "soci"

  GIT_REPOSITORY  "https://github.com/SOCI/soci"
  GIT_TAG         "master"

  SOURCE_DIR      "${CMAKE_SOURCE_DIR}/deps/soci"
  INSTALL_DIR     "${CMAKE_BINARY_DIR}"

  UPDATE_COMMAND  ""
  INSTALL_COMMAND ""
)

set(SQLITE3_INCLUDE_DIRS
  "${CMAKE_SOURCE_DIR}/deps/sqlite3"
)

set(SOCI_INCLUDE_DIRS
  "${CMAKE_SOURCE_DIR}/deps/soci/include"
  "${CMAKE_CURRENT_BINARY_DIR}/soci/src/soci-build/include"
)

set(SOCI_LIBRARIES)
  if(MSVC)
    set(SOCI_LIBRARIES
    debug     "${CMAKE_CURRENT_BINARY_DIR}/soci/src/soci-build/lib/Debug/soci_core_4_0.lib"
    optimized "${CMAKE_CURRENT_BINARY_DIR}/soci/src/soci-build/lib/Release/soci_core_4_0.lib"
  )
else()
  set(SOCI_LIBRARIES
    "${CMAKE_CURRENT_BINARY_DIR}/soci/src/soci-build/lib/libsoci_core.so.4.0"
  )
endif()

# define db soci target
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${SOCI_INCLUDE_DIRS})
add_library(db_soci SHARED ${SRC})
target_link_libraries(db_soci medusa ${SOCI_LIBRARIES})
set_target_properties(db_soci PROPERTIES
  FOLDER "Databases"
  PREFIX "")

add_dependencies(db_soci soci)

if(WIN32)
  install(TARGETS db_soci RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS db_soci LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
