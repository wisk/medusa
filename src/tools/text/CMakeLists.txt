include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/text)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/text)

# dummy ui source files
set(SRC
  ${SRCROOT}/main.cpp
)

set(INC
)

add_executable(medusa_text
  ${INC}
  ${SRC}
)

find_package(Threads REQUIRED)
target_link_libraries(medusa_text Medusa ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(medusa_text PROPERTIES FOLDER "Tools")

install(TARGETS medusa_text RUNTIME DESTINATION .)
