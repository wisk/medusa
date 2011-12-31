set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/arm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/arm)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  )

# define arm architecture target
add_library(arch_arm SHARED ${SRC})
target_link_libraries(arch_arm Medusa)

if(WIN32)
  install(TARGETS arch_arm RUNTIME DESTINATION .)
else()
  install(TARGETS arch_arm LIBRARY DESTINATION .)
endif()
