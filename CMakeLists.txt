cmake_minimum_required(VERSION 3.14)

project(IEDClient VERSION "1.0.0" LANGUAGES C CXX)

set(PROJECT                         "IEDClient")
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)

set(CMAKE_INCLUDE_CURRENT_DIR       ON)
set(BUILD_TESTS                     ON)

# Software version and other stuff
configure_file(builder_information.hpp.in ${CMAKE_BINARY_DIR}/include/builder_information.hpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_BINARY_DIR}/include/
)

# Hack
set(Qt6_DIR /opt/Qt6/6.7.0/gcc_64/lib/cmake/Qt6/)

# Qt 6 specific stuff
find_package(Qt6 COMPONENTS Core Qml Quick QuickControls2 QuickTemplates2 REQUIRED)
qt_standard_project_setup()

if (BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

add_subdirectory(3rdparty/)
add_subdirectory(src/)