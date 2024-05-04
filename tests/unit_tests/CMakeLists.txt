# --------------------------------------------------
# Create unit tests executable
# --------------------------------------------------
add_executable(${NOX_UNIT_TESTS_EXE_NAME})
target_include_directories(
    ${NOX_UNIT_TESTS_EXE_NAME}
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_compile_features(${NOX_UNIT_TESTS_EXE_NAME} PUBLIC cxx_std_17)

if(NOX_ENABLE_COVERAGE)
    target_compile_options(
        ${NOX_UNIT_TESTS_EXE_NAME}
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>: -O0 -g --coverage>
    )

    target_link_libraries(
        ${NOX_UNIT_TESTS_EXE_NAME}
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>: --coverage>
    )
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY)
endif()

gtest_discover_tests(${NOX_UNIT_TESTS_EXE_NAME})

# --------------------------------------------------
# Set properties
# --------------------------------------------------
set_target_properties(${NOX_UNIT_TESTS_EXE_NAME} PROPERTIES FOLDER ${NOX_TESTS_FOLDER_NAME})

# --------------------------------------------------
# Add dependencies
# --------------------------------------------------
if(NOX_BUILD_RENDERER_OPENGL)
    add_dependencies(${NOX_UNIT_TESTS_EXE_NAME} ${NOX_OPENGL_LIB_NAME})
endif()

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
    ${NOX_UNIT_TESTS_EXE_NAME}
    PRIVATE
    ${NOX_LIB_NAME}
    GTest::gtest_main
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_UNIT_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/format_descriptor_tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugins_tests.cpp
)

target_sources(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE ${NOX_UNIT_TESTS_SOURCES})

# --------------------------------------------------
# Add subdirectories
# --------------------------------------------------
add_subdirectory(mocks)

nox_create_project_source_tree(${NOX_UNIT_TESTS_EXE_NAME})

add_custom_target(
    run-${NOX_UNIT_TESTS_EXE_NAME}
    COMMAND ${NOX_UNIT_TESTS_EXE_NAME}
)

set_target_properties(run-${NOX_UNIT_TESTS_EXE_NAME} PROPERTIES FOLDER ${NOX_TESTS_FOLDER_NAME})
