# Breakpad client library — Linux and Windows
# On unsupported platforms, creates an empty interface target.

set(BREAKPAD_DIR ${CMAKE_SOURCE_DIR}/3rdparty/breakpad/src)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    enable_language(ASM)

    set(BREAKPAD_SOURCES
        ${BREAKPAD_DIR}/client/linux/crash_generation/crash_generation_client.cc
        ${BREAKPAD_DIR}/client/linux/handler/exception_handler.cc
        ${BREAKPAD_DIR}/client/linux/handler/minidump_descriptor.cc
        ${BREAKPAD_DIR}/client/linux/dump_writer_common/thread_info.cc
        ${BREAKPAD_DIR}/client/linux/dump_writer_common/ucontext_reader.cc
        ${BREAKPAD_DIR}/client/linux/log/log.cc
        ${BREAKPAD_DIR}/client/linux/microdump_writer/microdump_writer.cc
        ${BREAKPAD_DIR}/client/linux/minidump_writer/linux_dumper.cc
        ${BREAKPAD_DIR}/client/linux/minidump_writer/linux_ptrace_dumper.cc
        ${BREAKPAD_DIR}/client/linux/minidump_writer/minidump_writer.cc
        ${BREAKPAD_DIR}/client/linux/minidump_writer/pe_file.cc
        ${BREAKPAD_DIR}/client/minidump_file_writer.cc
        ${BREAKPAD_DIR}/common/convert_UTF.cc
        ${BREAKPAD_DIR}/common/md5.cc
        ${BREAKPAD_DIR}/common/string_conversion.cc
        ${BREAKPAD_DIR}/common/linux/breakpad_getcontext.S
        ${BREAKPAD_DIR}/common/linux/elfutils.cc
        ${BREAKPAD_DIR}/common/linux/file_id.cc
        ${BREAKPAD_DIR}/common/linux/guid_creator.cc
        ${BREAKPAD_DIR}/common/linux/linux_libc_support.cc
        ${BREAKPAD_DIR}/common/linux/memory_mapped_file.cc
        ${BREAKPAD_DIR}/common/linux/safe_readlink.cc
    )

    set_source_files_properties(
        ${BREAKPAD_DIR}/common/linux/breakpad_getcontext.S
        PROPERTIES LANGUAGE ASM
    )

    add_library(breakpad_client STATIC ${BREAKPAD_SOURCES})

    target_include_directories(breakpad_client PUBLIC ${BREAKPAD_DIR})
    target_include_directories(breakpad_client PUBLIC ${BREAKPAD_DIR}/third_party/lss)
    target_compile_options(breakpad_client PRIVATE -Wno-sign-compare)
    target_compile_definitions(breakpad_client PUBLIC BREAKPAD_ENABLED BREAKPAD_LINUX)

elseif(WIN32)
    set(BREAKPAD_SOURCES
        ${BREAKPAD_DIR}/client/windows/handler/exception_handler.cc
        ${BREAKPAD_DIR}/client/windows/crash_generation/crash_generation_client.cc
        ${BREAKPAD_DIR}/common/windows/guid_string.cc
    )

    add_library(breakpad_client STATIC ${BREAKPAD_SOURCES})

    target_include_directories(breakpad_client PUBLIC ${BREAKPAD_DIR})
    target_compile_definitions(breakpad_client PUBLIC BREAKPAD_ENABLED BREAKPAD_WINDOWS)
    target_link_libraries(breakpad_client PRIVATE dbghelp)

else()
    add_library(breakpad_client INTERFACE)
endif()
