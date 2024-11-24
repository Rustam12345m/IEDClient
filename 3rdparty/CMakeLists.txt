# 3rdparty projects

# libiec61850
set(BUILD_EXAMPLES CACHE BOOL OFF)
# set(CONFIG_INCLUDE_GOOSE_SUPPORT CACHE BOOL OFF)
add_subdirectory(libiec61850/)

# GoogleTests
if (BUILD_TESTS)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE) # To avoid installing all libs stuff
    add_subdirectory(googletest/)
endif()

