set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/soci)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/soci)


# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/soci_db.hpp
  ${SRCROOT}/soci_db.cpp
  )

add_library(db_soci SHARED ${SRC})
add_definitions(db_soci SQLite3)

find_package(SQLite3 CONFIG)
# include("${SOCI_ROOT}/cmake/SOCI.cmake")

include_directories("${SOCI_ROOT}/include") # HACK(wisk): INTERFACE_INCLUDE_DIRECTORIES shoud be used instead

# define db soci target
target_link_libraries(db_soci medusa sqlite3 SOCI::soci_core_static SOCI::soci_sqlite3_static)
set_target_properties(db_soci PROPERTIES
  FOLDER "Databases"
  PREFIX ""
  DEBUG_POSTFIX ""
)

install(TARGETS db_soci
  RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
  LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
)
