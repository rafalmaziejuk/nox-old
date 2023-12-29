#include "format_descriptor.h"
#include "nox_assert.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_framebuffer_visitor.h"
#include "opengl/gl_render_pass.h"
#include "opengl/gl_render_pass_visitor.h"
#include "opengl/gl_texture.h"
#include "opengl/gl_texture_visitor.h"

#include <glad/gl.h>

namespace nox {

bool GLFramebuffer::validateInput(const FramebufferDescriptor &descriptor) {
    bool result = true;

    auto validateAttachments = [&descriptor]() {
        if (descriptor.renderPass == nullptr) {
            return false;
        }

        GLRenderPassVisitor renderPassVisitor{};
        descriptor.renderPass->accept(renderPassVisitor);
        const auto &glRenderPass = renderPassVisitor.get();
        const auto &attachmentsDescriptors = glRenderPass.getAttachmentsDescriptors();

        for (size_t i = 0u; i < descriptor.attachments.size(); i++) {
            const auto &attachment = *descriptor.attachments[i];
            const auto &attachmentDescriptor = attachmentsDescriptors[i];

            if (attachment.getFormat() != attachmentDescriptor.format) {
                return false;
            }
        }

        return true;
    };

    result &= validateAttachments();
    result &= (descriptor.size.x() > 0u);
    result &= (descriptor.size.y() > 0u);

    return result;
}

bool GLFramebuffer::isDefaultFramebuffer(const AttachmentsContainer &attachments) {
    if (attachments.size() > 1u) {
        return false;
    }

    GLTextureVisitor visitor{};
    attachments.back()->accept(visitor);

    return visitor.isDefaultFramebufferAttachment();
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
    GLRenderPassVisitor visitor{};
    renderPass->accept(visitor);
    const auto &glRenderPass = visitor.get();
    const auto &attachmentsDescriptors = glRenderPass.getAttachmentsDescriptors();

    for (size_t i = 0u; i < attachmentsDescriptors.size(); i++) {
        const auto &attachmentDescriptor = attachmentsDescriptors[i];
        const auto &clearValue = values[i];

        if (attachmentDescriptor.loadOp == AttachmentLoadOp::CLEAR) {
            if (const auto *value = std::get_if<ClearColorValue>(&clearValue)) {
                clearColorAttachment(*value, static_cast<int32_t>(i));
            }

            if (const auto *value = std::get_if<ClearDepthStencilValue>(&clearValue)) {
                clearDepthAttachment(value->depth);
            }
        }

        if (attachmentDescriptor.stencilLoadOp == AttachmentLoadOp::CLEAR) {
            if (const auto *value = std::get_if<ClearDepthStencilValue>(&clearValue)) {
                clearStencilAttachment(value->stencil);
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

void GLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

void GLFramebuffer::accept(FramebufferVisitor &visitor) const {
    visitor.visit(*this);
}

GLFramebufferWithAttachments::GLFramebufferWithAttachments(const FramebufferDescriptor &descriptor) {
    glCreateFramebuffers(1, &m_handle);

    m_attachmentsPoints.reserve(descriptor.attachments.size());

    attachColorAttachments(descriptor.attachments);
    attachDepthStencilAttachments(descriptor.attachments);
}

void GLFramebufferWithAttachments::attachColorAttachments(const AttachmentsContainer &attachments) {
    for (const auto *attachment : attachments) {
        auto descriptor = getImageFormatDescriptor(attachment->getFormat());

        if (descriptor.isColor) {
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
        auto descriptor = getImageFormatDescriptor(attachment->getFormat());
        AttachmentPoint attachmentPoint = GL_NONE;

        if (descriptor.isDepth) {
            attachmentPoint = GL_DEPTH_ATTACHMENT;
        } else if (descriptor.isStencil) {
            attachmentPoint = GL_STENCIL_ATTACHMENT;
        } else if (descriptor.isDepthStencil) {
            attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;
        }

        if (attachmentPoint != GL_NONE) {
            attachAttachment(attachment, attachmentPoint);
        }
    }
}

void GLFramebufferWithAttachments::attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint) {
    GLTextureVisitor visitor{};
    attachment->accept(visitor);
    const auto &glTexture = visitor.get();

    glNamedFramebufferTexture(m_handle, attachmentPoint, glTexture.getHandle(), 0);
    m_attachmentsPoints.push_back(attachmentPoint);
}

GLDefaultFramebuffer::GLDefaultFramebuffer() {
    m_attachmentsPoints.reserve(1u);
    m_attachmentsPoints.push_back(GL_BACK_LEFT);
    glNamedFramebufferDrawBuffers(m_handle, 1u, m_attachmentsPoints.data());
}

} // namespace nox
