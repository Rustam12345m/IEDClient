# Building cmd's classes to a static library

set(SOURCES
	do_tablemodel.hpp
	do_tablemodel.cpp
	events_tablemodel.hpp
	events_tablemodel.cpp
	fs_tablemodel.hpp
	fs_tablemodel.cpp
	ld_listmodel.hpp
	ld_listmodel.cpp
	ln_tablemodel.hpp
	ln_tablemodel.cpp
	sort_proxy_model.hpp
	sort_proxy_model.cpp
	last_conn_list.hpp
	last_conn_list.cpp
)

qt_add_library(models_lib STATIC ${SOURCES})

target_link_libraries(models_lib PUBLIC core_lib cmd_lib)
target_link_libraries(models_lib PRIVATE Qt6::Core)

target_include_directories(models_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})