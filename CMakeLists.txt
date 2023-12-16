cmake_minimum_required(VERSION 3.14)

project(IEDClient VERSION "1.0.0" LANGUAGES C CXX)

set(PROJECT 					"IEDClient")
set(CMAKE_CXX_STANDARD 			17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR 	ON)

# Software version and other stuff
configure_file(builder_information.hpp.in ${CMAKE_BINARY_DIR}/include/builder_information.hpp)

include_directories(
	${CMAKE_SOURCE_DIR}/src/
	${CMAKE_BINARY_DIR}/include/
)

# Qt 6 specific stuff
set(Qt6_DIR /opt/Qt/6.5.2/gcc_64/lib/cmake/Qt6/)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Core Qml Quick QuickControls2 QuickTemplates2 REQUIRED)
qt_standard_project_setup()

# Main source code if IEDClient
add_subdirectory(src/)

# libiec61850 and other libraries
add_subdirectory(3rdparty/)

# Unit-tests
enable_testing()
add_subdirectory(tests/)

# CPack
#install(TARGETS IEDClient RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)