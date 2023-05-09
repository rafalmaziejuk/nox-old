#include "renderer/opengl/gl_helper.h"

#define GL_ENUM_TO_CASE(enumName) \
    case enumName:                \
        return #enumName

namespace NOX::GLHelper {

namespace {

const char *glDebugEnumToString(GLenum value) {
    switch (value) {
        GL_ENUM_TO_CASE(GL_DEBUG_SOURCE_API);
        GL_ENUM_TO_CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
        GL_ENUM_TO_CASE(GL_DEBUG_SOURCE_SHADER_COMPILER);
        GL_ENUM_TO_CASE(GL_DEBUG_SOURCE_THIRD_PARTY);
        GL_ENUM_TO_CASE(GL_DEBUG_SOURCE_APPLICATION);
        GL_ENUM_TO_CASE(GL_DEBUG_SOURCE_OTHER);

        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_ERROR);
        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_PORTABILITY);
        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_PERFORMANCE);
        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_MARKER);
        GL_ENUM_TO_CASE(GL_DEBUG_TYPE_OTHER);

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

GLbitfield mapBufferStorageFlags(uint8_t accessMethod) {
    GLbitfield flags = 0u;

    if (accessMethod & BufferAccessMethod::STATIC) {
        flags |= 0u;
    }
    if (accessMethod & BufferAccessMethod::DYNAMIC) {
        flags |= GL_DYNAMIC_STORAGE_BIT;
    }

    return flags;
}

GLenum mapDataTypeEnum(uint8_t type, uint8_t size) {
    if (size == 1u) {
        if (type == FormatDataType::UINT) {
            return GL_UNSIGNED_BYTE;
        }
        if (type == FormatDataType::SINT) {
            return GL_BYTE;
        }
    }
    if (size == 2u) {
        if (type == FormatDataType::UINT) {
            return GL_UNSIGNED_SHORT;
        }
        if (type == FormatDataType::SINT) {
            return GL_SHORT;
        }
        if (type == FormatDataType::FLOAT) {
            return GL_HALF_FLOAT;
        }
    }
    if (size == 4u) {
        if (type == FormatDataType::UINT) {
            return GL_UNSIGNED_INT;
        }
        if (type == FormatDataType::SINT) {
            return GL_INT;
        }
        if (type == FormatDataType::FLOAT) {
            return GL_FLOAT;
        }
    }
    if (size == 8u) {
        if (type == FormatDataType::FLOAT) {
            return GL_DOUBLE;
        }
    }

    NOX_ASSERT(true);
    return 0u;
}

} // namespace NOX::GLHelper
