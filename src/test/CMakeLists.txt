set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
  set(WORKING_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>)
else()
  set(WORKING_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
set(SAMPLES_DIR ${CMAKE_SOURCE_DIR}/samples)
add_definitions(-DSAMPLES_DIR="${SAMPLES_DIR}")

find_package(Catch2 CONFIG REQUIRED)

## Core
add_executable(test_core ${TEST_ROOT}/test_core.cpp)
target_link_libraries(test_core PRIVATE medusa Catch2::Catch)
set_target_properties(test_core PROPERTIES FOLDER "Tests")
add_test(NAME "testing_core"
  COMMAND $<TARGET_FILE:test_core>
  WORKING_DIRECTORY ${WORKING_DIR})

## Architecture
add_executable(test_arch ${TEST_ROOT}/test_arch.cpp)
target_link_libraries(test_arch PRIVATE medusa Catch2::Catch)
set_target_properties(test_arch PROPERTIES FOLDER "Tests")
add_test(NAME "testing_architecture"
  COMMAND $<TARGET_FILE:test_arch>
  WORKING_DIRECTORY ${WORKING_DIR})

## Binding
add_executable(test_bind ${TEST_ROOT}/test_bind.cpp)
target_link_libraries(test_bind PRIVATE medusa Catch2::Catch)
set_target_properties(test_bind PROPERTIES FOLDER "Tests")
add_test(NAME "testing_binding"
  COMMAND $<TARGET_FILE:test_bind>
  WORKING_DIRECTORY ${WORKING_DIR})

## Compilation
add_executable(test_compil ${TEST_ROOT}/test_compil.cpp)
target_link_libraries(test_compil PRIVATE medusa Catch2::Catch)
set_target_properties(test_compil PROPERTIES FOLDER "Tests")
add_test(NAME "testing_compilation"
  COMMAND $<TARGET_FILE:test_compil>
  WORKING_DIRECTORY ${WORKING_DIR})

## Database
add_executable(test_db ${TEST_ROOT}/test_db.cpp)
target_link_libraries(test_db PRIVATE medusa Catch2::Catch)
set_target_properties(test_db PROPERTIES FOLDER "Tests")
add_test(NAME "testing_database"
  COMMAND $<TARGET_FILE:test_db>
  WORKING_DIRECTORY ${WORKING_DIR})

## Emulation
add_executable(test_emul ${TEST_ROOT}/test_emul.cpp)
target_link_libraries(test_emul PRIVATE medusa Catch2::Catch)
set_target_properties(test_emul PROPERTIES FOLDER "Tests")
add_test(NAME "testing_emulation"
  COMMAND $<TARGET_FILE:test_emul>
  WORKING_DIRECTORY ${WORKING_DIR})

## Expression
add_executable(test_expr ${TEST_ROOT}/test_expr.cpp)
target_link_libraries(test_expr PRIVATE medusa Catch2::Catch)
set_target_properties(test_expr PROPERTIES FOLDER "Tests")
add_test(NAME "testing_expression"
  COMMAND $<TARGET_FILE:test_expr>
  WORKING_DIRECTORY ${WORKING_DIR})

## Loader
add_executable(test_ldr ${TEST_ROOT}/test_ldr.cpp)
target_link_libraries(test_ldr PRIVATE medusa Catch2::Catch)
set_target_properties(test_ldr PROPERTIES FOLDER "Tests")
add_test(NAME "testing_loader"
  COMMAND $<TARGET_FILE:test_ldr>
  WORKING_DIRECTORY ${WORKING_DIR})

## Operating System
add_executable(test_os ${TEST_ROOT}/test_os.cpp)
target_link_libraries(test_os PRIVATE medusa Catch2::Catch)
set_target_properties(test_os PROPERTIES FOLDER "Tests")
add_test(NAME "testing_operating_system"
  COMMAND $<TARGET_FILE:test_os>
  WORKING_DIRECTORY ${WORKING_DIR})