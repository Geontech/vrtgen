cmake_minimum_required(VERSION 3.13)
include(GNUInstallDirs)

set(generated_header_files
    ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen/packing/cif0.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen/packing/cif1.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen/packing/header.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen/packing/trailer.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen/enums.hpp
)
set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/fixed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/packed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/swap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types.hpp
)

set(CMAKE_CLEAN_FILES ${generated_header_files})

add_custom_command(
    OUTPUT
        ${generated_header_files}
    COMMAND
        python3 ${CMAKE_CURRENT_SOURCE_DIR}/gencpp.py
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/gencpp.py
        ${CMAKE_CURRENT_SOURCE_DIR}/templates/enums.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/templates/struct.hpp
)

add_library(vrtgen INTERFACE)
target_sources(vrtgen INTERFACE
    ${header_files}
    ${generated_header_files}
)
target_include_directories(vrtgen INTERFACE
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS vrtgen)
install(
    DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen
        ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
)

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/Catch2/include
)

add_executable(test_libvrtgen
    tests/main.cpp
    tests/packed.cpp
    tests/prologue.cpp
    tests/utils.cpp
)
target_link_libraries(test_libvrtgen vrtgen)
target_link_libraries(test_libvrtgen Catch2)
target_compile_options(test_libvrtgen PRIVATE -Wall)
target_compile_features(test_libvrtgen PRIVATE cxx_std_11)

add_custom_target(check
    COMMAND
        ${CMAKE_CURRENT_BINARY_DIR}/test_libvrtgen $(TEST_OPTIONS)
    DEPENDS
        test_libvrtgen
)
