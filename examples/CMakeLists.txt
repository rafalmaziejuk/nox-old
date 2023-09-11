macro(add_example_project NAME)
    add_executable(${NAME})
    target_compile_features(${NAME} PRIVATE cxx_std_17)
    target_link_libraries(${NAME} PRIVATE ${NOX_LIB_NAME})
    set_target_properties(${NAME} PROPERTIES FOLDER "${NOX_EXAMPLES_FOLDER_NAME}")
endmacro()

add_subdirectory(sandbox)
