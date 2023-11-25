# Building app's classes to a static library

set(APP_SOURCES
	app_settings.hpp
	app_settings.cpp
	
	ied_connection.hpp

	backend_base.hpp
	backend_base.cpp
	
	app_backend.hpp
	app_backend.cpp
	
	fs_backend.hpp
	fs_backend.cpp
	
	dev_backend.hpp
	dev_backend.cpp
	
	presenter.hpp
	presenter.cpp
)

qt_add_library(app_lib STATIC ${APP_SOURCES})

target_link_libraries(app_lib PRIVATE Qt6::Core Qt::Qml)

target_include_directories(app_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})