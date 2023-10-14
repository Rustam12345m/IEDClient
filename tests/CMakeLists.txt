set(TEST_TARGET_NAME utests)

set(TEST_SOURCES
	main.cpp
	core_tests.hpp
)

# Unit tests
add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_TARGET_NAME} gtest gtest_main)

# CTest
add_test(NAME UnitTests COMMAND ${TEST_TARGET_NAME})