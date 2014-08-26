add_executable(sample_tests TestOfTest.cc)
target_link_libraries(sample_tests gtest gtest_main)
add_test(sample_tests sample_tests)


