# Emulator of Server IEC 61850 - MMS
set(SERV_EMUL_BIN big_server)
set(SERV_EXAMPLE_SRC
	big_server.cpp
	logical_device.hpp
	logical_node.hpp
	lphd_lnode.hpp
	lln0_lnode.hpp
	ggio_lnode.hpp)

set(LIB61850_INC
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/config/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/hal/inc/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/iec61850/inc/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/common/inc/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/mms/inc/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/tls/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/mms/iso_mms/asn1c/
	${CMAKE_SOURCE_DIR}/3rdparty/libiec61850/src/logging/
)

add_executable(${SERV_EMUL_BIN} ${SERV_EXAMPLE_SRC})

target_include_directories(${SERV_EMUL_BIN} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${LIB61850_INC})
target_link_libraries(${SERV_EMUL_BIN} PRIVATE iec61850)