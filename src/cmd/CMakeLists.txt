# Building cmd's classes to a static library

set(SOURCES
    interface/ied_model_api.hpp
    interface/ied_state_api.hpp
    interface/ied_fs_api.hpp
    interface/ied_control_api.hpp
    interface/iec61850_api.hpp

	cmd_interface.hpp
    cmd_event.hpp
	command_queue.hpp

	cmd_thread.hpp
	cmd_thread.cpp

	ied_credentials.hpp
	connect_cmd.hpp
	connect_cmd.cpp
	disconnect_cmd.hpp
	disconnect_cmd.cpp

	update_lds_status_cmd.hpp
	update_lds_status_cmd.cpp

	update_lns_status_cmd.cpp
	update_lns_status_cmd.cpp

	update_dataset_cmd.hpp
	update_dataset_cmd.cpp

	update_ln_cmd.hpp
	update_ln_cmd.cpp

	update_rcbs_cmd.hpp
	update_rcbs_cmd.cpp

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