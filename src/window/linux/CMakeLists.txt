# --------------------------------------------------
# Find required dependencies
# --------------------------------------------------
find_package(X11 REQUIRED)
if(NOT X11_FOUND)
    message(FATAL_ERROR "X11 not found")
endif()

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
    ${NOX_LIB_NAME}
    PRIVATE
    X11::X11
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_WINDOW_LINUX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/linux_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/linux_window.h
)

target_sources(${NOX_LIB_NAME} PRIVATE ${NOX_WINDOW_LINUX_SOURCES})
