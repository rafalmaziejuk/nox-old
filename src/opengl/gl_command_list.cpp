#include "asserts.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_command_list.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_pipeline_layout.h"
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

void GLCommandList::beginRenderPass(const RenderPassBeginDescriptor &descriptor) {
    m_framebuffer = static_cast<const GLFramebuffer *>(descriptor.framebuffer);
    m_renderPass = static_cast<const GLRenderPass *>(descriptor.renderPass);
    NOX_ASSERT_MSG(m_framebuffer->isCompatible(m_renderPass), "Framebuffer and render pass aren't compatible");

    const auto &attachmentDescriptors = m_renderPass->getAttachmentDescriptors();
    m_framebuffer->bind();
    m_framebuffer->clearAttachments(descriptor.clearValues, attachmentDescriptors);
    m_framebuffer->invalidateAttachments(attachmentDescriptors);
}

void GLCommandList::endRenderPass() {
    m_framebuffer = nullptr;
    m_renderPass = nullptr;
    m_indexType = 0u;
    m_primitiveTopology = 0u;
}

void GLCommandList::bindGraphicsPipelineState(const GraphicsPipelineState &pipelineState) {
    const auto *pipeline = static_cast<const GLGraphicsPipelineState *>(&pipelineState);
    const auto &subpassDescriptor = m_renderPass->getSubpassDescriptor(pipeline->getSubpassIndex());

    m_framebuffer->bindColorAttachments(subpassDescriptor.colorAttachmentReferences);
    m_primitiveTopology = pipeline->getPrimitiveTopology();
    pipeline->bind();
}

void GLCommandList::bindVertexBuffer(const Buffer &buffer) {
    const auto *vertexBuffer = static_cast<const GLVertexBuffer *>(&buffer);

    vertexBuffer->bind();
}

void GLCommandList::bindIndexBuffer(const Buffer &buffer) {
    const auto *indexBuffer = static_cast<const GLIndexBuffer *>(&buffer);

    indexBuffer->bind();
    m_indexType = indexBuffer->getIndexType();
}

void GLCommandList::bindDescriptorSets(const PipelineLayout &pipelineLayout, uint32_t firstSetIndex, uint32_t setCount) {
    const auto *layout = static_cast<const GLPipelineLayout *>(&pipelineLayout);
    const auto &descriptorSets = layout->getDescriptorSets();
    for (uint32_t i = firstSetIndex; i < setCount; i++) {
        descriptorSets[i].bind();
    }
}

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    glDrawArrays(m_primitiveTopology,
                 static_cast<GLint>(firstVertexIndex),
                 static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    glDrawElements(m_primitiveTopology,
                   static_cast<GLsizei>(vertexCount),
                   m_indexType,
                   nullptr);
}

} // namespace nox
