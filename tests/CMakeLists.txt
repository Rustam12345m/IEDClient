set(TEST_TARGET_NAME unit_tests)

# Unit tests
add_executable(${TEST_TARGET_NAME} test.cpp)
target_link_libraries(${TEST_TARGET_NAME} gtest gtest_main)

# CTest
add_test(NAME UnitTests COMMAND ${TEST_TARGET_NAME})