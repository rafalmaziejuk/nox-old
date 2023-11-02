#include "renderer/opengl/gl_texture.h"

#include <glad/gl.h>

namespace NOX {

namespace {

GLenum mapTextureTarget(TextureType type) {
    switch (type) {
    case TextureType::TEXTURE2D: return GL_TEXTURE_2D;

    default: return GL_NONE;
    }
}

GLenum mapTextureFormat(Format format) {
    switch (format) {
    case Format::R8UI: return GL_R8UI;
    case Format::R16UI: return GL_R16UI;
    case Format::R32UI: return GL_R32UI;
    case Format::RG8UI: return GL_RG8UI;
    case Format::RG16UI: return GL_RG16UI;
    case Format::RG32UI: return GL_RG32UI;
    case Format::RGB8UI: return GL_RGB8UI;
    case Format::RGB16UI: return GL_RGB16UI;
    case Format::RGB32UI: return GL_RGB32UI;
    case Format::RGBA8UI: return GL_RGBA8UI;
    case Format::RGBA16UI: return GL_RGBA16UI;
    case Format::RGBA32UI: return GL_RGBA32UI;
    case Format::R8I: return GL_R8I;
    case Format::R16I: return GL_R16I;
    case Format::R32I: return GL_R32I;
    case Format::RG8I: return GL_RG8I;
    case Format::RG16I: return GL_RG16I;
    case Format::RG32I: return GL_RG32I;
    case Format::RGB8I: return GL_RGB8I;
    case Format::RGB16I: return GL_RGB16I;
    case Format::RGB32I: return GL_RGB32I;
    case Format::RGBA8I: return GL_RGBA8I;
    case Format::RGBA16I: return GL_RGBA16I;
    case Format::RGBA32I: return GL_RGBA32I;
    case Format::R8_UNORM: return GL_R8;
    case Format::R16_UNORM: return GL_R16;
    case Format::RG8_UNORM: return GL_RG8;
    case Format::RG16_UNORM: return GL_RG16;
    case Format::RGB8_UNORM: return GL_RGB8;
    case Format::RGB16_UNORM: return GL_RGB16;
    case Format::RGBA8_UNORM: return GL_RGBA8;
    case Format::RGBA16_UNORM: return GL_RGBA16;
    case Format::R8_SNORM: return GL_R8_SNORM;
    case Format::R16_SNORM: return GL_R16_SNORM;
    case Format::RG8_SNORM: return GL_RG8_SNORM;
    case Format::RG16_SNORM: return GL_RG16_SNORM;
    case Format::RGB8_SNORM: return GL_RGB8_SNORM;
    case Format::RGB16_SNORM: return GL_RGB16_SNORM;
    case Format::RGBA8_SNORM: return GL_RGBA8_SNORM;
    case Format::RGBA16_SNORM: return GL_RGBA16_SNORM;
    case Format::R16F: return GL_R16F;
    case Format::R32F: return GL_R32F;
    case Format::RG16F: return GL_RG16F;
    case Format::RG32F: return GL_RG32F;
    case Format::RGB16F: return GL_RGB32F;
    case Format::RGB64F: return GL_RGBA16F;
    case Format::RGBA16F: return GL_RGBA16F;
    case Format::RGBA32F: return GL_RGBA32F;
    case Format::DEPTH16I: return GL_DEPTH_COMPONENT16;
    case Format::DEPTH24I: return GL_DEPTH_COMPONENT24;
    case Format::DEPTH32I: return GL_DEPTH_COMPONENT32;
    case Format::DEPTH32F: return GL_DEPTH_COMPONENT32F;
    case Format::STENCIL8UI: return GL_STENCIL_INDEX8;
    case Format::DEPTH24I_STENCIL8UI: return GL_DEPTH24_STENCIL8;
    case Format::DEPTH32F_STENCIL8UI: return GL_DEPTH32F_STENCIL8;

    default: return GL_NONE;
    }
}

} // namespace

GLTexture::GLTexture(const TextureDescriptor &descriptor) : m_descriptor{descriptor} {
    auto target = mapTextureTarget(descriptor.type);
    glCreateTextures(target, 1, &m_handle);

    switch (descriptor.type) {
    case TextureType::TEXTURE2D:
        createTexture2D();
        break;

    default: break;
    }
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &m_handle);
}

void GLTexture::bind(uint32_t index) const {
    glBindTextureUnit(index, m_handle);
}

void GLTexture::createTexture2D() const {
    auto width = m_descriptor.size.x();
    auto height = m_descriptor.size.y();
    auto format = mapTextureFormat(m_descriptor.format);
    glTextureStorage2D(m_handle, 1, format, width, height);
}

} // namespace NOX
