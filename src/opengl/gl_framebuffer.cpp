#include "format_description.h"
#include "nox_assert.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_render_pass.h"
#include "opengl/gl_texture.h"

#include <glad/gl.h>

namespace nox {

bool GLFramebuffer::validateInput(const FramebufferDescription &description) {
    bool result = true;

    auto validateAttachments = [&description]() {
        if (description.renderPass == nullptr) {
            return false;
        }

        const auto *glRenderPass = static_cast<const GLRenderPass *>(description.renderPass);
        const auto &attachmentsDescriptions = glRenderPass->getAttachmentsDescriptions();

        for (size_t i = 0u; i < description.attachments.size(); i++) {
            const auto &attachment = *description.attachments[i];
            const auto &attachmentDescription = attachmentsDescriptions[i];

            if (attachment.getFormat() != attachmentDescription.format) {
                return false;
            }
        }

        return true;
    };

    result &= validateAttachments();
    result &= (description.size.x() > 0u);
    result &= (description.size.y() > 0u);

    return result;
}

bool GLFramebuffer::isDefaultFramebuffer(const AttachmentsContainer &attachments) {
    if (attachments.size() > 1u) {
        return false;
    }

    const auto *glTexture = dynamic_cast<const GLTexture2D *>(attachments.back());
    if (glTexture == nullptr) {
        return false;
    }

    return glTexture->isPresentable();
}

GLFramebuffer::~GLFramebuffer() {
    auto attachmentsCount = static_cast<GLsizei>(m_attachmentsPoints.size());
    glInvalidateNamedFramebufferData(m_handle, attachmentsCount, m_attachmentsPoints.data());
    glDeleteFramebuffers(1, &m_handle);
}

bool GLFramebuffer::validate() const {
    return (glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void GLFramebuffer::clearAttachments(const ClearValues &values, const RenderPass *renderPass) const {
    const auto *glRenderPass = static_cast<const GLRenderPass *>(renderPass);
    const auto &attachmentsDescriptions = glRenderPass->getAttachmentsDescriptions();

    NOX_ASSERT(attachmentsDescriptions.size() == values.size());

    for (size_t i = 0u; i < attachmentsDescriptions.size(); i++) {
        const auto &attachmentDescription = attachmentsDescriptions[i];
        const auto attachmentFormatDescription = getImageFormatDescription(attachmentDescription.format);
        const auto *clearColorValue = std::get_if<ClearColorValue>(&values[i]);
        const auto *clearDepthStencilValue = std::get_if<ClearDepthStencilValue>(&values[i]);

        if (attachmentDescription.loadOp == AttachmentLoadOp::CLEAR) {
            if (attachmentFormatDescription.isColor) {
                NOX_ASSERT(clearColorValue != nullptr);
                clearColorAttachment(*clearColorValue, static_cast<int32_t>(i));
            }

            if (attachmentFormatDescription.isDepth) {
                NOX_ASSERT(clearDepthStencilValue != nullptr);
                clearDepthAttachment(clearDepthStencilValue->depth);
            }

            if (attachmentFormatDescription.isStencil) {
                NOX_ASSERT(clearDepthStencilValue != nullptr);
                clearStencilAttachment(clearDepthStencilValue->stencil);
            }

            if (attachmentFormatDescription.isDepthStencil) {
                NOX_ASSERT(clearDepthStencilValue != nullptr);
                clearDepthStencilAttachment(clearDepthStencilValue->depth, clearDepthStencilValue->stencil);
            }
        }
    }
}

void GLFramebuffer::clearColorAttachment(const ClearColorValue &value, int32_t index) const {
    if (const auto *colorF = std::get_if<Vector4D<float>>(&value)) {
        glClearNamedFramebufferfv(m_handle, GL_COLOR, index, colorF->values.data());
    } else if (const auto *colorI = std::get_if<Vector4D<int32_t>>(&value)) {
        glClearNamedFramebufferiv(m_handle, GL_COLOR, index, colorI->values.data());
    } else if (const auto *colorUI = std::get_if<Vector4D<uint32_t>>(&value)) {
        glClearNamedFramebufferuiv(m_handle, GL_COLOR, index, colorUI->values.data());
    }
}

void GLFramebuffer::clearDepthAttachment(float depth) const {
    glClearNamedFramebufferfv(m_handle, GL_DEPTH, 0, &depth);
}

void GLFramebuffer::clearStencilAttachment(uint32_t stencil) const {
    glClearNamedFramebufferuiv(m_handle, GL_STENCIL, 0, &stencil);
}

void GLFramebuffer::clearDepthStencilAttachment(float depth, uint32_t stencil) const {
    glClearNamedFramebufferfi(m_handle, GL_DEPTH_STENCIL, 0, depth, static_cast<GLint>(stencil));
}

void GLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

GLFramebufferWithAttachments::GLFramebufferWithAttachments(const FramebufferDescription &description) {
    glCreateFramebuffers(1, &m_handle);

    m_attachmentsPoints.reserve(description.attachments.size());

    attachColorAttachments(description.attachments);
    attachDepthStencilAttachments(description.attachments);
}

void GLFramebufferWithAttachments::attachColorAttachments(const AttachmentsContainer &attachments) {
    for (const auto *attachment : attachments) {
        const auto description = getImageFormatDescription(attachment->getFormat());

        if (description.isColor) {
            auto attachmentPoint = GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(m_attachmentsPoints.size());
            attachAttachment(attachment, attachmentPoint);
        }
    }

    auto attachmentsCount = static_cast<GLsizei>(m_attachmentsPoints.size());
    if (attachmentsCount > 0u) {
        glNamedFramebufferDrawBuffers(m_handle, attachmentsCount, m_attachmentsPoints.data());
    } else {
        glNamedFramebufferDrawBuffer(m_handle, GL_NONE);
    }
}

void GLFramebufferWithAttachments::attachDepthStencilAttachments(const AttachmentsContainer &attachments) {
    for (const auto *attachment : attachments) {
        const auto description = getImageFormatDescription(attachment->getFormat());
        AttachmentPoint attachmentPoint = GL_NONE;

        if (description.isDepth) {
            attachmentPoint = GL_DEPTH_ATTACHMENT;
        } else if (description.isStencil) {
            attachmentPoint = GL_STENCIL_ATTACHMENT;
        } else if (description.isDepthStencil) {
            attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;
        }

        if (attachmentPoint != GL_NONE) {
            attachAttachment(attachment, attachmentPoint);
        }
    }
}

void GLFramebufferWithAttachments::attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint) {
    const auto *glTexture = static_cast<const GLTexture *>(attachment);
    glNamedFramebufferTexture(m_handle, attachmentPoint, glTexture->getHandle(), 0);
    m_attachmentsPoints.push_back(attachmentPoint);
}

GLDefaultFramebuffer::GLDefaultFramebuffer() {
    m_attachmentsPoints.reserve(1u);
    m_attachmentsPoints.push_back(GL_BACK_LEFT);
    glNamedFramebufferDrawBuffers(m_handle, 1u, m_attachmentsPoints.data());
}

} // namespace nox
