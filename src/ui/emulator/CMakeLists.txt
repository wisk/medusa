include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/emulator)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/emulator)

# dummy ui source files
set(SRC
  ${SRCROOT}/main.cpp
)

add_executable(emulator
  ${SRC}
)

find_package(Threads REQUIRED)
target_link_libraries(emulator Medusa ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS emulator RUNTIME DESTINATION .)
