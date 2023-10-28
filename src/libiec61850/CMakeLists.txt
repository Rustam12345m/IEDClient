set(SOURCES
	lib61850.hpp
	lib61850.cpp
)

qt_add_library(lib61850_adapter STATIC ${SOURCES})

target_link_libraries(lib61850_adapter PRIVATE Qt6::Core)

target_include_directories(lib61850_adapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})