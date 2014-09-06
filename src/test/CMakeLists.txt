set(TEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# LATER: Could we generate these lines using file(glob...)+foreach?

## Architecture
add_executable(test_arch ${TEST_ROOT}/test_arch.cpp)
target_link_libraries(test_arch Medusa)
add_test("testing_architecture" test_arch)

## Database
add_executable(test_db ${TEST_ROOT}/test_db.cpp)
target_link_libraries(test_db Medusa)
add_test("testing_database" test_db)

## Emulation
add_executable(test_emul ${TEST_ROOT}/test_emul.cpp)
target_link_libraries(test_emul Medusa)
add_test("testing_emulation" test_emul)

## Expression
add_executable(test_expr ${TEST_ROOT}/test_expr.cpp)
target_link_libraries(test_expr Medusa)
add_test("testing_expression" test_expr)

## Loader
add_executable(test_ldr ${TEST_ROOT}/test_ldr.cpp)
target_link_libraries(test_ldr Medusa)
add_test("testing_loader" test_ldr)

## Operating System
add_executable(test_os ${TEST_ROOT}/test_os.cpp)
target_link_libraries(test_os Medusa)
add_test("testing_operating system" test_os)