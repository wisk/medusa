include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/tools/text)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/tools/text)

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

if (MEDUSA_CMAKE_USE_COTIRE)
  cotire(medusa_text)
endif()

install(TARGETS medusa_text RUNTIME DESTINATION .)
