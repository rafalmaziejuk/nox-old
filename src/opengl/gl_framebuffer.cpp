#include "asserts.h"
#include "format_descriptor.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_render_pass.h"
#include "opengl/gl_texture.h"

#include <glad/gl.h>

namespace nox {

namespace {

[[nodiscard]] ImageFormatType getAttachmentType(AttachmentPoint attachmentPoint) {
    switch (attachmentPoint) {
    case GL_DEPTH_ATTACHMENT: return ImageFormatType::DEPTH;
    case GL_STENCIL_ATTACHMENT: return ImageFormatType::STENCIL;
    case GL_DEPTH_STENCIL_ATTACHMENT: return ImageFormatType::DEPTH_STENCIL;
    default: return ImageFormatType::COLOR;
    }
}

} // namespace

std::unique_ptr<GLFramebuffer> GLFramebuffer::create(const FramebufferDescriptor &descriptor) {
    std::unique_ptr<GLFramebuffer> framebuffer{nullptr};
    if (descriptor.attachments.empty()) {
        framebuffer = std::make_unique<GLFramebuffer>();
    } else {
        framebuffer = std::make_unique<GLFramebuffer>(descriptor);
        NOX_ENSURE_RETURN_NULLPTR_MSG(framebuffer->attachAttachments(descriptor.attachments),
                                      "Couldn't attach attachments successfully");
    }
    NOX_ENSURE_RETURN_NULLPTR_MSG(framebuffer->isComplete(), "Framebuffer isn't complete");

    const auto *glRenderPass = static_cast<const GLRenderPass *>(descriptor.renderPass);
    NOX_ENSURE_RETURN_NULLPTR_MSG(framebuffer->isCompatible(glRenderPass), "Framebuffer isn't compatible with given render pass")

    return framebuffer;
}

GLFramebuffer::GLFramebuffer() {
    m_attachmentPoints.reserve(1u);
    m_attachmentPoints.push_back(GL_BACK_LEFT);
}

GLFramebuffer::GLFramebuffer(const FramebufferDescriptor &descriptor) {
    glCreateFramebuffers(1, &m_handle);

    m_attachmentPoints.reserve(descriptor.attachments.size());
}

GLFramebuffer::~GLFramebuffer() {
    auto attachmentsCount = static_cast<GLsizei>(m_attachmentPoints.size());
    glInvalidateNamedFramebufferData(m_handle, attachmentsCount, m_attachmentPoints.data());
    glDeleteFramebuffers(1, &m_handle);
}

void GLFramebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

void GLFramebuffer::clearAttachments(const ClearValues &values, const AttachmentDescriptors &descriptors) const {
    for (size_t i = 0u; i < m_attachmentPoints.size(); i++) {
        if (descriptors[i].loadOp == AttachmentLoadOp::CLEAR) {
            const auto attachmentType = getAttachmentType(m_attachmentPoints[i]);
            const auto *clearColorValue = std::get_if<ClearColorValue>(&values[i]);
            const auto *clearDepthStencilValue = std::get_if<ClearDepthStencilValue>(&values[i]);

            if (attachmentType == ImageFormatType::COLOR) {
                clearColorAttachment(clearColorValue, static_cast<int32_t>(i));
            } else if (attachmentType == ImageFormatType::DEPTH) {
                clearDepthAttachment(clearDepthStencilValue);
            } else if (attachmentType == ImageFormatType::STENCIL) {
                clearStencilAttachment(clearDepthStencilValue);
            } else if (attachmentType == ImageFormatType::DEPTH_STENCIL) {
                clearDepthStencilAttachment(clearDepthStencilValue);
            }
        }
    }
}

void GLFramebuffer::invalidateAttachments(const AttachmentDescriptors &descriptors) const {
    AttachmentPoints attachmentPoints;
    attachmentPoints.reserve(descriptors.size());

    for (size_t i = 0u; i < m_attachmentPoints.size(); i++) {
        if (descriptors[i].storeOp == AttachmentStoreOp::DONT_CARE) {
            attachmentPoints.push_back(m_attachmentPoints[i]);
        }
    }

    auto attachmentPointsCount = static_cast<GLsizei>(attachmentPoints.size());
    glInvalidateNamedFramebufferData(m_handle, attachmentPointsCount, attachmentPoints.data());
}

void GLFramebuffer::bindColorAttachments(const ColorAttachmentReferences &references) const {
    AttachmentPoints attachmentPoints;
    attachmentPoints.reserve(references.size());

    for (const auto &attachmentReference : references) {
        attachmentPoints.push_back(m_attachmentPoints[attachmentReference.index]);
    }
    glNamedFramebufferDrawBuffers(m_handle,
                                  static_cast<GLsizei>(attachmentPoints.size()),
                                  attachmentPoints.data());
}

bool GLFramebuffer::isComplete() const {
    return (glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

bool GLFramebuffer::isCompatible(const GLRenderPass *renderPass) const {
    bool result = true;

    result &= (m_attachmentPoints.size() == renderPass->getAttachmentDescriptors().size());

    return result;
}

bool GLFramebuffer::attachAttachments(const Attachments &attachments) {
    for (const auto *attachment : attachments) {
        const auto attachmentFormat = attachment->getFormat();
        const auto attachmentType = getImageFormatType(attachmentFormat);
        AttachmentPoint attachmentPoint = GL_NONE;

        if (attachmentType == ImageFormatType::COLOR) {
            attachmentPoint = GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(m_attachmentPoints.size());
        } else if (attachmentType == ImageFormatType::DEPTH) {
            attachmentPoint = GL_DEPTH_ATTACHMENT;
        } else if (attachmentType == ImageFormatType::STENCIL) {
            attachmentPoint = GL_STENCIL_ATTACHMENT;
        } else if (attachmentType == ImageFormatType::DEPTH_STENCIL) {
            attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;
        }
        NOX_ENSURE_RETURN_FALSE(attachmentPoint != GL_NONE);

        attachAttachment(attachment, attachmentPoint);
    }

    return true;
}

void GLFramebuffer::attachAttachment(const Texture *attachment, AttachmentPoint attachmentPoint) {
    const auto *glTexture = static_cast<const GLTexture *>(attachment);
    glNamedFramebufferTexture(m_handle, attachmentPoint, glTexture->getHandle(), 0);
    m_attachmentPoints.push_back(attachmentPoint);
}

void GLFramebuffer::clearColorAttachment(const ClearColorValue *value, int32_t index) const {
    if (const auto *colorF = std::get_if<Vector4D<float>>(value)) {
        glClearNamedFramebufferfv(m_handle, GL_COLOR, index, colorF->values.data());
    } else if (const auto *colorI = std::get_if<Vector4D<int32_t>>(value)) {
        glClearNamedFramebufferiv(m_handle, GL_COLOR, index, colorI->values.data());
    } else if (const auto *colorUI = std::get_if<Vector4D<uint32_t>>(value)) {
        glClearNamedFramebufferuiv(m_handle, GL_COLOR, index, colorUI->values.data());
    }
}

void GLFramebuffer::clearDepthAttachment(const ClearDepthStencilValue *value) const {
    glClearNamedFramebufferfv(m_handle, GL_DEPTH, 0, &value->depth);
}

void GLFramebuffer::clearStencilAttachment(const ClearDepthStencilValue *value) const {
    glClearNamedFramebufferuiv(m_handle, GL_STENCIL, 0, &value->stencil);
}

void GLFramebuffer::clearDepthStencilAttachment(const ClearDepthStencilValue *value) const {
    glClearNamedFramebufferfi(m_handle, GL_DEPTH_STENCIL, 0, value->depth, static_cast<GLint>(value->stencil));
}

} // namespace nox
