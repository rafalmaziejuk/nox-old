#include "asserts.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_command_list.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_render_pass.h"

#include <glad/gl.h>

namespace nox {

bool GLCommandList::validateInput(const RenderPassBeginDescriptor &descriptor) {
    bool result = true;

    result &= (descriptor.framebuffer != nullptr);
    result &= (descriptor.renderPass != nullptr);
    result &= (!descriptor.clearValues.empty());

    return result;
}

GLCommandList::GLCommandList(const CommandListDescriptor & /*descriptor*/) {}

void GLCommandList::setViewport(const Viewport &viewport) {
    auto x = static_cast<GLint>(viewport.x);
    auto y = static_cast<GLint>(viewport.y);
    auto width = static_cast<GLsizei>(viewport.width);
    auto height = static_cast<GLsizei>(viewport.height);
    glViewport(x, y, width, height);
    glDepthRangef(viewport.nearClip, viewport.farClip);
}

void GLCommandList::beginRenderPass(const RenderPassBeginDescriptor &descriptor) {
    NOX_ASSERT(validateInput(descriptor));

    const auto *glFramebuffer = static_cast<const GLFramebuffer *>(descriptor.framebuffer);
    const auto *glRenderPass = static_cast<const GLRenderPass *>(descriptor.renderPass);
    m_state.currentFramebuffer = glFramebuffer;
    m_state.currentRenderPass = glRenderPass;

    const auto &attachmentDescriptors = glRenderPass->getAttachmentDescriptors();
    glFramebuffer->bind();
    glFramebuffer->clearAttachments(descriptor.clearValues, attachmentDescriptors);
    glFramebuffer->invalidateAttachments(attachmentDescriptors);
}

void GLCommandList::endRenderPass() {
    m_state.currentFramebuffer->unbind();

    m_state.currentFramebuffer = nullptr;
    m_state.currentRenderPass = nullptr;
}

void GLCommandList::bindGraphicsPipelineState(const GraphicsPipelineState &pipelineState) {
    NOX_ASSERT(m_state.currentRenderPass != nullptr);
    NOX_ASSERT(m_state.currentFramebuffer != nullptr);

    const auto *pipeline = static_cast<const GLGraphicsPipelineState *>(&pipelineState);
    const auto *currentRenderPass = m_state.currentRenderPass;
    const auto *currentFramebuffer = m_state.currentFramebuffer;

    const auto &pipelineLayout = pipeline->getPipelineLayout();
    const auto &subpassDescriptor = currentRenderPass->getSubpassDescriptor(pipeline->getSubpassIndex());
    const auto &inputAttachmentBindings = pipelineLayout.getInputAttachmentBindings();
    for (const auto &reference : subpassDescriptor.inputAttachmentReferences) {
        if (reference.index != AttachmentReference::attachmentUnused) {
            inputAttachmentBindings[reference.index].bind();
        }
    }
    currentFramebuffer->bindColorAttachments(subpassDescriptor.colorAttachmentReferences);

    const auto &textureBindings = pipelineLayout.getTextureBindings();
    for (const auto &binding : textureBindings) {
        binding.bind();
    }

    m_state.primitiveTopology = pipeline->getPrimitiveTopology();

    pipeline->bind();
}

void GLCommandList::bindVertexBuffer(const Buffer &buffer) {
    const auto *vertexBuffer = static_cast<const GLVertexBuffer *>(&buffer);
    auto vertexArrayIndex = vertexBuffer->getVertexArrayIndex();

    auto &vertexArrayRegistry = GLVertexArrayRegistry::instance();
    vertexArrayRegistry.setBoundVertexArrayIndex(vertexArrayIndex);

    const auto &vertexArray = vertexArrayRegistry.getVertexArray(vertexArrayIndex);
    vertexArray.bind();
}

void GLCommandList::bindIndexBuffer(const Buffer &buffer) {
    const auto *indexBuffer = static_cast<const GLIndexBuffer *>(&buffer);
    m_state.indexType = indexBuffer->getIndexType();

    auto &vertexArrayRegistry = GLVertexArrayRegistry::instance();
    auto vertexArrayIndex = vertexArrayRegistry.getBoundVertexArrayIndex();

    auto &vertexArray = vertexArrayRegistry.getVertexArray(vertexArrayIndex);
    vertexArray.setIndexBuffer(indexBuffer->getHandle());
}

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    glDrawArrays(m_state.primitiveTopology,
                 static_cast<GLint>(firstVertexIndex),
                 static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    glDrawElements(m_state.primitiveTopology,
                   static_cast<GLsizei>(vertexCount),
                   m_state.indexType,
                   nullptr);
}

} // namespace nox
