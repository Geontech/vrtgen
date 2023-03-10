cmake_minimum_required(VERSION 3.15)
project(vrtgen-cpp VERSION 0.7.0 LANGUAGES CXX)
include(GNUInstallDirs)

# Set the C++ version required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On Mac OS X, give the system installation lowest priority so that virtual
# environments can be found
set(CMAKE_FIND_FRAMEWORK "LAST")
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

add_subdirectory(include)

if (BUILD_TESTS)
# Create an empty "check" target to hang tests off of
add_custom_target(check)

# Add Catch2 subdir
add_subdirectory(tests/cpp/include/external/catch2)

# Compile everything with most warnings
add_compile_options("-Wall" "-Wpedantic" "-Wextra")

# Utility testing
add_library(testutils OBJECT tests/cpp/main.cpp)
target_link_libraries(testutils Catch2)
target_include_directories(testutils PUBLIC tests/cpp/include)

# libvrtgen testing
add_executable(test_libvrtgen
    tests/libvrtgen/cif0.cpp
    tests/libvrtgen/cif1.cpp
    tests/libvrtgen/command.cpp
    tests/libvrtgen/header.cpp
    tests/libvrtgen/class_id.cpp
    tests/libvrtgen/types.cpp
    tests/libvrtgen/trailer.cpp
)
target_link_libraries(test_libvrtgen vrtgen)
target_link_libraries(test_libvrtgen Catch2)
target_link_libraries(test_libvrtgen testutils)
target_compile_options(test_libvrtgen PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(test_libvrtgen PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

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
function(add_codegen_file TARGET YAML_FILE FILE_LIST)
    set(TEST_DIRNAME tests/codegen/cpp)
    get_filename_component(TEST_BASENAME "${YAML_FILE}" NAME_WE)
    set(hpp_list ${FILE_LIST})
    set(cpp_list ${FILE_LIST})
    list(TRANSFORM hpp_list APPEND ".hpp")
    list(TRANSFORM cpp_list APPEND ".cpp")
    add_custom_command(
        OUTPUT
        ${hpp_list}
        ${cpp_list}
        COMMAND
        ${Python3_EXECUTABLE} -m vrtgen.main cpp --dir ${CMAKE_CURRENT_BINARY_DIR}/${TEST_DIRNAME}/${TEST_BASENAME} --namespace ${TEST_BASENAME}_ns ${YAML_FILE}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${YAML_FILE}
    )
    target_sources(${TARGET} PRIVATE ${cpp_list})
endfunction()

add_executable(test_codegen
    tests/codegen/cpp/test_header.cpp
    tests/codegen/cpp/test_stream_id.cpp
    tests/codegen/cpp/test_class_id.cpp
    tests/codegen/cpp/test_timestamp.cpp
    tests/codegen/cpp/test_basic.cpp
    tests/codegen/cpp/test_data.cpp
    tests/codegen/cpp/test_trailer.cpp
    tests/codegen/cpp/test_context.cpp
    tests/codegen/cpp/test_command.cpp
)

add_subdirectory(tests/codegen/cpp)

add_codegen_file(test_codegen tests/codegen/yamls/header.yaml "${header_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/stream_id.yaml "${stream_id_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/class_id.yaml "${class_id_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/timestamp.yaml "${timestamp_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/basic.yaml "${basic_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/data.yaml "${data_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/trailer.yaml "${trailer_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/context.yaml "${context_gen_list}")
add_codegen_file(test_codegen tests/codegen/yamls/command.yaml "${command_gen_list}")

target_link_libraries(test_codegen vrtgen)
target_link_libraries(test_codegen Catch2)
target_link_libraries(test_codegen testutils)
target_compile_options(test_codegen PRIVATE -g)
target_include_directories(test_codegen PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/tests/codegen/cpp
    # Explicitly add the build "vrtgen" include directory so that the relative
    # includes in packed.hpp can resolve generated headers
    ${PROJECT_SOURCE_DIR}/include
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
endif()

# Create an empty "docs" target to hang doxygen off of
add_custom_target(docs)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_subdirectory(docs)
else (DOXYGEN_FOUND)
    message(STATUS "Doxygen not found, not building docs")
endif (DOXYGEN_FOUND)

# Packaging options
set(CPACK_PACKAGE_NAME libvrtgen)
set(CPACK_PACKAGE_VENDOR "Geon Technologies, LLC")
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
set(CPACK_RPM_PACKAGE_REQUIRES "vrtgen >= 0.6.0")
include(CPack)
