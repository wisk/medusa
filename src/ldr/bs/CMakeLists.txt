set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/bs)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/bs)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/bootsector_loader.cpp
  ${SRCROOT}/bootsector_loader.hpp
  )

# define boot sector loader target
add_library(ldr_bs SHARED ${SRC})
target_link_libraries(ldr_bs medusa)
set_target_properties(ldr_bs PROPERTIES
  FOLDER "Loaders"
  PREFIX "")

if(WIN32)
  install(TARGETS ldr_bs RUNTIME DESTINATION lib)
else()
  install(TARGETS ldr_bs LIBRARY DESTINATION lib)
endif()
