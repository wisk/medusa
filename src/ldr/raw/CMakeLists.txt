set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/raw)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/raw)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/raw_loader.cpp
  ${INCROOT}/raw_loader.hpp
  )

# define raw architecture target
add_library(ldr_raw SHARED ${SRC})
target_link_libraries(ldr_raw Medusa)

if(WIN32)
  install(TARGETS ldr_raw RUNTIME DESTINATION .)
else()
  install(TARGETS ldr_raw LIBRARY DESTINATION .)
endif()
