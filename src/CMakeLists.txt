# Source code
add_subdirectory(libiec61850_stub/)
add_subdirectory(core/)
add_subdirectory(cmd/)
add_subdirectory(tools/)
add_subdirectory(models/)
add_subdirectory(app/)

# All used Qt's libraries
add_library(aggregated_qt_lib INTERFACE)
target_link_libraries(aggregated_qt_lib INTERFACE
    Qt6::Quick
    Qt6::QuickControls2
)

# Build the main binary
qt6_add_resources(QRC_SOURCE ${CMAKE_SOURCE_DIR}/ui/resources.qrc)
qt_add_executable(${PROJECT} main.cpp ${QRC_SOURCE})

# qt_add_qml_module(
#     ${PROJECT}
#     URI Application
#     VERSION 1.0
#     QML_FILES ../ui/main.qml
# )

target_link_libraries(${PROJECT} PRIVATE app_lib)
target_link_libraries(${PROJECT} PRIVATE aggregated_qt_lib)
target_link_libraries(${PROJECT} PRIVATE breakpad_client)
target_link_libraries(${PROJECT} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

# Tests
if(BUILD_TESTS)
    add_subdirectory(tests/)
endif()
