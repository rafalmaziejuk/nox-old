# --------------------------------------------------
# Check if clang-tidy executable exists
# --------------------------------------------------
find_program(CLANG_TIDY_EXECUTABLE clang-tidy)
if (NOT CLANG_TIDY_EXECUTABLE)
    message(FATAL_ERROR "Unable to find clang-tidy executable.")
endif()

# --------------------------------------------------
# Find Python and run-clang-tidy script
# --------------------------------------------------
find_package(Python 3 REQUIRED)

find_program(RUN_CLANG_TIDY run-clang-tidy)
if (NOT RUN_CLANG_TIDY)
    message(FATAL_ERROR "Failed to find run-clang-tidy script.")
endif()

# --------------------------------------------------
# Run clang-tidy
# --------------------------------------------------
execute_process(COMMAND ${Python_EXECUTABLE} ${RUN_CLANG_TIDY} -clang-tidy-binary ${CLANG_TIDY_EXECUTABLE} -p ${PROJECT_BINARY_DIR} RESULTS_VARIABLE EXIT_CODE)
if (NOT EXIT_CODE STREQUAL 0)
    message(FATAL_ERROR "clang-tidy code analysis failed.")
endif()
