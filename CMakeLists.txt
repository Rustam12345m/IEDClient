cmake_minimum_required(VERSION 3.14)

project(IEDMaster VERSION "1.0.0" LANGUAGES CXX)

set(PROJECT "IEDMaster")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SOURCES
	src/main.cpp
	src/app_core.h
	src/app_core.cpp
	src/main_presenter.h
	src/main_presenter.cpp

	src/core/tnode.h
	src/core/data_attribute.h
	src/core/data_object.h
	src/core/logical_node.h
	src/core/logical_device.h
	src/core/dataset.h
	src/core/report_block.h
	src/core/fs_tree.h
	src/core/object_tree.h
	src/core/object_tree.cpp
	src/core/tnode_factory.h

	src/core/ln_flat_state.h
	src/core/ln_flat_state.cpp

	src/cmd/base_command.h
	src/cmd/command_queue.h
	src/cmd/cmd_thread.h
	src/cmd/cmd_thread.cpp
	src/cmd/lib_interface.h
	src/cmd/lib61850_adapter.h
	src/cmd/lib61850_adapter.cpp
	src/cmd/update_lnode_cmd.h
	src/cmd/update_lnode_cmd.cpp
	src/cmd/get_filelist_cmd.h
	src/cmd/get_filelist_cmd.cpp
	src/cmd/connect_cmd.h
	src/cmd/connect_cmd.cpp
	src/cmd/get_ldlist_cmd.h
	src/cmd/get_ldlist_cmd.cpp

	src/models/sort_proxy_model.h
	src/models/sort_proxy_model.cpp
	src/models/ld_listmodel.h
	src/models/ld_listmodel.cpp
	src/models/do_tablemodel.h
	src/models/do_tablemodel.cpp
	src/models/ln_tablemodel.h
	src/models/ln_tablemodel.cpp
	src/models/fs_tablemodel.h
	src/models/fs_tablemodel.cpp
	src/models/events_tablemodel.h
	src/models/events_tablemodel.cpp

	ui/resources.qrc
)

# External library - libiec61850
# IEDMaster's options for 61850
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
configure_file(builder_information.h.in ${CMAKE_SOURCE_DIR}/src/builder_information.h)

# Qt
set(Qt6_DIR /opt/Qt/6.5.2/gcc_64/lib/cmake/Qt6/)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6 COMPONENTS Core Qml Quick QuickControls2 QuickTemplates2 REQUIRED)

qt6_add_resources(QRC_SOURCE ${CMAKE_SOURCE_DIR}/ui/resources.qrc)
qt_add_executable(IEDMaster ${PROJECT_SOURCES} ${QRC_SOURCE})

target_link_libraries(IEDMaster PRIVATE Qt::Core Qt::Qml Qt::Quick)
target_link_libraries(IEDMaster PRIVATE iec61850)

# Unit-tests
#enable_testing()
#add_test(IEDMaster_utests test_version)

# CPack
#install(TARGETS IEDMaster RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
