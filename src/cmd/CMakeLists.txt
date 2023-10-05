# Building cmd's classes to a static library

set(SOURCES
	base_command.hpp
	command_queue.hpp
	cmd_thread.hpp
	cmd_thread.cpp
	lib_interface.hpp
	lib61850_adapter.hpp
	lib61850_adapter.cpp
	update_lnode_cmd.hpp
	update_lnode_cmd.cpp
	get_filelist_cmd.hpp
	get_filelist_cmd.cpp
	connect_cmd.hpp
	connect_cmd.cpp
	get_ldlist_cmd.hpp
	get_ldlist_cmd.cpp
	download_file_cmd.hpp
	download_file_cmd.cpp
	remove_file_cmd.hpp
	remove_file_cmd.cpp
)

qt_add_library(cmd_lib STATIC ${SOURCES})

target_link_libraries(cmd_lib PRIVATE Qt6::Core)

target_include_directories(cmd_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})