set(NOX_EXAMPLES_BASE_LIB_NAME "base")

set(NOX_EXAMPLES_BASE_DIR "${PROJECT_SOURCE_DIR}/examples/base")
set(NOX_EXAMPLES_BASE_THIRD_PARTY_DIR "${NOX_EXAMPLES_BASE_DIR}/third_party")

set(NOX_EXAMPLES_BASE_FOLDER_NAME "${NOX_EXAMPLES_FOLDER_NAME}/base")
set(NOX_EXAMPLES_BASE_THIRD_PARTY_FOLDER_NAME "${NOX_EXAMPLES_BASE_FOLDER_NAME}/third_party")

# --------------------------------------------------
# Create target
# --------------------------------------------------
add_library(${NOX_EXAMPLES_BASE_LIB_NAME} STATIC)
target_include_directories(
    ${NOX_EXAMPLES_BASE_LIB_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

# --------------------------------------------------
# Specify compile options
# --------------------------------------------------
target_compile_features(${NOX_EXAMPLES_BASE_LIB_NAME} PRIVATE cxx_std_17)

# --------------------------------------------------
# Set properties
# --------------------------------------------------
set_target_properties(
    ${NOX_EXAMPLES_BASE_LIB_NAME}
    PROPERTIES
    FOLDER "${NOX_EXAMPLES_BASE_FOLDER_NAME}"
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${NOX_EXAMPLES_BASE_THIRD_PARTY_DIR}/glfw glfw)
set_target_properties(
    glfw
    PROPERTIES 
    FOLDER "${NOX_EXAMPLES_BASE_THIRD_PARTY_FOLDER_NAME}/glfw"
)
set_target_properties(
    update_mappings
    PROPERTIES
    FOLDER "${NOX_EXAMPLES_BASE_THIRD_PARTY_FOLDER_NAME}/glfw"
)

add_subdirectory(${NOX_EXAMPLES_BASE_THIRD_PARTY_DIR}/stb_image stb_image)
set_target_properties(
    stb_image
    PROPERTIES 
    FOLDER "${NOX_EXAMPLES_BASE_THIRD_PARTY_FOLDER_NAME}/stb_image"
)

target_link_libraries(
    ${NOX_EXAMPLES_BASE_LIB_NAME}
    PUBLIC
    ${NOX_LIB_NAME}
    PRIVATE
    glfw
    stb_image
)

# --------------------------------------------------
# Add subdirectories with sources
# --------------------------------------------------
add_subdirectory(src)

# --------------------------------------------------
# Add public headers to sources (for IDE)
# --------------------------------------------------
file(GLOB_RECURSE NOX_EXAMPLES_BASE_PUBLIC_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/base/*.h")
target_sources(${NOX_EXAMPLES_BASE_LIB_NAME} PRIVATE ${NOX_EXAMPLES_BASE_PUBLIC_HEADERS})

nox_create_project_source_tree(${NOX_EXAMPLES_BASE_LIB_NAME})
