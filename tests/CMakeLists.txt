set(TEST_TARGET_NAME tests)

set(TEST_SOURCES
	main.cpp

	Cmd_Tests.hpp
	DataModel_Tests.hpp
	DumpModel_Tests.hpp
	Items_Tests.hpp
	AppConfig_Tests.hpp
)

# Unit tests
qt_add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE gtest gmock gtest_main)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE app_lib)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE aggregated_qt_lib)

# CTest
add_test(NAME Tests COMMAND ${TEST_TARGET_NAME})