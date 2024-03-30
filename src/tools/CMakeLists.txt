# Building tools to a static library
set(TOOL_SOURCES
    dump_model.hpp
    dump_model.cpp
    dump_writer.hpp
    dump_writer.cpp
)

qt_add_library(tools_lib STATIC ${TOOL_SOURCES})

target_link_libraries(tools_lib PRIVATE Qt6::Core)

target_include_directories(tools_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})