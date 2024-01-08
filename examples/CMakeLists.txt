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
        base
    )
endmacro()

add_subdirectory(base)
add_subdirectory(sandbox)
