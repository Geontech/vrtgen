# vrtgen library testing
add_executable(test_vrtgen
    cif0.cpp
    cif1.cpp
    command.cpp
    header.cpp
    class_id.cpp
    types.cpp
    trailer.cpp
)
target_include_directories(test_vrtgen
    PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
target_link_libraries(test_vrtgen
    PRIVATE
    Catch2::Catch2WithMain
)
# Run the library tests, with optional JUnit output
if(JUNIT_OUTPUT)
    set(LIBVRTGEN_TEST_OPTIONS "-r junit" "-o test_libvrtgen-results.xml")
    set_property(DIRECTORY APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/test_vrtgen-results.xml
    )
endif()
# Add to 'check' target
add_custom_target(check-vrtgen
    COMMAND test_vrtgen ${VRTGEN_TEST_OPTIONS} $(TEST_OPTIONS)
)
add_dependencies(check check-vrtgen)