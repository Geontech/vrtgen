list(APPEND basic_gen_list
    "basic_data_packet"
    "basic_control_packet"
    "basic_control_packet_ack"
    "basic_context_packet"
)
list(TRANSFORM basic_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(basic_gen_list "${basic_gen_list}" PARENT_SCOPE)

list(APPEND context_gen_list
    "test_context1"
    "test_context2"
    "test_context4"
    "test_context5"
    "test_context6"
    "test_context7"
    "test_context_association_lists"
    "test_context_index_list"
    "test_context_sector_step_scan"
)
list(TRANSFORM context_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(context_gen_list "${context_gen_list}" PARENT_SCOPE)

list(APPEND data_gen_list
    "test_data1"
    "test_data2"
    "test_data3"
    "test_data4"
    "test_data5"
    "test_data6"
    "test_data7"
    "test_data8"
    "test_data9"
    "test_data10"
    "test_data11"
)
list(TRANSFORM data_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(data_gen_list "${data_gen_list}" PARENT_SCOPE)

list(APPEND stream_id_gen_list
    "test_stream_id_data3"
    "test_stream_id_data4"
    "without_stream_id_context"
    "without_stream_id_control"
    "without_stream_id_data"
    "with_stream_id_context"
    "with_stream_id_control"
    "with_stream_id_data"
)
list(TRANSFORM stream_id_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(stream_id_gen_list "${stream_id_gen_list}" PARENT_SCOPE)

list(APPEND timestamp_gen_list
    "timestamp_context1"
    "timestamp_control1"
    "timestamp_data1"
)
list(TRANSFORM timestamp_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(timestamp_gen_list "${timestamp_gen_list}" PARENT_SCOPE)

list(APPEND packetheader_gen_list
    "test_header_ack_packet"
    "test_header_ack_t_s_packet"
    "test_header_context_not_v49d0_packet"
    "test_header_context_packet"
    "test_header_context_t_s_packet"
    "test_header_control_packet"
    "test_header_control_t_s_packet"
    "test_header_data_packet"
    "test_header_data_t_s_packet"
    "test_header_tsm_context8"
)
list(TRANSFORM packetheader_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(packetheader_gen_list "${packetheader_gen_list}" PARENT_SCOPE)

list(APPEND classid_gen_list
    "test_context_class_id1"
    "test_context_class_id2"
    "test_control_class_id1"
    "test_control_class_id2"
    "test_data_class_id1"
    "test_data_class_id2"
)
list(TRANSFORM classid_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(classid_gen_list "${classid_gen_list}" PARENT_SCOPE)

list(APPEND packettrailer_gen_list
    "trailer_data1"
    "trailer_data2"
    "trailer_data3"
    "trailer_data4"
    "trailer_data5"
    "trailer_data6"
)
list(TRANSFORM packettrailer_gen_list PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
set(packettrailer_gen_list "${packettrailer_gen_list}" PARENT_SCOPE)