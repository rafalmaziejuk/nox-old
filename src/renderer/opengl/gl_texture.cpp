#include "renderer/opengl/gl_texture.h"

#include <glad/gl.h>

namespace NOX {

namespace {

#define VALUE_TO_GL_ENUM_CASE(value, enumName) \
    case value:                                \
        return enumName

GLenum mapTextureTarget(TextureType type) {
    switch (type) {
        VALUE_TO_GL_ENUM_CASE(TextureType::TEXTURE2D, GL_TEXTURE_2D);

    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

GLenum mapTextureFormat(Format format) {
    switch (format) {
        VALUE_TO_GL_ENUM_CASE(Format::R8UI, GL_R8UI);
        VALUE_TO_GL_ENUM_CASE(Format::R16UI, GL_R16UI);
        VALUE_TO_GL_ENUM_CASE(Format::R32UI, GL_R32UI);
        VALUE_TO_GL_ENUM_CASE(Format::RG8UI, GL_RG8UI);
        VALUE_TO_GL_ENUM_CASE(Format::RG16UI, GL_RG16UI);
        VALUE_TO_GL_ENUM_CASE(Format::RG32UI, GL_RG32UI);
        VALUE_TO_GL_ENUM_CASE(Format::RGB8UI, GL_RGB8UI);
        VALUE_TO_GL_ENUM_CASE(Format::RGB16UI, GL_RGB16UI);
        VALUE_TO_GL_ENUM_CASE(Format::RGB32UI, GL_RGB32UI);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA8UI, GL_RGBA8UI);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA16UI, GL_RGBA16UI);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA32UI, GL_RGBA32UI);
        VALUE_TO_GL_ENUM_CASE(Format::R8I, GL_R8I);
        VALUE_TO_GL_ENUM_CASE(Format::R16I, GL_R16I);
        VALUE_TO_GL_ENUM_CASE(Format::R32I, GL_R32I);
        VALUE_TO_GL_ENUM_CASE(Format::RG8I, GL_RG8I);
        VALUE_TO_GL_ENUM_CASE(Format::RG16I, GL_RG16I);
        VALUE_TO_GL_ENUM_CASE(Format::RG32I, GL_RG32I);
        VALUE_TO_GL_ENUM_CASE(Format::RGB8I, GL_RGB8I);
        VALUE_TO_GL_ENUM_CASE(Format::RGB16I, GL_RGB16I);
        VALUE_TO_GL_ENUM_CASE(Format::RGB32I, GL_RGB32I);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA8I, GL_RGBA8I);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA16I, GL_RGBA16I);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA32I, GL_RGBA32I);
        VALUE_TO_GL_ENUM_CASE(Format::R8_UNORM, GL_R8);
        VALUE_TO_GL_ENUM_CASE(Format::R16_UNORM, GL_R16);
        VALUE_TO_GL_ENUM_CASE(Format::RG8_UNORM, GL_RG8);
        VALUE_TO_GL_ENUM_CASE(Format::RG16_UNORM, GL_RG16);
        VALUE_TO_GL_ENUM_CASE(Format::RGB8_UNORM, GL_RGB8);
        VALUE_TO_GL_ENUM_CASE(Format::RGB16_UNORM, GL_RGB16);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA8_UNORM, GL_RGBA8);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA16_UNORM, GL_RGBA16);
        VALUE_TO_GL_ENUM_CASE(Format::R8_SNORM, GL_R8_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::R16_SNORM, GL_R16_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::RG8_SNORM, GL_RG8_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::RG16_SNORM, GL_RG16_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::RGB8_SNORM, GL_RGB8_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::RGB16_SNORM, GL_RGB16_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA8_SNORM, GL_RGBA8_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA16_SNORM, GL_RGBA16_SNORM);
        VALUE_TO_GL_ENUM_CASE(Format::R16F, GL_R16F);
        VALUE_TO_GL_ENUM_CASE(Format::R32F, GL_R32F);
        VALUE_TO_GL_ENUM_CASE(Format::RG16F, GL_RG16F);
        VALUE_TO_GL_ENUM_CASE(Format::RG32F, GL_RG32F);
        VALUE_TO_GL_ENUM_CASE(Format::RGB16F, GL_RGB32F);
        VALUE_TO_GL_ENUM_CASE(Format::RGB64F, GL_RGBA16F);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA16F, GL_RGBA16F);
        VALUE_TO_GL_ENUM_CASE(Format::RGBA32F, GL_RGBA32F);
        VALUE_TO_GL_ENUM_CASE(Format::DEPTH16I, GL_DEPTH_COMPONENT16);
        VALUE_TO_GL_ENUM_CASE(Format::DEPTH24I, GL_DEPTH_COMPONENT24);
        VALUE_TO_GL_ENUM_CASE(Format::DEPTH32I, GL_DEPTH_COMPONENT32);
        VALUE_TO_GL_ENUM_CASE(Format::DEPTH32F, GL_DEPTH_COMPONENT32F);
        VALUE_TO_GL_ENUM_CASE(Format::STENCIL8UI, GL_STENCIL_INDEX8);
        VALUE_TO_GL_ENUM_CASE(Format::DEPTH24I_STENCIL8UI, GL_DEPTH24_STENCIL8);
        VALUE_TO_GL_ENUM_CASE(Format::DEPTH32F_STENCIL8UI, GL_DEPTH32F_STENCIL8);

    default:
        NOX_ASSERT(true);
        return 0u;
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

    default:
        NOX_ASSERT(true);
        break;
    }

    glBindTextureUnit(0u, m_handle);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &m_handle);
}

void GLTexture::createTexture2D() {
    auto width = m_descriptor.size.x();
    auto height = m_descriptor.size.y();
    auto format = mapTextureFormat(m_descriptor.format);
    glTextureStorage2D(m_handle, 1, format, width, height);
}

} // namespace NOX
