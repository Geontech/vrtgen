find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Set the C++ version required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(pybind11)

pybind11_add_module(pyvrtgen
    vrtgen/vrtgen.cpp
    vrtgen/enums.cpp
    vrtgen/header.cpp
    vrtgen/prologue.cpp
    vrtgen/state_event_indicators.cpp
    vrtgen/trailer.cpp
    vrtgen/indicator_fields.cpp
    vrtgen/cif0.cpp
    vrtgen/cif1.cpp
    vrtgen/cif2.cpp
    vrtgen/cif7.cpp
    vrtgen/command.cpp
)

target_include_directories(pyvrtgen
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../cpp
)

if(VRTGEN_INSTALL)
    install(
        TARGETS pyvrtgen
        DESTINATION ${Python3_SITELIB}
    )
endif()
