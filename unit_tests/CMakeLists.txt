# --------------------------------------------------
# Setup googletest
# --------------------------------------------------
include(GoogleTest)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/b796f7d.zip
)
FetchContent_MakeAvailable(googletest)
    
set_target_properties(gtest PROPERTIES FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}/gtest")
set_target_properties(gtest_main PROPERTIES FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}/gtest")

# --------------------------------------------------
# Create unit tests executable
# --------------------------------------------------
add_executable(${NOX_UNIT_TESTS_EXE_NAME})
target_include_directories(
    ${NOX_UNIT_TESTS_EXE_NAME}
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
)

target_compile_features(${NOX_UNIT_TESTS_EXE_NAME} PUBLIC cxx_std_17)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY)
endif()

gtest_discover_tests(${NOX_UNIT_TESTS_EXE_NAME})
enable_testing()

# --------------------------------------------------
# Set properties
# --------------------------------------------------
set_target_properties(${NOX_UNIT_TESTS_EXE_NAME} PROPERTIES FOLDER ${NOX_FOLDER_NAME})

# --------------------------------------------------
# Add dependencies
# --------------------------------------------------
if(NOX_BUILD_RENDERER_OPENGL)
    add_dependencies(${NOX_UNIT_TESTS_EXE_NAME} ${NOX_OPENGL_LIB_NAME})
endif()

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
    ${NOX_UNIT_TESTS_EXE_NAME}
    PRIVATE
    ${NOX_LIB_NAME}
    GTest::gtest_main
)

# --------------------------------------------------
# Add sources
# --------------------------------------------------
set(NOX_UNIT_TESTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_tests.cpp
)

target_sources(${NOX_UNIT_TESTS_EXE_NAME} PRIVATE ${NOX_UNIT_TESTS_SOURCES})

nox_create_project_source_tree(${NOX_UNIT_TESTS_EXE_NAME})
