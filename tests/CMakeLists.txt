set(TEST_TARGET_NAME tests)

set(TEST_SOURCES
	main.cpp

    model_item_test.hpp
    model_item_test.cpp

    data_model_test.hpp
    data_model_test.cpp

	app_settings_test.hpp
	app_settings_test.cpp

	cmd_test.hpp
	cmd_test.cpp
)

# Unit tests
qt_add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE gtest gmock gtest_main)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE app_lib)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE aggregated_qt_lib)

# CTest
add_test(NAME Tests COMMAND ${TEST_TARGET_NAME})