cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

include(cmake/utilities.cmake)
nox_extract_version()

project(nox VERSION ${NOX_VERSION})

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GenerateExportHeader)
include(GNUInstallDirs)

# --------------------------------------------------
# Set global properties/variables
# --------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# --------------------------------------------------
# Specify options
# --------------------------------------------------
set(NOX_INSTALLATION_TARGETS "" CACHE INTERNAL "")
set(NOX_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(NOX_MAIN_PROJECT ON)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(NOX_INSTALL "Generate installation target." ${NOX_MAIN_PROJECT})
option(NOX_BUILD_EXAMPLES "Build examples." OFF)
option(NOX_BUILD_RENDERER_OPENGL "Build OpenGL renderer." ON)

cmake_dependent_option(NOX_PLATFORM_WINDOWS "Target platform Windows." ON "WIN32" OFF)
cmake_dependent_option(NOX_PLATFORM_LINUX "Target platform Linux." ON "UNIX" OFF)

# --------------------------------------------------
# Specify build types
# --------------------------------------------------
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Release;Debug")
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE not specified, using Release.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "NOX build type: [Release, Debug]" FORCE)
endif()

# --------------------------------------------------
# Add subdirectories
# --------------------------------------------------
add_subdirectory(src)

if(NOX_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# --------------------------------------------------
# Create generated files
# --------------------------------------------------
set(NOX_CONFIG_PATH "${CMAKE_INSTALL_LIBDIR}/cmake/nox")

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/nox-config.cmake.in"
    "${PROJECT_BINARY_DIR}/nox-config.cmake"
    INSTALL_DESTINATION "${NOX_CONFIG_PATH}"
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/nox-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

configure_file(cmake/nox.pc.in nox.pc @ONLY)

# --------------------------------------------------
# Install
# --------------------------------------------------
if(NOX_INSTALL)
    install(
        TARGETS ${NOX_INSTALLATION_TARGETS}
        EXPORT nox-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(
        EXPORT nox-targets
        FILE nox-targets.cmake
        NAMESPACE nox::
        DESTINATION "${NOX_CONFIG_PATH}"
    )

    install(
        DIRECTORY "${PROJECT_SOURCE_DIR}/include/nox"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h"
    )

    install(
        FILES "${PROJECT_BINARY_DIR}/nox-config.cmake"
              "${PROJECT_BINARY_DIR}/nox-config-version.cmake"
        DESTINATION "${NOX_CONFIG_PATH}"
    )

    install(
        FILES "${PROJECT_BINARY_DIR}/nox.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    )
endif()
