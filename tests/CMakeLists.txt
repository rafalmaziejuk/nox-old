# --------------------------------------------------
# Set auxiliary variables
# --------------------------------------------------
set(NOX_TESTS_BASE_LIB_NAME "tests-base")
set(NOX_UNIT_TESTS_EXE_NAME "unit-tests")
set(NOX_TESTS_FOLDER_NAME "${NOX_FOLDER_NAME}/tests")

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
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest PROPERTIES FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}/gtest")
set_target_properties(gtest_main PROPERTIES FOLDER "${NOX_THIRD_PARTY_FOLDER_NAME}/gtest")

enable_testing()

get_filename_component(NOX_TESTS_DATA_DIR "${PROJECT_SOURCE_DIR}/tests/data" ABSOLUTE)
configure_file(config.h.in "${CMAKE_CURRENT_SOURCE_DIR}/config.h")

# --------------------------------------------------
# Add subdirectories
# --------------------------------------------------
add_subdirectory(base)
add_subdirectory(unit_tests)
