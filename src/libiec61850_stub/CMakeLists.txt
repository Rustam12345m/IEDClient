set(SOURCES
	ied_model_api_impl.hpp
    ied_model_api_impl.cpp

    ied_state_api_impl.hpp
    ied_state_api_impl.cpp

    ied_control_api_impl.hpp
    ied_control_api_impl.cpp

    ied_fs_api_impl.hpp
    ied_fs_api_impl.cpp

	libiec61850_adapter.hpp
	libiec61850_adapter.cpp

	libiec61850_tools.hpp
	libiec61850_tools.cpp
)

qt_add_library(lib61850_adapter STATIC ${SOURCES})

target_link_libraries(lib61850_adapter PRIVATE Qt6::Core)

target_include_directories(lib61850_adapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})