# C++ source code
set(PROJECT_SOURCES
	main.cpp

	${CMAKE_SOURCE_DIR}/ui/resources.qrc
)

add_subdirectory(core/)
add_subdirectory(lib61850_adapter/)
add_subdirectory(cmd/)
add_subdirectory(models/)
add_subdirectory(tools/)
add_subdirectory(app/)

qt6_add_resources(QRC_SOURCE ${CMAKE_SOURCE_DIR}/ui/resources.qrc)
qt_add_executable(${PROJECT} ${PROJECT_SOURCES} ${QRC_SOURCE})

target_link_libraries(${PROJECT} PRIVATE app_lib core_lib lib61850_adapter cmd_lib models_lib tools_lib)
target_link_libraries(${PROJECT} PRIVATE Qt::Core Qt::Qml Qt::Quick)