set(NOX_EXAMPLES_THIRD_PARTY_FOLDER_NAME "${NOX_EXAMPLES_FOLDER_NAME}/third_party")
set(NOX_EXAMPLES_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/examples/third_party")

macro(add_example_project NAME)
    # --------------------------------------------------
    # Create target
    # --------------------------------------------------
    add_executable(${NAME})

    # --------------------------------------------------
    # Specify compile options
    # --------------------------------------------------
    target_compile_features(${NAME} PRIVATE cxx_std_17)

    # --------------------------------------------------
    # Set properties
    # --------------------------------------------------
    set_target_properties(${NAME} PROPERTIES FOLDER "${NOX_EXAMPLES_FOLDER_NAME}")

    # --------------------------------------------------
    # Link libraries
    # --------------------------------------------------
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(${NOX_EXAMPLES_THIRD_PARTY_DIR}/glfw glfw)
    set_target_properties(
        glfw
        PROPERTIES 
        FOLDER "${NOX_EXAMPLES_THIRD_PARTY_FOLDER_NAME}/glfw"
    )
    set_target_properties(
        update_mappings
        PROPERTIES
        FOLDER "${NOX_EXAMPLES_THIRD_PARTY_FOLDER_NAME}/glfw"
    )

    target_link_libraries(
        ${NAME}
        PRIVATE
        ${NOX_LIB_NAME}
        glfw
    )
endmacro()

add_subdirectory(sandbox)
