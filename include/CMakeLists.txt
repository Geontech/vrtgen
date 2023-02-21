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

add_library(vrtgen INTERFACE)
target_sources(vrtgen INTERFACE ${header_files})
target_include_directories(vrtgen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS vrtgen)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vrtgen
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.hpp"
)