# --------------------------------------------------
# Create target
# --------------------------------------------------
add_library(${NOX_TESTS_UTILITIES_LIB_NAME} STATIC)
target_include_directories(
    ${NOX_TESTS_UTILITIES_LIB_NAME}
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

target_compile_features(${NOX_TESTS_UTILITIES_LIB_NAME} PUBLIC cxx_std_17)

get_filename_component(NOX_TEST_DATA_DIR "${PROJECT_SOURCE_DIR}/tests/data" ABSOLUTE)
configure_file(config.h.in "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

# --------------------------------------------------
# Set properties
# --------------------------------------------------
set_target_properties(${NOX_TESTS_UTILITIES_LIB_NAME} PROPERTIES FOLDER ${NOX_TESTS_FOLDER_NAME})

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
    ${NOX_TESTS_UTILITIES_LIB_NAME}
    PRIVATE
    ${NOX_LIB_NAME}
    glfw
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_TESTS_HELPERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/window.h
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
)

target_sources(${NOX_TESTS_UTILITIES_LIB_NAME} PRIVATE ${NOX_TESTS_HELPERS_SOURCES})

nox_create_project_source_tree(${NOX_TESTS_UTILITIES_LIB_NAME})
