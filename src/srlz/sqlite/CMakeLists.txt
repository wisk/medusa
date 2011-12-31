set(INCROOT ${CMAKE_SOURCE_DIR}/src/srlz/sqlite)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/srlz/sqlite)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/sqlite_serialize.hpp
  ${SRCROOT}/sqlite_serialize.cpp
  ${SRCROOT}/sqlite3.c
  ${INCROOT}/sqlite3.h
  ${INCROOT}/sqlite3ext.h
  )

# define sqlite serialize target
add_library(srlz_sqlite SHARED ${SRC})
target_link_libraries(srlz_sqlite Medusa)

if(WIN32)
  install(TARGETS srlz_sqlite RUNTIME DESTINATION .)
else()
  install(TARGETS srlz_sqlite LIBRARY DESTINATION .)
endif()
