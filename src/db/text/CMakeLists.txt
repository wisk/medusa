set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/text)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/text)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/text_db.hpp
  ${SRCROOT}/text_db.cpp
  )

# define db text target
add_library(db_text SHARED ${SRC})
target_link_libraries(db_text Medusa)
set_target_properties(db_text PROPERTIES FOLDER "Databases")

if(WIN32)
  install(TARGETS db_text RUNTIME DESTINATION .)
else()
  install(TARGETS db_text LIBRARY DESTINATION .)
endif()
