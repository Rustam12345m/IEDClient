set(SOURCES
    ied_model_api_impl.hpp
    ied_model_api_impl.cpp

    ied_state_api_impl.hpp
    ied_state_api_impl.cpp

    ied_control_api_impl.hpp
    ied_control_api_impl.cpp

    ied_fs_api_impl.hpp
    ied_fs_api_impl.cpp

    libiec61850_adapter.hpp
    libiec61850_adapter.cpp

    libiec61850_tools.hpp
    libiec61850_tools.cpp
)
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

qt_add_library(lib61850_adapter STATIC ${SOURCES})

link_directories(${CMAKE_BUILD_RPATH}/3rdparty/libiec61850/src/)
target_link_libraries(lib61850_adapter PRIVATE Qt6::Core)
target_link_libraries(lib61850_adapter PRIVATE iec61850)

target_include_directories(lib61850_adapter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(lib61850_adapter PUBLIC ${LIB61850_INC})