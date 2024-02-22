# Building app's classes to a static library
set(APP_SOURCES
	config/config_adapter.hpp
	config/config_adapter.cpp
	config/conf_connection_info.hpp

	app_helpers.hpp
	app_helpers.cpp

	app_status.hpp
	app_status.cpp

	app_settings.hpp
	app_settings.cpp

	iedcon_container.hpp

	backend_base.hpp
	backend_base.cpp

	app_presenter.hpp
	app_presenter.cpp

	fs_presenter.hpp
	fs_presenter.cpp

	ied_presenter.hpp
	ied_presenter.cpp

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
target_link_libraries(app_lib PUBLIC Qt::Qml)

target_include_directories(app_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})