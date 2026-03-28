find_package(CLI11 CONFIG REQUIRED)

set(SRCROOT ${CMAKE_SOURCE_DIR}/src/tools/text)

add_executable(medusa_text ${SRCROOT}/main.cpp)
target_link_libraries(medusa_text medusa CLI11::CLI11)
set_target_properties(medusa_text PROPERTIES FOLDER "Tools")

install(TARGETS medusa_text RUNTIME DESTINATION ${MEDUSA_EXECUTABLE_PATH})
