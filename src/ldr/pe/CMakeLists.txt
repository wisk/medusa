set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/pe)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/pe)

# all source files
set(SRC
  ${INCROOT}/pe.hpp
  ${INCROOT}/pe_loader.hpp

  ${SRCROOT}/main.cpp
  ${SRCROOT}/pe.cpp
  ${SRCROOT}/pe_loader.cpp
  )

# define pe loader target
add_library(ldr_pe SHARED ${SRC})
target_link_libraries(ldr_pe medusa)
set_target_properties(ldr_pe PROPERTIES
  FOLDER "Loaders"
  PREFIX "")

if(WIN32)
  install(TARGETS ldr_pe RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS ldr_pe LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
