# --------------------------------------------------
# Create nox target
# --------------------------------------------------
add_library(nox)
add_library(nox::nox ALIAS nox)
target_include_directories(
    nox 
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    PRIVATE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
)

# --------------------------------------------------
# Specify nox compile properties
# --------------------------------------------------
target_compile_features(nox PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(nox PRIVATE /W4 /WX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DNOX_DEBUG")
else()
    target_compile_options(nox PRIVATE -Wall)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNOX_DEBUG")
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(nox PUBLIC NOX_SHARED)
endif()

# --------------------------------------------------
# Specify OS specific definitions
# --------------------------------------------------
if(WIN32)
    target_compile_definitions(nox PUBLIC NOX_WIN32)
endif()

# --------------------------------------------------
# Add public headers to nox sources (for IDE)
# --------------------------------------------------
target_sources(nox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

file(GLOB_RECURSE NOX_PUBLIC_HEADERS ${PROJECT_SOURCE_DIR}/include/nox/*.h)
target_sources(nox PRIVATE ${NOX_PUBLIC_HEADERS})

create_project_source_tree(nox)
