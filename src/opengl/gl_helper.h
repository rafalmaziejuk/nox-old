#pragma once

#include "asserts.h"
#include "format_descriptor.h"

#include <glad/gl.h>

namespace nox {

inline GLenum mapVertexAttributeDataTypeToEnum(VertexAttributeDataType type) {
    switch (type) {
    case VertexAttributeDataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
    case VertexAttributeDataType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
    case VertexAttributeDataType::UNSIGNED_INT: return GL_UNSIGNED_INT;
    case VertexAttributeDataType::BYTE: return GL_BYTE;
    case VertexAttributeDataType::SHORT: return GL_SHORT;
    case VertexAttributeDataType::INT: return GL_INT;
    case VertexAttributeDataType::HALF_FLOAT: return GL_HALF_FLOAT;
    case VertexAttributeDataType::FLOAT: return GL_FLOAT;
    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

} // namespace nox
