set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/gb)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/gb)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/gameboy.h
  ${SRCROOT}/gameboy_loader.cpp
  ${INCROOT}/gameboy_loader.hpp
  )

# define pe architecture target
add_library(ldr_gb SHARED ${SRC})
target_link_libraries(ldr_gb medusa)
set_target_properties(ldr_gb PROPERTIES
  FOLDER "Loaders"
  PREFIX "")

if(WIN32)
  install(TARGETS ldr_gb RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS ldr_gb LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
