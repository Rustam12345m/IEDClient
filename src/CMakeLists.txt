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
	# Qt::Core
	# Qt6::Qml
	Qt6::Quick
	Qt6::QuickControls2
)

qt6_add_resources(QRC_SOURCE ${CMAKE_SOURCE_DIR}/ui/resources.qrc)
qt_add_executable(${PROJECT} main.cpp ${QRC_SOURCE})

target_link_libraries(${PROJECT} PRIVATE app_lib)
target_link_libraries(${PROJECT} PRIVATE aggregated_qt_lib)