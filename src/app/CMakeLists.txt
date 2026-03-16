# Building app's classes to a static library
set(APP_SOURCES
    config/conf_connection_info.hpp

    app_utils.hpp
    app_utils.cpp

    app_settings.hpp
    app_settings.cpp

    iedcon_container.hpp

    event_storage.hpp
    event_storage.cpp

    backend_interface.hpp
    backend_interface.cpp

    app_backend.hpp
    app_backend.cpp

    ied_fs_backend.hpp
    ied_fs_backend.cpp

    ied_backend.hpp
    ied_backend.cpp

    main_presenter.hpp
    main_presenter.cpp
)

qt_add_library(app_lib STATIC ${APP_SOURCES})

target_link_libraries(app_lib PUBLIC
    lib61850_adapter
    core_lib
    cmd_lib
    models_lib
    tools_lib
)
target_link_libraries(app_lib PUBLIC Qt::Qml Qt::Gui)

target_include_directories(app_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})