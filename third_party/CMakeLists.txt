# --------------------------------------------------
# Add GLFW (examples, tests)
# --------------------------------------------------
if(NOX_BUILD_EXAMPLES OR NOX_BUILD_TESTS)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw glfw)
    set_target_properties(
        glfw
        PROPERTIES 
        FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}/glfw"
    )
    set_target_properties(
        update_mappings
        PROPERTIES
        FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}/glfw"
    )
endif()

# --------------------------------------------------
# Add stb_image (examples)
# --------------------------------------------------
if(NOX_BUILD_EXAMPLES)
    add_subdirectory(stb_image stb_image)
    set_target_properties(
        stb_image
        PROPERTIES 
        FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}"
    )
endif()

# --------------------------------------------------
# Add GLAD (OpenGL renderer)
# --------------------------------------------------
if(NOX_BUILD_RENDERER_OPENGL)
    add_subdirectory(glad glad)
    set_target_properties(
        glad
        PROPERTIES 
        FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}"
    )
endif()
