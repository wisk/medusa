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

target_link_libraries(medusa_text medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(medusa_text PROPERTIES FOLDER "Tools")

install(TARGETS medusa_text RUNTIME DESTINATION bin)
