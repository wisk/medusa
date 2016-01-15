include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/tools/emulator)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/tools/emulator)

set(SRC
  ${SRCROOT}/main.cpp
)

add_executable(emulator
  ${SRC}
)

find_package(Threads REQUIRED)
target_link_libraries(emulator Medusa ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(emulator PROPERTIES FOLDER "Tools")

if (MEDUSA_CMAKE_USE_COTIRE)
  cotire(emulator)
endif()

install(TARGETS emulator RUNTIME DESTINATION .)
