# Building models's classes to a static library
set(SOURCES
    models_stub.hpp

    iec61850_enums.hpp
    iec61850_enums.cpp

    app_events_table.hpp
    app_events_table.cpp
    
    ied_file_table.hpp
    ied_file_table.cpp

    ln_state_table.cpp
    ln_state_table.hpp

    ld_overview_grid.hpp
    ld_overview_grid.cpp

    ld_properties_table.hpp
    ld_properties_table.cpp

    ln_overview_table.hpp
    ln_overview_table.cpp

    ln_common_tree.hpp
    ln_common_tree.cpp

    sort_proxy_model.hpp
    sort_proxy_model.cpp

    history_con_table.hpp
    history_con_table.cpp

    ds_overview_table.hpp
    ds_overview_table.cpp

    ds_signals_table.hpp
    ds_signals_table.cpp

    rcb_overview_table.hpp
    rcb_overview_table.cpp

    reports_table.hpp
    reports_table.cpp
)

qt_add_library(models_lib STATIC ${SOURCES})

target_link_libraries(models_lib PUBLIC core_lib cmd_lib)
target_link_libraries(models_lib PUBLIC Qt6::Core)

target_include_directories(models_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})