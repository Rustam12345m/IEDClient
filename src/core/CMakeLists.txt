# Building core's classes to a static library

set(CORE_SOURCES
	item.hpp
	item_factory.hpp
	data_attribute.hpp
	data_object.hpp
	logical_node.hpp
	logical_device.hpp
	dataset.hpp
	report_block.hpp
	fs_tree.hpp
	ied_tree.hpp
	ied_tree.cpp
	ln_flat_state.hpp
	ln_flat_state.cpp
)

qt_add_library(core_lib STATIC ${CORE_SOURCES})

target_link_libraries(core_lib PRIVATE Qt6::Core)

target_include_directories(core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})