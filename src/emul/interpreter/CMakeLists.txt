set(INCROOT ${CMAKE_SOURCE_DIR}/src/emul/interpreter)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/emul/interpreter)

set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/interpreter_emulator.cpp
  ${INCROOT}/interpreter_emulator.hpp
  )

add_library(emul_interpreter SHARED ${SRC})
target_link_libraries(emul_interpreter medusa)
set_target_properties(emul_interpreter PROPERTIES
  FOLDER "Emulations"
  PREFIX "")

if(WIN32)
  install(TARGETS emul_interpreter RUNTIME DESTINATION lib)
else()
  install(TARGETS emul_interpreter LIBRARY DESTINATION lib)
endif()
