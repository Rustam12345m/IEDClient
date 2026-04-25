set(TEST_TARGET_NAME unit_tests)

set(TEST_SOURCES
    main.cpp

    mock_api.hpp

    app_settings_test.cpp
    data_model_test.cpp
    model_item_test.cpp
    cmd_test.cpp
    control_blocks_test.cpp
    command_test.cpp
    event_storage_test.cpp
)

# Unit tests
qt_add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})

target_link_libraries(${TEST_TARGET_NAME} PRIVATE gtest gmock gtest_main)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE app_lib)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE aggregated_qt_lib)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE Qt6::Test)

# CTest
add_test(NAME ${TEST_TARGET_NAME} COMMAND ${TEST_TARGET_NAME})

add_subdirectory(qml)