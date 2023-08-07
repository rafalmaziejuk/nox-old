set(NOX_LIB_NAME "nox")
set(NOX_OPENGL_LIB_NAME "nox-opengl")

set(NOX_FOLDER_NAME "nox")
set(NOX_RENDERERS_FOLDER_NAME "${NOX_FOLDER_NAME}/renderers")
set(NOX_THIRD_PARTY_FOLDER_NAME "${NOX_FOLDER_NAME}/third_party")
set(NOX_EXAMPLES_FOLDER_NAME "${NOX_FOLDER_NAME}/examples")

set(NOX_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")

if(BUILD_SHARED_LIBS)
    set(NOX_DEBUG_POSTFIX "-d")
else()
    set(NOX_DEBUG_POSTFIX "-s-d")
    set(NOX_RELEASE_POSTFIX "-s")
endif()
