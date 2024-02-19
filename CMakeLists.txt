cmake_minimum_required(VERSION 3.15)
project(vrtgen VERSION 0.8.0 LANGUAGES CXX)
include(GNUInstallDirs)

# Check if being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED VRTGEN_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(VRTGEN_MASTER_PROJECT ON)
    else()
        set(VRTGEN_MASTER_PROJECT OFF)
    endif()
endif()

##
## Options
##
option(VRTGEN_INSTALL "Generate the install target" ${VRTGEN_MASTER_PROJECT})
option(VRTGEN_BUILD_TESTS "Build tests" OFF)
option(VRTGEN_BUILD_DOCS "Build docs with Doxygen" OFF)

##
## Individual libraries
##
add_subdirectory(lib/cpp)
add_subdirectory(lib/python)
