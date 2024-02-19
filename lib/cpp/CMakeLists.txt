# Set the C++ version required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##
## Library target
##
add_library(vrtgen INTERFACE)
add_library(vrtgen::vrtgen ALIAS vrtgen)
target_compile_features(vrtgen INTERFACE cxx_std_20)
target_include_directories(
    vrtgen
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# Create a pkg-config file, so other tools can find this.
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vrtgen.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/vrtgen.pc"
)

##
## Install library target
##
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "vrtgenConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)
if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR include)
endif()
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vrtgenConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/vrtgenConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vrtgen/cmake
    PATH_VARS INCLUDE_INSTALL_DIR
)
if(VRTGEN_INSTALL)
    install(
        DIRECTORY vrtgen/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vrtgen
    )
    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/vrtgenConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/vrtgenConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vrtgen/cmake
    )
    install(
        TARGETS vrtgen
        EXPORT vrtgen_Targets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    install(
        EXPORT vrtgen_Targets
        FILE vrtgenTargets.cmake
        NAMESPACE vrtgen::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vrtgen/cmake
    )
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/vrtgen.pc"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/pkgconfig
    )
endif(VRTGEN_INSTALL)

##
## Unit tests
##
if(VRTGEN_BUILD_TESTS)
    # Fetch and setup Catch2 for testing
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)
    # Create an empty "check" target to hang tests off of
    add_custom_target(check)
    # Compile everything with most warnings
    add_compile_options("-Wall" "-Wpedantic" "-Wextra")
    # Library tests
    add_subdirectory(tests/vrtgen)
    # Code generator tests
    add_subdirectory(tests/codegen)
endif(VRTGEN_BUILD_TESTS)

##
## Documentation
##
if(VRTGEN_BUILD_DOCS)
    # Create an empty "docs" target to hang doxygen off of
    add_custom_target(docs)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        add_subdirectory(docs)
    else (DOXYGEN_FOUND)
        message(STATUS "Doxygen not found, not building docs")
    endif (DOXYGEN_FOUND)
endif(VRTGEN_BUILD_DOCS)
