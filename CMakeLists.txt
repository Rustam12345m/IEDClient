cmake_minimum_required(VERSION 3.14)

project(IEDClient VERSION "0.0.1" LANGUAGES C CXX)

set(PROJECT                     "IEDClient")
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR   ON)
set(BUILD_TESTS                 ON)

# Software version and other stuff
configure_file(builder_information.hpp.in ${CMAKE_BINARY_DIR}/include/builder_information.hpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_BINARY_DIR}/include/
)

# Qt 6 specific stuff
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 QuickTemplates2)
qt_standard_project_setup()

if (BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

add_subdirectory(3rdparty/)
add_subdirectory(src/)

# Install the executable into "${CMAKE_INSTALL_PREFIX}/bin".
install(TARGETS ${PROJECT}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# Generate the deployment script for the target 
# qt_generate_deploy_app_script(
qt_generate_deploy_qml_app_script(
    TARGET ${PROJECT}
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
# Call the deployment script on "cmake --install".
install(SCRIPT ${deploy_script})