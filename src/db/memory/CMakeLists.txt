set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/memory)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/memory)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/memory_db.hpp
  ${SRCROOT}/memory_db.cpp
  )

add_library(db_memory SHARED ${SRC})

# define db memory target
target_link_libraries(db_memory
  PRIVATE
  medusa
)
set_target_properties(db_memory PROPERTIES
  FOLDER "Databases"
  PREFIX ""
  DEBUG_POSTFIX ""
)

install(TARGETS db_memory
  RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
  LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
)
