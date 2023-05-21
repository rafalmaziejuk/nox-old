#include "renderer/opengl/gl_helper.h"

#include <nox/renderer/command_list_types.h>

#define GL_DEBUG_ENUM_TO_CASE(enumName) \
    case enumName:                      \
        return #enumName

namespace NOX::GLHelper {

namespace {

const char *glDebugEnumToString(GLenum value) {
    switch (value) {
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_SOURCE_API);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_SOURCE_SHADER_COMPILER);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_SOURCE_THIRD_PARTY);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_SOURCE_APPLICATION);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_SOURCE_OTHER);

        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_ERROR);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_PORTABILITY);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_PERFORMANCE);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_MARKER);
        GL_DEBUG_ENUM_TO_CASE(GL_DEBUG_TYPE_OTHER);

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

GLenum mapDataTypeEnum(FormatDataType type, uint8_t size) {
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

GLenum mapShaderEnum(ShaderType type) {
    switch (type) {
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER;
    case ShaderType::FRAGMENT:
        return GL_FRAGMENT_SHADER;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

GLbitfield mapShaderBit(ShaderType type) {
    switch (type) {
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER_BIT;
    case ShaderType::FRAGMENT:
        return GL_FRAGMENT_SHADER_BIT;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

GLenum mapPrimitiveTopologyEnum(PrimitiveTopology topology) {
    switch (topology) {
    case PrimitiveTopology::TRIANGLE_LIST:
        return GL_TRIANGLES;
    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

GLbitfield mapClearFlagBits(uint8_t clearFlag) {
    GLbitfield flags = 0u;

    if (clearFlag & ClearFlag::COLOR) {
        flags |= GL_COLOR_BUFFER_BIT;
    }
    if (clearFlag & ClearFlag::DEPTH) {
        flags |= GL_DEPTH_BUFFER_BIT;
    }
    if (clearFlag & ClearFlag::STENCIL) {
        flags |= GL_STENCIL_BUFFER_BIT;
    }

    return flags;
}

} // namespace NOX::GLHelper
