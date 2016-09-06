set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/soci)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/soci)


# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/soci_db.hpp
  ${SRCROOT}/soci_db.cpp
  )

find_package(SQLite3 REQUIRED)
include("${SOCI_ROOT}/cmake/SOCI.cmake")

# define db soci target
include_directories(${SQLITE3_INCLUDE_DIRS})
include_directories(${SOCI_INCLUDE_DIRS})
add_library(db_soci SHARED ${SRC})
target_link_libraries(db_soci medusa ${SQLITE3_LIBRARY} SOCI::soci_core_static SOCI::soci_sqlite3_static)
set_target_properties(db_soci PROPERTIES
  FOLDER "Databases"
  PREFIX "")

install(TARGETS db_soci
  RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
  LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
)
