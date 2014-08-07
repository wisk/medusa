#avr8
set(INCROOT ${CMAKE_SOURCE_DIR}/src/os/avr8)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/os/avr8)

 set(SRC
  ${SRCROOT}/main.cpp
  )

add_library(os_avr8 SHARED ${SRC})
target_link_libraries(os_avr8 Medusa)

if (WIN32)
  install(TARGETS os_avr8 RUNTIME DESTINATION .)
else()
  install(TARGETS os_avr8 LIBRARY DESTINATION .)
endif()
