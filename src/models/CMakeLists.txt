# Building models's classes to a static library

set(SOURCES
	models_stub.hpp

	app_events_table.hpp
	app_events_table.cpp
	
	dev_fs_table.hpp
	dev_fs_table.cpp

	ln_state_table.cpp
	ln_state_table.hpp

	lds_grid.hpp
	lds_grid.cpp

	ld_prop_table.hpp
	ld_prop_table.cpp

	lns_table.hpp
	lns_table.cpp

	sort_proxy_model.hpp
	sort_proxy_model.cpp

	history_connections.hpp
	history_connections.cpp

	datasets_table.hpp
	datasets_table.cpp

	ds_signals_table.hpp
	ds_signals_table.cpp

	rcb_general_table.hpp
	rcb_general_table.cpp

	reports_table.hpp
	reports_table.cpp
)

qt_add_library(models_lib STATIC ${SOURCES})

target_link_libraries(models_lib PUBLIC app_lib core_lib cmd_lib)
target_link_libraries(models_lib PRIVATE Qt6::Core)

target_include_directories(models_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})