# --------------------------------------------------
# Check if clang-format executable exists
# --------------------------------------------------
find_program(CLANG_FORMAT_EXECUTABLE clang-format)
if (NOT CLANG_FORMAT_EXECUTABLE)
    message(FATAL_ERROR "Unable to find clang-format executable.")
endif()

# --------------------------------------------------
# Run clang-format
# --------------------------------------------------
set(SOURCES "")
foreach (DIRECTORY IN ITEMS include src examples)
    file(GLOB_RECURSE FILES "${DIRECTORY}/*.h" "${DIRECTORY}/*.cpp" "${DIRECTORY}/*.inl")
    list(APPEND SOURCES ${FILES})
endforeach()

list(FILTER SOURCES EXCLUDE REGEX "third_party")

execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${SOURCES})
