#include "core/assertion.h"

#include <glad/gl.h>

namespace NOX {

namespace {

const char *glDebugEnumToString(GLenum value) {
    switch (value) {
        NOX_CASE_RETURN_STRING(GL_DEBUG_SOURCE_API, GL_DEBUG_SOURCE_API);
        NOX_CASE_RETURN_STRING(GL_DEBUG_SOURCE_WINDOW_SYSTEM, GL_DEBUG_SOURCE_WINDOW_SYSTEM);
        NOX_CASE_RETURN_STRING(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_SOURCE_SHADER_COMPILER);
        NOX_CASE_RETURN_STRING(GL_DEBUG_SOURCE_THIRD_PARTY, GL_DEBUG_SOURCE_THIRD_PARTY);
        NOX_CASE_RETURN_STRING(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_SOURCE_APPLICATION);
        NOX_CASE_RETURN_STRING(GL_DEBUG_SOURCE_OTHER, GL_DEBUG_SOURCE_OTHER);

        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_ERROR, GL_DEBUG_TYPE_ERROR);
        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_PORTABILITY, GL_DEBUG_TYPE_PORTABILITY);
        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_PERFORMANCE, GL_DEBUG_TYPE_PERFORMANCE);
        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_MARKER, GL_DEBUG_TYPE_MARKER);
        NOX_CASE_RETURN_STRING(GL_DEBUG_TYPE_OTHER, GL_DEBUG_TYPE_OTHER);

    default:
        NOX_ASSERT(true);
        return "UNDEFINED";
    }
}

} // namespace

void GLAPIENTRY debugMessageCallback(GLenum source,
                                     GLenum type,
                                     GLuint /*id*/,
                                     GLenum severity,
                                     GLsizei /*length*/,
                                     const GLchar *message,
                                     const void * /*userParam*/) {
    const auto *typeLabel = glDebugEnumToString(type);
    const auto *sourceLabel = glDebugEnumToString(source);

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        NOX_LOG_TRACE(OPENGL, "[{}][{}] {}", typeLabel, sourceLabel, message);
    } else if (severity == GL_DEBUG_SEVERITY_LOW) {
        NOX_LOG_WARN(OPENGL, "[{}][{}] {}", typeLabel, sourceLabel, message);
    } else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
        NOX_LOG_CRITICAL(OPENGL, "[{}][{}] {}", typeLabel, sourceLabel, message);
    } else if (severity == GL_DEBUG_SEVERITY_HIGH) {
        NOX_LOG_ERROR(OPENGL, "[{}][{}] {}", typeLabel, sourceLabel, message);
        NOX_ASSERT(true);
    }
}

} // namespace NOX
