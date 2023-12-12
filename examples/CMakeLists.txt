set(NOX_EXAMPLES_THIRD_PARTY_FOLDER_NAME "${NOX_EXAMPLES_FOLDER_NAME}/third_party")
set(NOX_EXAMPLES_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/examples/third_party")
set(NOX_EXAMPLES_BASE_LIB_NAME "base")

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
    target_link_libraries(
        ${NAME}
        PRIVATE
        ${NOX_EXAMPLES_BASE_LIB_NAME}
    )
endmacro()

add_subdirectory(base)
add_subdirectory(sandbox)
