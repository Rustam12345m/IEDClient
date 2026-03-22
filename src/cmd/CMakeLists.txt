# Building cmd's classes to a static library

set(SOURCES
    interface/ied_model_api.hpp
    interface/ied_state_api.hpp
    interface/ied_fs_api.hpp
    interface/ied_control_api.hpp
    interface/control_types.hpp
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

    set_rcb_values_cmd.hpp
    set_rcb_values_cmd.cpp

    update_goose_cbs_cmd.hpp
    update_goose_cbs_cmd.cpp

    update_sv_cbs_cmd.hpp
    update_sv_cbs_cmd.cpp

    set_goose_enable_cmd.hpp
    set_goose_enable_cmd.cpp

    set_sv_enable_cmd.hpp
    set_sv_enable_cmd.cpp

    control_cmd.hpp
    control_cmd.cpp

    get_filelist_cmd.hpp
    get_filelist_cmd.cpp

    download_file_cmd.hpp
    download_file_cmd.cpp

    remove_file_cmd.hpp
    remove_file_cmd.cpp

    mms_dump_cmd.hpp
    mms_dump_cmd.cpp

    update_watchlist_cmd.hpp
    update_watchlist_cmd.cpp
)

qt_add_library(cmd_lib STATIC ${SOURCES})

target_link_libraries(cmd_lib PRIVATE Qt6::Core)
target_link_libraries(cmd_lib PRIVATE iec61850)

target_include_directories(cmd_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(cmd_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/config/
    ${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/hal/inc/
    ${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/iec61850/inc/
    ${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/common/inc/
    ${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/mms/inc/
)