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

# Helper function to generate C++ source from a YAML file using vrtpktgen
function(add_codegen_file TARGET YAML_FILE FILE_LIST)
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
        ${Python3_EXECUTABLE} -m vrtgen.main cpp --dir ${CMAKE_CURRENT_BINARY_DIR}/${TEST_BASENAME} --namespace ${TEST_BASENAME}_ns ${YAML_FILE}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
            ${YAML_FILE}
    )
    target_sources(${TARGET} PRIVATE ${cpp_list})
endfunction()

# Codegen test executable
add_executable(test_codegen
    test_header.cpp
    test_stream_id.cpp
    test_class_id.cpp
    test_timestamp.cpp
    test_basic.cpp
    test_data.cpp
    test_trailer.cpp
    test_context.cpp
    test_command.cpp
)
target_include_directories(test_codegen
    PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(test_codegen
    PRIVATE
    Catch2::Catch2WithMain
)

# Generated source lists
list(APPEND header_gen_list
    "header/test_header_ack_packet"
    "header/test_header_ack_t_s_packet"
    "header/test_header_context_not_v49d0_packet"
    "header/test_header_context_packet"
    "header/test_header_context_t_s_packet"
    "header/test_header_control_packet"
    "header/test_header_control_t_s_packet"
    "header/test_header_data_packet"
    "header/test_header_data_t_s_packet"
    "header/test_header_tsm_context8"
)
list(TRANSFORM header_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(header_gen_list "${header_gen_list}" PARENT_SCOPE)

list(APPEND stream_id_gen_list
    "stream_id/test_stream_id_data3"
    "stream_id/test_stream_id_data4"
    "stream_id/without_stream_id_context"
    "stream_id/without_stream_id_control"
    "stream_id/without_stream_id_data"
    "stream_id/with_stream_id_context"
    "stream_id/with_stream_id_control"
    "stream_id/with_stream_id_data"
)
list(TRANSFORM stream_id_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(stream_id_gen_list "${stream_id_gen_list}" PARENT_SCOPE)

list(APPEND class_id_gen_list
    "class_id/test_context_class_id1"
    "class_id/test_context_class_id2"
    "class_id/test_control_class_id1"
    "class_id/test_control_class_id2"
    "class_id/test_data_class_id1"
    "class_id/test_data_class_id2"
)
list(TRANSFORM class_id_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(class_id_gen_list "${class_id_gen_list}" PARENT_SCOPE)

list(APPEND timestamp_gen_list
    "timestamp/timestamp_context1"
    "timestamp/timestamp_control1"
    "timestamp/timestamp_data1"
)
list(TRANSFORM timestamp_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(timestamp_gen_list "${timestamp_gen_list}" PARENT_SCOPE)

list(APPEND basic_gen_list
    "basic/basic_data_packet"
    "basic/basic_control_packet"
    "basic/basic_control_packet_ack"
    "basic/basic_context_packet"
)
list(TRANSFORM basic_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(basic_gen_list "${basic_gen_list}" PARENT_SCOPE)

list(APPEND data_gen_list
    "data/test_data1"
    "data/test_data2"
    "data/test_data3"
    "data/test_data4"
    "data/test_data5"
    "data/test_data6"
    "data/test_data7"
    "data/test_data8"
    "data/test_data9"
    "data/test_data10"
    "data/test_data12"
    "data/test_data13"
)
list(TRANSFORM data_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(data_gen_list "${data_gen_list}" PARENT_SCOPE)

list(APPEND trailer_gen_list
    "trailer/trailer_data1"
    "trailer/trailer_data2"
    "trailer/trailer_data3"
    "trailer/trailer_data4"
    "trailer/trailer_data5"
    "trailer/trailer_data6"
)
list(TRANSFORM trailer_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(trailer_gen_list "${trailer_gen_list}" PARENT_SCOPE)

list(APPEND context_gen_list
    "context/test_context1"
    "context/test_context2"
    "context/test_context4"
    "context/ref_point_id_optional"
    "context/ref_point_id_required"
    "context/bandwidth_optional"
    "context/bandwidth_required"
    "context/test_basic_cif1"
    "context/test_basic_cif2"
    "context/test_basic_cif2_optional"
    "context/test_basic_cif7"
    "context/test_cif7_attributes"
    "context/test_cif7_all_attributes"
    "context/user_defined_discrete_io"
    "context/required_discrete_io"
    "context/not_user_defined_discrete_io"
    "context/test_all_generate_params"
    "context/cif2_uuid_fields"
    # "test_context5"
    # "test_context6"
    "context/test_context8"
    # "test_context_association_lists"
    # "test_context_index_list"
    # "test_context_sector_step_scan"
)
list(TRANSFORM context_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(context_gen_list "${context_gen_list}" PARENT_SCOPE)

list(APPEND command_gen_list
    "command/test_command_packet1"
    "command/test_ack_packet1"
    "command/test_command_packet3"
    "command/test_ack_packet3"
    "command/test_command_packet4"
    "command/test_ack_packet4"
    "command/test_command_packet5"
    "command/test_ack_packet5"
    "command/test_command_packet9"
    "command/test_ack_packet9"
    "command/test_command_packet_wif010"
    "command/test_ack_packet_wif010"
    "command/test_command_packet_wif110"
    "command/test_ack_packet_wif110"
    "command/test_command_packet11"
    "command/test_ack_packet11"
    # "command/test_command_packet_no_action12"
    # "command/test_command_packet_dry_run12"
    # "command/test_command_packet_exectute12"
)
list(TRANSFORM command_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(command_gen_list "${command_gen_list}" PARENT_SCOPE)

add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/header.yaml "${header_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/stream_id.yaml "${stream_id_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/class_id.yaml "${class_id_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/timestamp.yaml "${timestamp_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/basic.yaml "${basic_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/data.yaml "${data_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/trailer.yaml "${trailer_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/context.yaml "${context_gen_list}")
add_codegen_file(test_codegen ${PROJECT_SOURCE_DIR}/tests/codegen/yamls/command.yaml "${command_gen_list}")

# Run the code generator tests, with optional JUnit output
if(JUNIT_OUTPUT)
    set(CODEGEN_TEST_OPTIONS "-r junit" "-o test_codegen-results.xml")
    set_property(DIRECTORY APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/test_codegen-results.xml
    )
endif()

# Add to 'check' target
add_custom_target(check-codegen
    COMMAND test_codegen ${CODEGEN_TEST_OPTIONS} $(TEST_OPTIONS)
)
add_dependencies(check check-codegen)