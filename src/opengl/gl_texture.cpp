#include "nox_assert.h"
#include "opengl/gl_texture.h"
#include "opengl/gl_texture_visitor.h"

#include <glad/gl.h>

namespace nox {

namespace {

GLenum mapTextureTarget(TextureType type) {
    switch (type) {
    case TextureType::TEXTURE2D: return GL_TEXTURE_2D;

    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

GLenum mapTextureFormat(ImageFormat format) {
    switch (format) {
    case ImageFormat::R8: return GL_R8;
    case ImageFormat::R16: return GL_R16;
    case ImageFormat::RG8: return GL_RG8;
    case ImageFormat::RG16: return GL_RG16;
    case ImageFormat::RGB8: return GL_RGB8;
    case ImageFormat::RGB16: return GL_RGB16;
    case ImageFormat::RGBA8: return GL_RGBA8;
    case ImageFormat::RGBA16: return GL_RGBA16;
    case ImageFormat::R8UI: return GL_R8UI;
    case ImageFormat::R16UI: return GL_R16UI;
    case ImageFormat::R32UI: return GL_R32UI;
    case ImageFormat::RG8UI: return GL_RG8UI;
    case ImageFormat::RG16UI: return GL_RG16UI;
    case ImageFormat::RG32UI: return GL_RG32UI;
    case ImageFormat::RGB8UI: return GL_RGB8UI;
    case ImageFormat::RGB16UI: return GL_RGB16UI;
    case ImageFormat::RGB32UI: return GL_RGB32UI;
    case ImageFormat::RGBA8UI: return GL_RGBA8UI;
    case ImageFormat::RGBA16UI: return GL_RGBA16UI;
    case ImageFormat::RGBA32UI: return GL_RGBA32UI;
    case ImageFormat::R8I: return GL_R8I;
    case ImageFormat::R16I: return GL_R16I;
    case ImageFormat::R32I: return GL_R32I;
    case ImageFormat::RG8I: return GL_RG8I;
    case ImageFormat::RG16I: return GL_RG16I;
    case ImageFormat::RG32I: return GL_RG32I;
    case ImageFormat::RGB8I: return GL_RGB8I;
    case ImageFormat::RGB16I: return GL_RGB16I;
    case ImageFormat::RGB32I: return GL_RGB32I;
    case ImageFormat::RGBA8I: return GL_RGBA8I;
    case ImageFormat::RGBA16I: return GL_RGBA16I;
    case ImageFormat::RGBA32I: return GL_RGBA32I;
    case ImageFormat::R8_SNORM: return GL_R8_SNORM;
    case ImageFormat::R16_SNORM: return GL_R16_SNORM;
    case ImageFormat::RG8_SNORM: return GL_RG8_SNORM;
    case ImageFormat::RG16_SNORM: return GL_RG16_SNORM;
    case ImageFormat::RGB8_SNORM: return GL_RGB8_SNORM;
    case ImageFormat::RGB16_SNORM: return GL_RGB16_SNORM;
    case ImageFormat::RGBA8_SNORM: return GL_RGBA8_SNORM;
    case ImageFormat::RGBA16_SNORM: return GL_RGBA16_SNORM;
    case ImageFormat::R16F: return GL_R16F;
    case ImageFormat::R32F: return GL_R32F;
    case ImageFormat::RG16F: return GL_RG16F;
    case ImageFormat::RG32F: return GL_RG32F;
    case ImageFormat::RGB16F: return GL_RGB16F;
    case ImageFormat::RGB32F: return GL_RGB32F;
    case ImageFormat::RGBA16F: return GL_RGBA16F;
    case ImageFormat::RGBA32F: return GL_RGBA32F;
    case ImageFormat::DEPTH16: return GL_DEPTH_COMPONENT16;
    case ImageFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
    case ImageFormat::DEPTH32: return GL_DEPTH_COMPONENT32;
    case ImageFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
    case ImageFormat::STENCIL8: return GL_STENCIL_INDEX8;
    case ImageFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
    case ImageFormat::DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;

    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

} // namespace

GLTexture::GLTexture(TextureType type) {
    auto target = mapTextureTarget(type);
    glCreateTextures(target, 1, &m_handle);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &m_handle);
}

void GLTexture::bind(uint32_t index) const {
    glBindTextureUnit(index, m_handle);
}

void GLTexture::accept(TextureVisitor &visitor) const {
    visitor.visit(*this);
}

bool GLTexture2D::validateInput(const Texture2DDescriptor &descriptor) {
    return (mapTextureFormat(descriptor.format) != GL_NONE);
}

GLTexture2D::GLTexture2D(const Texture2DDescriptor &descriptor) : GLTexture{TextureType::TEXTURE2D} {
    auto width = static_cast<GLsizei>(descriptor.size.x());
    auto height = static_cast<GLsizei>(descriptor.size.y());
    auto format = mapTextureFormat(descriptor.format);
    glTextureStorage2D(m_handle, 1, format, width, height);
}

TextureType GLTexture2D::getTextureType() const {
    return TextureType::TEXTURE2D;
}

} // namespace nox
