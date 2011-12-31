set(INCROOT ${CMAKE_SOURCE_DIR}/src/srlz/dummy)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/srlz/dummy)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/dummy_serialize.hpp
  ${SRCROOT}/dummy_serialize.cpp
  )

# define dummy serialize target
add_library(srlz_dummy SHARED ${SRC})
target_link_libraries(srlz_dummy Medusa)

if(WIN32)
  install(TARGETS srlz_dummy RUNTIME DESTINATION .)
else()
  install(TARGETS srlz_dummy LIBRARY DESTINATION .)
endif()
