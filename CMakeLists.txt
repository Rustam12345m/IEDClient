cmake_minimum_required(VERSION 3.14)

project(IEDClient VERSION "1.0.0" LANGUAGES CXX)

set(PROJECT "IEDClient")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SOURCES
	src/main.cpp
	src/app_core.hpp
	src/app_core.cpp
	src/main_presenter.hpp
	src/main_presenter.cpp

	src/core/tnode.hpp
	src/core/data_attribute.hpp
	src/core/data_object.hpp
	src/core/logical_node.hpp
	src/core/logical_device.hpp
	src/core/dataset.hpp
	src/core/report_block.hpp
	src/core/fs_tree.hpp
	src/core/object_tree.hpp
	src/core/object_tree.cpp
	src/core/tnode_factory.hpp

	src/core/ln_flat_state.hpp
	src/core/ln_flat_state.cpp

	src/cmd/base_command.hpp
	src/cmd/command_queue.hpp
	src/cmd/cmd_thread.hpp
	src/cmd/cmd_thread.cpp
	src/cmd/lib_interface.hpp
	src/cmd/lib61850_adapter.hpp
	src/cmd/lib61850_adapter.cpp
	src/cmd/update_lnode_cmd.hpp
	src/cmd/update_lnode_cmd.cpp
	src/cmd/get_filelist_cmd.hpp
	src/cmd/get_filelist_cmd.cpp
	src/cmd/connect_cmd.hpp
	src/cmd/connect_cmd.cpp
	src/cmd/get_ldlist_cmd.hpp
	src/cmd/get_ldlist_cmd.cpp

	src/models/sort_proxy_model.hpp
	src/models/sort_proxy_model.cpp
	src/models/ld_listmodel.hpp
	src/models/ld_listmodel.cpp
	src/models/do_tablemodel.hpp
	src/models/do_tablemodel.cpp
	src/models/ln_tablemodel.hpp
	src/models/ln_tablemodel.cpp
	src/models/fs_tablemodel.hpp
	src/models/fs_tablemodel.cpp
	src/models/events_tablemodel.hpp
	src/models/events_tablemodel.cpp

	ui/resources.qrc
)

# External library - libiec61850
# IEDClient's options for 61850
set(BUILD_EXAMPLES CACHE BOOL OFF)
set(BUILD_PYTHON_BINDINGS CACHE BOOL OFF)
set(CONFIG_INCLUDE_GOOSE_SUPPORT CACHE BOOL OFF)

add_subdirectory(depends/libiec61850/)
link_directories(${CMAKE_BUILD_RPATH}/depends/libiec61850/src/)

# All libiec61850 headers
include_directories(
	${CMAKE_SOURCE_DIR}/src/
	${CMAKE_SOURCE_DIR}/src/models/
	${CMAKE_SOURCE_DIR}/src/core/
	${CMAKE_SOURCE_DIR}/src/cmd/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/config/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/common/inc/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/goose/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/sampled_values/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/hal/inc/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/iec61850/inc/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/mms/inc/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/mms/iso_mms/asn1c/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/logging/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/src/tls/
	${CMAKE_SOURCE_DIR}/depends/libiec61850/hal/inc/
)

# Software version and other stub
configure_file(builder_information.hpp.in ${CMAKE_SOURCE_DIR}/src/builder_information.hpp)

# Qt
set(Qt6_DIR /opt/Qt/6.5.2/gcc_64/lib/cmake/Qt6/)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6 COMPONENTS Core Qml Quick QuickControls2 QuickTemplates2 REQUIRED)

qt6_add_resources(QRC_SOURCE ${CMAKE_SOURCE_DIR}/ui/resources.qrc)
qt_add_executable(IEDClient ${PROJECT_SOURCES} ${QRC_SOURCE})

target_link_libraries(IEDClient PRIVATE Qt::Core Qt::Qml Qt::Quick)
target_link_libraries(IEDClient PRIVATE iec61850)

# Unit-tests
#enable_testing()
#add_test(IEDClient_utests test_version)

# CPack
#install(TARGETS IEDClient RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
