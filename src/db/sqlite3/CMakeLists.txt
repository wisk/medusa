set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/sqlite3)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/sqlite3)

find_package(sqlite3 CONFIG REQUIRED)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/sqlite3_db.hpp
  ${SRCROOT}/sqlite3_db.cpp
  )

add_library(db_sqlite3 SHARED ${SRC})

# define db sqlite3 target
target_link_libraries(db_sqlite3
  PRIVATE 
  medusa
  sqlite3
)
set_target_properties(db_sqlite3 PROPERTIES
  FOLDER "Databases"
  PREFIX ""
  DEBUG_POSTFIX ""
)

install(TARGETS db_sqlite3
  RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
  LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
)
