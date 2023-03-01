# Header files
set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/cif0.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/cif1.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/cif2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/enums.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/header.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/indicator_fields.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/prologue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/state_event_indicators.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing/trailer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/packing.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/socket/socket_base.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/socket/socket_endpoint.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/socket/tcp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/socket/udp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/socket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/types/fixed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/types/packed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/types/swap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/types/uuid.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/types.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/utility.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/version.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen/vrtgen.hpp
)

# Project library
add_library(vrtgen INTERFACE)
target_include_directories(
    vrtgen
    INTERFACE
    $<BUILD_INTERFACE:${vrtgen_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install
install(
    TARGETS vrtgen
    EXPORT vrtgen_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Versioning
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "vrtgenConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR include)
endif()

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/vrtgenConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/vrtgenConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vrtgen/cmake
    PATH_VARS INCLUDE_INSTALL_DIR
)

install(
    EXPORT vrtgen_Targets
    FILE vrtgenTargets.cmake
    NAMESPACE vrtgen::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vrtgen/cmake
)

install(
    FILES
    "${PROJECT_BINARY_DIR}/vrtgenConfig.cmake"
    "${PROJECT_BINARY_DIR}/include/vrtgenConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vrtgen/cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "vrtgenConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/vrtgen
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.hpp"
)
