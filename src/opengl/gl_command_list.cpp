#include "asserts.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_command_list.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_render_pass.h"

#include <glad/gl.h>

namespace nox {

GLCommandList::GLCommandList(const CommandListDescriptor & /*descriptor*/) {}

void GLCommandList::setViewport(const Viewport &viewport) {
    auto x = static_cast<GLint>(viewport.x);
    auto y = static_cast<GLint>(viewport.y);
    auto width = static_cast<GLsizei>(viewport.width);
    auto height = static_cast<GLsizei>(viewport.height);
    glViewport(x, y, width, height);
    glDepthRangef(viewport.nearClip, viewport.farClip);
}

void GLCommandList::beginRenderPass(const RenderPassBeginInfo &info) {
    NOX_ASSERT(info.framebuffer != nullptr);
    NOX_ASSERT(info.renderPass != nullptr);
    NOX_ASSERT(!info.clearValues.empty());

    const auto *glFramebuffer = static_cast<const GLFramebuffer *>(info.framebuffer);
    const auto *glRenderPass = static_cast<const GLRenderPass *>(info.renderPass);
    m_state.currentFramebuffer = glFramebuffer;
    m_state.currentRenderPass = glRenderPass;

    const auto &attachmentDescriptors = glRenderPass->getAttachmentDescriptors();
    glFramebuffer->bind();
    glFramebuffer->clearAttachments(info.clearValues, attachmentDescriptors);
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

    m_state.currentPipeline = static_cast<const GLGraphicsPipelineState *>(&pipelineState);

    const auto *currentPipeline = m_state.currentPipeline;
    const auto *currentRenderPass = m_state.currentRenderPass;
    const auto *currentFramebuffer = m_state.currentFramebuffer;

    const auto &subpassDescriptor = currentRenderPass->getSubpassDescriptor(currentPipeline->getSubpassIndex());
    currentFramebuffer->bindColorAttachments(subpassDescriptor.colorAttachmentReferences);

    m_state.primitiveTopology = currentPipeline->getPrimitiveTopology();

    currentPipeline->bind();
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

void GLCommandList::bindDescriptorSets(const DescriptorSetsBindInfo &info) {
    NOX_ASSERT(m_state.currentPipeline != nullptr);
    NOX_ASSERT(info.pipelineLayout != nullptr);

    const auto *currentPipeline = m_state.currentPipeline;
    const auto *currentRenderPass = m_state.currentRenderPass;
    const auto &subpassDescriptor = currentRenderPass->getSubpassDescriptor(currentPipeline->getSubpassIndex());

    for (const auto &reference : subpassDescriptor.inputAttachmentReferences) {
        if (reference.index != AttachmentReference::attachmentUnused) {
            inputAttachmentBindings[reference.index].bind();
        }
    }

    const auto &textureBindings = pipelineLayout->getTextureBindings();
    for (const auto &binding : textureBindings) {
        binding.bind();
    }
}

void GLCommandList::bindUniforms(const UniformsBindInfo &info) {
    NOX_ASSERT(info.size > 0u);
    NOX_ASSERT(info.data != nullptr);
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
