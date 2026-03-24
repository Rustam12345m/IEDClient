# Building core's classes to a static library

set(CORE_SOURCES
    model_item.hpp
    model_item.cpp

    model_item_value.cpp
    model_item_value.hpp
    model_values_updater.hpp

    item_factory.hpp
    item_factory.cpp

    sub_attribute.hpp

    data_attribute.cpp
    data_attribute.hpp
    data_object.hpp

    logical_node.hpp
    logical_node.cpp
    logical_device.hpp

    dataset.hpp
    report_control_block.hpp
    goose_control_block.hpp
    sv_control_block.hpp
    sgcb.hpp

    fs_model.hpp

    received_report.hpp
    report_storage.hpp
    report_storage.cpp

    data_model.hpp
    data_model.cpp
    data_model_builder.hpp
    data_model_builder.cpp

    ln_signal_matrix.hpp
    ln_signal_matrix.cpp

    ied.hpp
    server_identify.hpp
)

qt_add_library(core_lib STATIC ${CORE_SOURCES})

target_link_libraries(core_lib PRIVATE Qt6::Core)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

