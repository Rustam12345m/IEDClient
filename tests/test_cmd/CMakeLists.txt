set(TEST_TARGET_NAME test_cmd)

set(TEST_SOURCES
	main.cpp
	test_cmd.hpp
)

# Unit tests
qt_add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE gtest gtest_main)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE core_lib cmd_lib)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE Qt::Core)

# CTest
add_test(NAME TestCMD COMMAND ${TEST_TARGET_NAME})