# Building cmd's classes to a static library

set(SOURCES
	base_command.hpp
	command_queue.hpp
	lib_interface.hpp

	cmd_thread.hpp
	cmd_thread.cpp
	
	connect_cmd.hpp
	connect_cmd.cpp

	update_ld_status_cmd.hpp
	update_ld_status_cmd.cpp

	update_ln_cmd.hpp
	update_ln_cmd.cpp

	get_filelist_cmd.hpp
	get_filelist_cmd.cpp

	download_file_cmd.hpp
	download_file_cmd.cpp

	remove_file_cmd.hpp
	remove_file_cmd.cpp
)

qt_add_library(cmd_lib STATIC ${SOURCES})

target_link_libraries(cmd_lib PRIVATE Qt6::Core)

target_include_directories(cmd_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})