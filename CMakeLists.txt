cmake_minimum_required(VERSION 3.14)

project(IEDClient VERSION "1.0.0" LANGUAGES C CXX)

set(PROJECT 					"IEDClient")
set(CMAKE_CXX_STANDARD 			17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR 	ON)

# Software version and other stuff
configure_file(builder_information.hpp.in ${CMAKE_BINARY_DIR}/include/builder_information.hpp)

# Main source code if IEDClient
add_subdirectory(src/)

# libiec61850 and other libraries
add_subdirectory(3rdparty/)

# Unit-tests
enable_testing()
add_subdirectory(tests/)

# CPack
#install(TARGETS IEDClient RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
