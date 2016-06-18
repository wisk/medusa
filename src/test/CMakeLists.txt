set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
  set(WORKING_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>)
else()
  set(WORKING_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
set(SAMPLES_DIR ${CMAKE_SOURCE_DIR}/samples)
add_definitions(-DSAMPLES_DIR="${SAMPLES_DIR}")

# LATER: Could we generate these lines using file(glob...)+foreach?

## Core (medusa ${MEDUSA_NEEDED_LIBRARIES})
add_executable(test_core ${TEST_ROOT}/test_core.cpp)
target_link_libraries(test_core medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_core PROPERTIES FOLDER "Tests")
add_test(NAME "testing_core"
  COMMAND $<TARGET_FILE:test_core>
  WORKING_DIRECTORY ${WORKING_DIR})

## Architecture
add_executable(test_arch ${TEST_ROOT}/test_arch.cpp)
target_link_libraries(test_arch medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_arch PROPERTIES FOLDER "Tests")
add_test(NAME "testing_architecture"
  COMMAND $<TARGET_FILE:test_arch>
  WORKING_DIRECTORY ${WORKING_DIR})

## Binding
add_executable(test_bind ${TEST_ROOT}/test_bind.cpp)
target_link_libraries(test_bind medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_bind PROPERTIES FOLDER "Tests")
add_test(NAME "testing_binding"
  COMMAND $<TARGET_FILE:test_bind>
  WORKING_DIRECTORY ${WORKING_DIR})

## Database
add_executable(test_db ${TEST_ROOT}/test_db.cpp)
target_link_libraries(test_db medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_db PROPERTIES FOLDER "Tests")
add_test(NAME "testing_database"
  COMMAND $<TARGET_FILE:test_db>
  WORKING_DIRECTORY ${WORKING_DIR})

## Emulation
add_executable(test_emul ${TEST_ROOT}/test_emul.cpp)
target_link_libraries(test_emul medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_emul PROPERTIES FOLDER "Tests")
add_test(NAME "testing_emulation"
  COMMAND $<TARGET_FILE:test_emul>
  WORKING_DIRECTORY ${WORKING_DIR})

## Expression
add_executable(test_expr ${TEST_ROOT}/test_expr.cpp)
target_link_libraries(test_expr medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_expr PROPERTIES FOLDER "Tests")
add_test(NAME "testing_expression"
  COMMAND $<TARGET_FILE:test_expr>
  WORKING_DIRECTORY ${WORKING_DIR})

## Loader
add_executable(test_ldr ${TEST_ROOT}/test_ldr.cpp)
target_link_libraries(test_ldr medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_ldr PROPERTIES FOLDER "Tests")
add_test(NAME "testing_loader"
  COMMAND $<TARGET_FILE:test_ldr>
  WORKING_DIRECTORY ${WORKING_DIR})

## Operating System
add_executable(test_os ${TEST_ROOT}/test_os.cpp)
target_link_libraries(test_os medusa ${MEDUSA_NEEDED_LIBRARIES})
set_target_properties(test_os PROPERTIES FOLDER "Tests")
add_test(NAME "testing_operating_system"
  COMMAND $<TARGET_FILE:test_os>
  WORKING_DIRECTORY ${WORKING_DIR})
