set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/mach-o)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/mach-o)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/mach-o_loader.cpp
  ${INCROOT}/mach-o_loader.hpp
  ${INCROOT}/mach-o_traits.hpp
  ${INCROOT}/mach-o.h
  )

# define mach-o loader target
add_library(ldr_mach-o SHARED ${SRC})
target_link_libraries(ldr_mach-o medusa)
set_target_properties(ldr_mach-o PROPERTIES
  FOLDER "Loaders"
  PREFIX "")

if(WIN32)
  install(TARGETS ldr_mach-o RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS ldr_mach-o LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
