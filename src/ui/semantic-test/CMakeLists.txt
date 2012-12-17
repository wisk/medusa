include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/semantic-test)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/semantic-test)

# dummy ui source files
set(SRC
  ${SRCROOT}/main.cpp
)

add_executable(semantic
  ${SRC}
)

find_package(Threads REQUIRED)
target_link_libraries(semantic Medusa ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS semantic RUNTIME DESTINATION .)
