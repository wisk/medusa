set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/avr8)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/avr8)

# all source files
set(SRC
  ${INCROOT}/avr8_loader.hpp

  ${SRCROOT}/main.cpp
  ${SRCROOT}/avr8_loader.cpp
  )

# define pe loader target
add_library(ldr_avr8 SHARED ${SRC})
target_link_libraries(ldr_avr8 Medusa)

if(WIN32)
  install(TARGETS ldr_avr8 RUNTIME DESTINATION .)
else()
  install(TARGETS ldr_avr8 LIBRARY DESTINATION .)
endif()


 
