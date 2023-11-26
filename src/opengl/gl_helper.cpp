#include "opengl/gl_helper.h"

#include <nox/config.h>

namespace NOX::GLHelper {

GLenum mapFormatDataTypeToEnum(FormatDataType type, uint8_t size) {
    if (type == FormatDataType::UINT) {
        if (size == 1u) {
            return GL_UNSIGNED_BYTE;
        }
        if (size == 2u) {
            return GL_UNSIGNED_SHORT;
        }
        if (size == 4u) {
            return GL_UNSIGNED_INT;
        }
    }
    if (type == FormatDataType::SINT) {
        if (size == 1u) {
            return GL_BYTE;
        }
        if (size == 2u) {
            return GL_SHORT;
        }
        if (size == 4u) {
            return GL_INT;
        }
    }
    if (type == FormatDataType::FLOAT) {
        if (size == 2u) {
            return GL_HALF_FLOAT;
        }
        if (size == 4u) {
            return GL_FLOAT;
        }
        if (size == 8u) {
            return GL_DOUBLE;
        }
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

bool isExtensionSupported(std::string_view extensionsList, std::string_view extension) {
    return (extensionsList.find(extension) != std::string::npos);
}

} // namespace NOX::GLHelper
