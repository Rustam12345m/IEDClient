# Building models's classes to a static library

set(SOURCES
	models_stub.hpp
	
	do_tablemodel.hpp
	do_tablemodel.cpp
	
	app_events_model.hpp
	app_events_model.cpp

	files_model.hpp
	files_model.cpp

	ld_grid_model.hpp
	ld_grid_model.cpp
	ld_property_model.hpp
	ld_property_model.cpp

	ln_tablemodel.hpp
	ln_tablemodel.cpp

	sort_proxy_model.hpp
	sort_proxy_model.cpp

	last_conn_list.hpp
	last_conn_list.cpp
)

qt_add_library(models_lib STATIC ${SOURCES})

target_link_libraries(models_lib PUBLIC app_lib core_lib cmd_lib)
target_link_libraries(models_lib PRIVATE Qt6::Core)

target_include_directories(models_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})