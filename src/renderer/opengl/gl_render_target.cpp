#include "renderer/format_helper.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_texture.h"

#include <glad/gl.h>

namespace NOX {

void GLRenderTargetBase::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

void GLRenderTargetBase::clear(const Vector4D<float> &color, uint8_t index) const {
    glClearNamedFramebufferfv(m_handle, GL_COLOR, index, color.values.data());
}

void GLRenderTargetBase::clear(const Vector4D<int32_t> &color, uint8_t index) const {
    glClearNamedFramebufferiv(m_handle, GL_COLOR, index, color.values.data());
}

void GLRenderTargetBase::clear(const Vector4D<uint32_t> &color, uint8_t index) const {
    glClearNamedFramebufferuiv(m_handle, GL_COLOR, index, color.values.data());
}

void GLRenderTargetBase::clear(float depth) const {
    glClearNamedFramebufferfv(m_handle, GL_DEPTH, 0, &depth);
}

void GLRenderTargetBase::clear(uint32_t stencil) const {
    glClearNamedFramebufferuiv(m_handle, GL_STENCIL, 0, &stencil);
}

void GLRenderTargetBase::clear(float depth, uint32_t stencil) const {
    glClearNamedFramebufferfi(m_handle, GL_DEPTH_STENCIL, 0, depth, stencil);
}

void GLDefaultRenderTarget::clear(const Vector4D<float> &color, uint8_t index) const {
    NOX_ASSERT(index != 0u);
    GLRenderTargetBase::clear(color, 0u);
}

void GLDefaultRenderTarget::clear(const Vector4D<int32_t> &color, uint8_t index) const {
    NOX_ASSERT(index != 0u);
    GLRenderTargetBase::clear(color, 0u);
}

void GLDefaultRenderTarget::clear(const Vector4D<uint32_t> &color, uint8_t index) const {
    NOX_ASSERT(index != 0u);
    GLRenderTargetBase::clear(color, 0u);
}

GLRenderTarget::GLRenderTarget(const RenderTargetDescriptor &descriptor) : m_size{descriptor.size} {
    const auto &colorAttachments = descriptor.attachments.colorAttachments;
    const auto &depthStencilAttachments = descriptor.attachments.depthStencilAttachments;
    m_colorAttachmentsCount = validateColorAttachments(colorAttachments);
    m_depthStencilAttachmentsCount = validateDepthStencilAttachments(depthStencilAttachments);

    auto attachmentsCount = m_colorAttachmentsCount + m_depthStencilAttachmentsCount;
    NOX_ASSERT_MSG(attachmentsCount == 0u, "Render target with no attachments is not allowed");
    m_attachmentPoints.reserve(attachmentsCount);

    glCreateFramebuffers(1, &m_handle);

    if (m_colorAttachmentsCount > 0u) {
        for (uint32_t i = 0; i < m_colorAttachmentsCount; i++) {
            auto attachmentPoint = GL_COLOR_ATTACHMENT0 + i;
            m_attachmentPoints.push_back(attachmentPoint);
            createColorAttachment(*colorAttachments[i], attachmentPoint);
        }
        glNamedFramebufferDrawBuffers(m_handle, m_colorAttachmentsCount, m_attachmentPoints.data());
    } else {
        glNamedFramebufferDrawBuffer(m_handle, GL_NONE);
    }

    if (m_depthStencilAttachmentsCount > 0u) {
        m_depthStencilAttachments.reserve(m_depthStencilAttachmentsCount);

        for (uint32_t i = 0; i < m_depthStencilAttachmentsCount; i++) {
            createDepthStencilAttachment(depthStencilAttachments[i]);
        }
    }

    auto result = glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER);
    NOX_ASSERT_MSG(result != GL_FRAMEBUFFER_COMPLETE, "Render target is not complete");
}

GLRenderTarget::~GLRenderTarget() {
    auto attachmentsCount = m_colorAttachmentsCount + m_depthStencilAttachmentsCount;
    glInvalidateNamedFramebufferData(m_handle, attachmentsCount, m_attachmentPoints.data());
    glDeleteFramebuffers(1, &m_handle);
}

uint8_t GLRenderTarget::validateColorAttachments(const ColorAttachmentsContainer &attachments) {
    int32_t maxColorAttachments = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

    uint8_t colorAttachmentsCount = 0u;
    for (const auto &attachment : attachments) {
        if (attachment == nullptr) {
            break;
        }
        colorAttachmentsCount++;
    }
    NOX_ASSERT_MSG(colorAttachmentsCount > maxColorAttachments, "Color attachments count for render target exceeded");

    return colorAttachmentsCount;
}

uint8_t GLRenderTarget::validateDepthStencilAttachments(const DepthStencilAttachmentsContainer &attachments) {
    uint8_t depthStencilAttachmentCount = 0u;
    uint8_t depthAttachmentCount = 0u;
    uint8_t stencilAttachmentCount = 0u;
    for (const auto &attachment : attachments) {
        if (attachment == Format::NONE) {
            break;
        }

        auto formatDescriptor = FormatHelper::getFormatDescriptor(attachment);
        if (formatDescriptor.hasDepth && formatDescriptor.hasStencil) {
            depthStencilAttachmentCount++;
        } else if (formatDescriptor.hasDepth) {
            depthAttachmentCount++;
        } else if (formatDescriptor.hasStencil) {
            stencilAttachmentCount++;
        }
    }

    bool isValid = true;
    if (depthStencilAttachmentCount > 1u) {
        isValid = false;
    } else if (depthStencilAttachmentCount == 1u) {
        if ((depthAttachmentCount != 0u) || (stencilAttachmentCount != 0u)) {
            isValid = false;
        }
    } else if (depthStencilAttachmentCount == 0u) {
        if ((depthAttachmentCount > 1u) || (stencilAttachmentCount > 1u)) {
            isValid = false;
        }
    }
    NOX_ASSERT_MSG(!isValid, "Depth and stencil attachments combination is incorrect");

    return (depthStencilAttachmentCount + depthAttachmentCount + stencilAttachmentCount);
}

void GLRenderTarget::createColorAttachment(const Texture &texture, uint32_t attachmentPoint) {
    const auto &glTexture = dynamic_cast<const GLTexture *>(&texture);
    NOX_ASSERT(glTexture == nullptr);

    glNamedFramebufferTexture(m_handle, attachmentPoint, glTexture->getHandle(), 0);
}

void GLRenderTarget::createDepthStencilAttachment(Format format) {
    TextureDescriptor textureDescriptor{};
    textureDescriptor.type = TextureType::TEXTURE2D;
    textureDescriptor.format = format;
    textureDescriptor.size = {m_size.x(), m_size.y(), 0u};
    m_depthStencilAttachments.emplace_back(std::make_unique<GLTexture>(textureDescriptor));

    auto formatDescriptor = FormatHelper::getFormatDescriptor(format);
    uint32_t attachmentPoint = 0u;
    if (formatDescriptor.hasDepth && formatDescriptor.hasStencil) {
        attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;
    } else if (formatDescriptor.hasDepth) {
        attachmentPoint = GL_DEPTH_ATTACHMENT;
    } else if (formatDescriptor.hasStencil) {
        attachmentPoint = GL_STENCIL_ATTACHMENT;
    }
    m_attachmentPoints.push_back(attachmentPoint);

    glNamedFramebufferTexture(m_handle, attachmentPoint, m_depthStencilAttachments.front()->getHandle(), 0);
}

} // namespace NOX
