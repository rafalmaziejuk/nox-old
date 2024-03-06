#include "asserts.h"
#include "format_descriptor.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_render_pass.h"
#include "opengl/gl_texture.h"

#include <glad/gl.h>

namespace nox {

bool GLFramebuffer::validateInput(const FramebufferDescriptor &descriptor) {
    bool result = true;

    auto validateAttachments = [&descriptor]() {
        if (descriptor.renderPass == nullptr) {
            return false;
        }

        const auto *glRenderPass = static_cast<const GLRenderPass *>(descriptor.renderPass);
        const auto &attachmentDescriptors = glRenderPass->getAttachmentDescriptors();

        for (size_t i = 0u; i < descriptor.attachments.size(); i++) {
            const auto &attachment = *descriptor.attachments[i];
            const auto &attachmentDescriptor = attachmentDescriptors[i];

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

GLFramebuffer::~GLFramebuffer() {
    auto attachmentsCount = static_cast<GLsizei>(m_attachmentPoints.size());
    glInvalidateNamedFramebufferData(m_handle, attachmentsCount, m_attachmentPoints.data());
    glDeleteFramebuffers(1, &m_handle);
}

bool GLFramebuffer::validate() const {
    return (glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void GLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

void GLFramebuffer::unbind() const { // NOLINT(readability-convert-member-functions-to-static)
    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

void GLFramebuffer::clearAttachments(const ClearValues &values, const AttachmentDescriptors &attachmentDescriptors) const {
    NOX_ASSERT(values.size() == attachmentDescriptors.size());
    NOX_ASSERT(values.size() == m_attachmentPoints.size());

    for (size_t i = 0u; i < m_attachmentPoints.size(); i++) {
        const auto &attachmentDescriptor = attachmentDescriptors[i];

        if (attachmentDescriptor.loadOp == AttachmentLoadOp::CLEAR) {
            const auto attachmentFormatDescriptor = getImageFormatDescriptor(attachmentDescriptor.format);
            const auto *clearColorValue = std::get_if<ClearColorValue>(&values[i]);
            const auto *clearDepthStencilValue = std::get_if<ClearDepthStencilValue>(&values[i]);

            if (attachmentFormatDescriptor.isColor) {
                clearColorAttachment(clearColorValue, static_cast<int32_t>(i));
            }

            if (attachmentFormatDescriptor.isDepth) {
                clearDepthAttachment(clearDepthStencilValue);
            }

            if (attachmentFormatDescriptor.isStencil) {
                clearStencilAttachment(clearDepthStencilValue);
            }

            if (attachmentFormatDescriptor.isDepthStencil) {
                clearDepthStencilAttachment(clearDepthStencilValue);
            }
        }
    }
}

void GLFramebuffer::bindColorAttachments(const ColorAttachmentReferences &attachmentReferences) const {
    AttachmentPoints attachmentPoints;
    for (const auto &attachmentReference : attachmentReferences) {
        attachmentPoints.push_back(GL_COLOR_ATTACHMENT0 + attachmentReference.index);
    }
    glNamedFramebufferDrawBuffers(m_handle,
                                  static_cast<GLsizei>(attachmentPoints.size()),
                                  attachmentPoints.data());
}

void GLFramebuffer::invalidateAttachments(const AttachmentDescriptors &attachmentDescriptors) const {
    AttachmentPoints attachmentPoints;
    for (size_t i = 0u; i < m_attachmentPoints.size(); i++) {
        const auto &attachmentDescriptor = attachmentDescriptors[i];
        const auto attachmentFormatDescriptor = getImageFormatDescriptor(attachmentDescriptor.format);

        if (attachmentDescriptor.storeOp == AttachmentStoreOp::DONT_CARE) {
            if (attachmentFormatDescriptor.isColor) {
                attachmentPoints.push_back(GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(i));
            }

            if (attachmentFormatDescriptor.isDepth) {
                attachmentPoints.push_back(GL_DEPTH_ATTACHMENT);
            }

            if (attachmentFormatDescriptor.isStencil) {
                attachmentPoints.push_back(GL_STENCIL_ATTACHMENT);
            }

            if (attachmentFormatDescriptor.isDepthStencil) {
                attachmentPoints.push_back(GL_DEPTH_STENCIL_ATTACHMENT);
            }
        }
    }

    auto attachmentPointsCount = static_cast<GLsizei>(attachmentPoints.size());
    glInvalidateNamedFramebufferData(m_handle, attachmentPointsCount, attachmentPoints.data());
}

void GLFramebuffer::clearColorAttachment(const ClearColorValue *value, int32_t index) const {
    NOX_ASSERT(value != nullptr);

    if (const auto *colorF = std::get_if<Vector4D<float>>(value)) {
        glClearNamedFramebufferfv(m_handle, GL_COLOR, index, colorF->values.data());
    } else if (const auto *colorI = std::get_if<Vector4D<int32_t>>(value)) {
        glClearNamedFramebufferiv(m_handle, GL_COLOR, index, colorI->values.data());
    } else if (const auto *colorUI = std::get_if<Vector4D<uint32_t>>(value)) {
        glClearNamedFramebufferuiv(m_handle, GL_COLOR, index, colorUI->values.data());
    }
}

void GLFramebuffer::clearDepthAttachment(const ClearDepthStencilValue *value) const {
    NOX_ASSERT(value != nullptr);

    glClearNamedFramebufferfv(m_handle, GL_DEPTH, 0, &value->depth);
}

void GLFramebuffer::clearStencilAttachment(const ClearDepthStencilValue *value) const {
    NOX_ASSERT(value != nullptr);

    glClearNamedFramebufferuiv(m_handle, GL_STENCIL, 0, &value->stencil);
}

void GLFramebuffer::clearDepthStencilAttachment(const ClearDepthStencilValue *value) const {
    NOX_ASSERT(value != nullptr);

    glClearNamedFramebufferfi(m_handle, GL_DEPTH_STENCIL, 0, value->depth, static_cast<GLint>(value->stencil));
}

GLFramebuffer::GLFramebuffer(const FramebufferDescriptor &descriptor) {
    glCreateFramebuffers(1, &m_handle);

    m_attachmentPoints.reserve(descriptor.attachments.size());

    attachColorAttachments(descriptor.attachments);
    attachDepthStencilAttachments(descriptor.attachments);
}

void GLFramebuffer::attachColorAttachments(const Attachments &attachments) {
    for (const auto *attachment : attachments) {
        const auto descriptor = getImageFormatDescriptor(attachment->getFormat());

        if (descriptor.isColor) {
            auto attachmentPoint = GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(m_attachmentPoints.size());
            attachAttachment(attachment, attachmentPoint);
        }
    }
}

void GLFramebuffer::attachDepthStencilAttachments(const Attachments &attachments) {
    for (const auto *attachment : attachments) {
        const auto descriptor = getImageFormatDescriptor(attachment->getFormat());
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

void GLFramebuffer::attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint) {
    const auto *glTexture = static_cast<const GLTexture *>(attachment);
    glNamedFramebufferTexture(m_handle, attachmentPoint, glTexture->getHandle(), 0);
    m_attachmentPoints.push_back(attachmentPoint);
}

} // namespace nox
