set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/dos)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/dos)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/dos_loader.cpp
  ${SRCROOT}/dos_loader.hpp
  ${SRCROOT}/dos.hpp
  ${SRCROOT}/dos.cpp
  
  )

# define boot sector loader target
add_library(ldr_dos SHARED ${SRC})
target_link_libraries(ldr_dos Medusa)

if(WIN32)
  install(TARGETS ldr_dos RUNTIME DESTINATION .)
else()
  install(TARGETS ldr_dos LIBRARY DESTINATION .)
endif()
