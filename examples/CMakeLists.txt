macro(add_example_project NAME)
    add_executable(${NAME})
    target_link_libraries(${NAME} PRIVATE nox)
    set_target_properties(${NAME} PROPERTIES FOLDER "nox/examples")

    if(NOT BUILD_SHARED_LIBS AND NOX_BUILD_RENDERER_OPENGL)
        target_link_libraries(${NAME} PRIVATE nox-opengl)
    endif()
endmacro()

add_subdirectory(sandbox)
