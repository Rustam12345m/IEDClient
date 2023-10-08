# C++ source code
set(PROJECT_SOURCES
	main.cpp

	${CMAKE_SOURCE_DIR}/ui/resources.qrc
)

include_directories(
	${CMAKE_SOURCE_DIR}/src/
	${CMAKE_BINARY_DIR}/include/
)

# Qt 6 specific stuff
set(Qt6_DIR /opt/Qt/6.5.2/gcc_64/lib/cmake/Qt6/)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6 COMPONENTS Core Qml Quick QuickControls2 QuickTemplates2 REQUIRED)

add_subdirectory(app/)
add_subdirectory(core/)
add_subdirectory(cmd/)
add_subdirectory(models/)

qt6_add_resources(QRC_SOURCE ${CMAKE_SOURCE_DIR}/ui/resources.qrc)
qt_add_executable(${PROJECT} ${PROJECT_SOURCES} ${QRC_SOURCE})

target_link_libraries(${PROJECT} PRIVATE app_lib core_lib cmd_lib models_lib)
target_link_libraries(${PROJECT} PRIVATE Qt::Core Qt::Qml Qt::Quick)