# --------------------------------------------------
# Create stb_image target
# --------------------------------------------------
add_library(stb_image STATIC)
target_include_directories(
    stb_image
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_EXAMPLES_STB_IMAGE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.c
)

target_sources(stb_image PRIVATE ${NOX_EXAMPLES_STB_IMAGE_SOURCES})
