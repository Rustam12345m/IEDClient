# Breakpad client library — Linux only
# Builds the minimal set of sources needed for crash dump generation.

enable_language(ASM)

set(BREAKPAD_DIR ${CMAKE_SOURCE_DIR}/3rdparty/breakpad/src)

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

# Mark the .S file as ASM so CMake processes it correctly
set_source_files_properties(
    ${BREAKPAD_DIR}/common/linux/breakpad_getcontext.S
    PROPERTIES LANGUAGE ASM
)

add_library(breakpad_client STATIC ${BREAKPAD_SOURCES})

target_include_directories(breakpad_client PUBLIC
    ${BREAKPAD_DIR}
    ${BREAKPAD_DIR}/third_party/lss
)

target_compile_options(breakpad_client PRIVATE -Wno-sign-compare)
