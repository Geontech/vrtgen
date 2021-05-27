cmake_minimum_required(VERSION 3.13)
include(GNUInstallDirs)

project(vrtgen-cpp VERSION 0.3.1 LANGUAGES CXX)

# On Mac OS X, give the system installation lowest priority so that virtual
# environments can be found
set(CMAKE_FIND_FRAMEWORK "last")
find_package(Python3 REQUIRED)

# Ensure that the base vrtgen Python library is available
execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import vrtgen"
    OUTPUT_QUIET
    ERROR_QUIET
    RESULT_VARIABLE import_vrtgen_error_code
)
if(NOT import_vrtgen_error_code EQUAL 0)
    message(FATAL_ERROR "vrtgen Python module not found")
endif()

# Support per-target JUnit test output at configure time
option(JUNIT_OUTPUT "Use JUnit output format for test results" OFF)

# Generate a version header
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen/version.hpp"
)

# Get the list of generated headers from the library generator (avoids need to
# modify this file if headers are added, renamed or removed)
execute_process(
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gencpp.py headers
    OUTPUT_VARIABLE generated_header_files
)
separate_arguments(generated_header_files UNIX_COMMAND ${generated_header_files})
list(TRANSFORM generated_header_files PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")

set(CMAKE_CLEAN_FILES ${generated_header_files})

# List template files used to generate headers, used to trigger a re-generate
# step if they are modified
set(template_path ${CMAKE_CURRENT_SOURCE_DIR}/python/vrtgen/backend/cpp/templates)
set(template_files
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/cif.hpp
    ${template_path}/enums.hpp
    ${template_path}/struct.hpp
)

add_custom_command(
    OUTPUT
        ${generated_header_files}
    COMMAND
        ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gencpp.py generate
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/gencpp.py
        ${template_files}
)
# Define an intermediate target to ensure that header generation only occurs
# once, as part of the vrtgen target
add_custom_target(generate-headers DEPENDS ${generated_header_files})

# Normal hand-written headers
set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/socket/socket_base.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/socket/socket_endpoint.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/socket/udp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types/field.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types/fixed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types/packed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types/size.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types/swap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/types.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/version.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vrtgen/vrtgen.hpp
)

add_library(vrtgen INTERFACE)
add_dependencies(vrtgen generate-headers)
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
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.hpp"
)

subdirs(external/Catch2)

# Compile everything with most warnings
add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra"
)

# Create an empty "check" target to hang tests off of
add_custom_target(check)

add_library(testutils OBJECT
    tests/utils/main.cpp
    tests/utils/bindump.cpp
)
target_link_libraries(testutils Catch2)
target_include_directories(testutils PUBLIC
    tests/include
)

add_executable(test_libvrtgen
    tests/libvrtgen/cif0.cpp
    tests/libvrtgen/control.cpp
    tests/libvrtgen/packed.cpp
    tests/libvrtgen/prologue.cpp
    tests/libvrtgen/socket.cpp
    tests/libvrtgen/types.cpp
    tests/libvrtgen/utils.cpp
)
target_link_libraries(test_libvrtgen vrtgen)
target_link_libraries(test_libvrtgen Catch2)
target_link_libraries(test_libvrtgen testutils)
target_compile_options(test_libvrtgen PRIVATE -Wall -Wextra -Wpedantic)

# Run the library tests, with optional JUnit output
if(JUNIT_OUTPUT)
    set(LIBVRTGEN_TEST_OPTIONS "-r junit" "-o test_libvrtgen-results.xml")
    set_property(DIRECTORY APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/test_libvrtgen-results.xml
    )
endif()
add_custom_target(check-libvrtgen
    COMMAND test_libvrtgen ${LIBVRTGEN_TEST_OPTIONS} $(TEST_OPTIONS)
)
add_dependencies(check check-libvrtgen)

# Helper function to generate C++ source from a YAML file using vrtpktgen
function(add_codegen_file TARGET YAML_FILE)
    get_filename_component(TEST_DIRNAME "${YAML_FILE}" DIRECTORY)
    get_filename_component(TEST_BASENAME "${YAML_FILE}" NAME_WE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIRNAME}/${TEST_BASENAME}.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIRNAME}/${TEST_BASENAME}.hpp
        COMMAND
            ${Python3_EXECUTABLE} -m vrtgen.main cpp --dir ${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIRNAME} ${YAML_FILE}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${YAML_FILE}
    )
    target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIRNAME}/${TEST_BASENAME}.cpp)
endfunction()

add_executable(test_codegen
    tests/codegen/test_basic.cpp
    tests/codegen/test_cif0.cpp
    tests/codegen/test_control_ids.cpp
    tests/codegen/test_timestamp.cpp
    tests/codegen/test_structs.cpp
    tests/codegen/test_user.cpp
)
add_codegen_file(test_codegen tests/codegen/yamls/basic.yaml)
add_codegen_file(test_codegen tests/codegen/yamls/cif0.yaml)
add_codegen_file(test_codegen tests/codegen/yamls/control_ids.yaml)
add_codegen_file(test_codegen tests/codegen/yamls/timestamp.yaml)
add_codegen_file(test_codegen tests/codegen/yamls/structs.yaml)
add_codegen_file(test_codegen tests/codegen/yamls/user.yaml)

target_link_libraries(test_codegen vrtgen)
target_link_libraries(test_codegen Catch2)
target_link_libraries(test_codegen testutils)
target_compile_options(test_codegen PRIVATE)
target_include_directories(test_codegen PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/tests/codegen
    # Explicitly add the build "vrtgen" include directory so that the relative
    # includes in packed.hpp can resolve generated headers
    ${CMAKE_CURRENT_BINARY_DIR}/include/vrtgen
)

# Run the code generator tests, with optional JUnit output
if(JUNIT_OUTPUT)
    set(CODEGEN_TEST_OPTIONS "-r junit" "-o test_codegen-results.xml")
    set_property(DIRECTORY APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/test_codegen-results.xml
    )
endif()
add_custom_target(check-codegen
    COMMAND test_codegen ${CODEGEN_TEST_OPTIONS} $(TEST_OPTIONS)
)
add_dependencies(check check-codegen)

# Packaging options
set(CPACK_PACKAGE_NAME libvrtgen)
set(CPACK_PACKAGE_VENDOR "Geon Technologies, LLC")
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
include(CPack)
