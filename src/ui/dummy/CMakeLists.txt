include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/dummy)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/dummy)

# dummy ui source files
set(SRC
  ${SRCROOT}/main.cpp
)

# dummy ui header files
set(INC
  ${INCROOT}/dummy.hpp
)

add_executable(medusa_dummy
  ${INC}
  ${SRC}
)

find_package(Threads REQUIRED)
target_link_libraries(medusa_dummy Medusa ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS medusa_dummy RUNTIME DESTINATION .)
