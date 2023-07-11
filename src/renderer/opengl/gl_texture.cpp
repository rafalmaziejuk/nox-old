#include "renderer/opengl/gl_texture.h"

#include <glad/gl.h>

namespace NOX {

namespace {

GLenum mapTextureTarget(TextureType type) {
    switch (type) {
        NOX_CASE_RETURN_VALUE(TextureType::TEXTURE2D, GL_TEXTURE_2D);

    default:
        NOX_ASSERT(true);
        return 0u;
    }
}

GLenum mapTextureFormat(Format format) {
    switch (format) {
        NOX_CASE_RETURN_VALUE(Format::R8UI, GL_R8UI);
        NOX_CASE_RETURN_VALUE(Format::R16UI, GL_R16UI);
        NOX_CASE_RETURN_VALUE(Format::R32UI, GL_R32UI);
        NOX_CASE_RETURN_VALUE(Format::RG8UI, GL_RG8UI);
        NOX_CASE_RETURN_VALUE(Format::RG16UI, GL_RG16UI);
        NOX_CASE_RETURN_VALUE(Format::RG32UI, GL_RG32UI);
        NOX_CASE_RETURN_VALUE(Format::RGB8UI, GL_RGB8UI);
        NOX_CASE_RETURN_VALUE(Format::RGB16UI, GL_RGB16UI);
        NOX_CASE_RETURN_VALUE(Format::RGB32UI, GL_RGB32UI);
        NOX_CASE_RETURN_VALUE(Format::RGBA8UI, GL_RGBA8UI);
        NOX_CASE_RETURN_VALUE(Format::RGBA16UI, GL_RGBA16UI);
        NOX_CASE_RETURN_VALUE(Format::RGBA32UI, GL_RGBA32UI);
        NOX_CASE_RETURN_VALUE(Format::R8I, GL_R8I);
        NOX_CASE_RETURN_VALUE(Format::R16I, GL_R16I);
        NOX_CASE_RETURN_VALUE(Format::R32I, GL_R32I);
        NOX_CASE_RETURN_VALUE(Format::RG8I, GL_RG8I);
        NOX_CASE_RETURN_VALUE(Format::RG16I, GL_RG16I);
        NOX_CASE_RETURN_VALUE(Format::RG32I, GL_RG32I);
        NOX_CASE_RETURN_VALUE(Format::RGB8I, GL_RGB8I);
        NOX_CASE_RETURN_VALUE(Format::RGB16I, GL_RGB16I);
        NOX_CASE_RETURN_VALUE(Format::RGB32I, GL_RGB32I);
        NOX_CASE_RETURN_VALUE(Format::RGBA8I, GL_RGBA8I);
        NOX_CASE_RETURN_VALUE(Format::RGBA16I, GL_RGBA16I);
        NOX_CASE_RETURN_VALUE(Format::RGBA32I, GL_RGBA32I);
        NOX_CASE_RETURN_VALUE(Format::R8_UNORM, GL_R8);
        NOX_CASE_RETURN_VALUE(Format::R16_UNORM, GL_R16);
        NOX_CASE_RETURN_VALUE(Format::RG8_UNORM, GL_RG8);
        NOX_CASE_RETURN_VALUE(Format::RG16_UNORM, GL_RG16);
        NOX_CASE_RETURN_VALUE(Format::RGB8_UNORM, GL_RGB8);
        NOX_CASE_RETURN_VALUE(Format::RGB16_UNORM, GL_RGB16);
        NOX_CASE_RETURN_VALUE(Format::RGBA8_UNORM, GL_RGBA8);
        NOX_CASE_RETURN_VALUE(Format::RGBA16_UNORM, GL_RGBA16);
        NOX_CASE_RETURN_VALUE(Format::R8_SNORM, GL_R8_SNORM);
        NOX_CASE_RETURN_VALUE(Format::R16_SNORM, GL_R16_SNORM);
        NOX_CASE_RETURN_VALUE(Format::RG8_SNORM, GL_RG8_SNORM);
        NOX_CASE_RETURN_VALUE(Format::RG16_SNORM, GL_RG16_SNORM);
        NOX_CASE_RETURN_VALUE(Format::RGB8_SNORM, GL_RGB8_SNORM);
        NOX_CASE_RETURN_VALUE(Format::RGB16_SNORM, GL_RGB16_SNORM);
        NOX_CASE_RETURN_VALUE(Format::RGBA8_SNORM, GL_RGBA8_SNORM);
        NOX_CASE_RETURN_VALUE(Format::RGBA16_SNORM, GL_RGBA16_SNORM);
        NOX_CASE_RETURN_VALUE(Format::R16F, GL_R16F);
        NOX_CASE_RETURN_VALUE(Format::R32F, GL_R32F);
        NOX_CASE_RETURN_VALUE(Format::RG16F, GL_RG16F);
        NOX_CASE_RETURN_VALUE(Format::RG32F, GL_RG32F);
        NOX_CASE_RETURN_VALUE(Format::RGB16F, GL_RGB32F);
        NOX_CASE_RETURN_VALUE(Format::RGB64F, GL_RGBA16F);
        NOX_CASE_RETURN_VALUE(Format::RGBA16F, GL_RGBA16F);
        NOX_CASE_RETURN_VALUE(Format::RGBA32F, GL_RGBA32F);
        NOX_CASE_RETURN_VALUE(Format::DEPTH16I, GL_DEPTH_COMPONENT16);
        NOX_CASE_RETURN_VALUE(Format::DEPTH24I, GL_DEPTH_COMPONENT24);
        NOX_CASE_RETURN_VALUE(Format::DEPTH32I, GL_DEPTH_COMPONENT32);
        NOX_CASE_RETURN_VALUE(Format::DEPTH32F, GL_DEPTH_COMPONENT32F);
        NOX_CASE_RETURN_VALUE(Format::STENCIL8UI, GL_STENCIL_INDEX8);
        NOX_CASE_RETURN_VALUE(Format::DEPTH24I_STENCIL8UI, GL_DEPTH24_STENCIL8);
        NOX_CASE_RETURN_VALUE(Format::DEPTH32F_STENCIL8UI, GL_DEPTH32F_STENCIL8);

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
