

# Head files
set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/cif0.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/cif1.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/cif2.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/enums.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/header.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/indicator_fields.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/prologue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/state_event_indicators.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing/trailer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/packing.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/socket/socket_base.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/socket/socket_endpoint.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/socket/tcp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/socket/udp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/socket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/fixed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/packed.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/swap.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types/uuid.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/types.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utility.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen.hpp
)

add_library(vrtgen INTERFACE)
target_sources(vrtgen INTERFACE ${header_files})
target_include_directories(vrtgen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../)

message("TEST TEST ${CMAKE_CURRENT_SOURCE_DIR}")

install(TARGETS vrtgen)
install(
    DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.hpp"
)