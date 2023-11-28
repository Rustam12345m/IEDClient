# Building core's classes to a static library

set(CORE_SOURCES
	item.hpp
	item_factory.hpp
	item_factory.cpp

	sub_attribute.hpp
	data_attribute.hpp
	data_object.hpp
	logical_node.hpp
	logical_device.hpp

	dataset.hpp
	report_control_block.hpp
	goose_control_block.hpp
	sv_control_block.hpp

	fs_model.hpp

	data_model.hpp
	data_model.cpp
	data_model_builder.hpp
	data_model_builder.cpp

	ln_flat_state.hpp
	ln_flat_state.cpp

	ied_object.hpp
	ied_object.cpp

	server_identify.hpp
)

qt_add_library(core_lib STATIC ${CORE_SOURCES})

target_link_libraries(core_lib PRIVATE Qt6::Core)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})