#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_command_list.h"
#include "renderer/opengl/gl_helper.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_state.h"
#include "renderer/opengl/gl_vertex_array.h"

namespace NOX {

GLCommandList::GLCommandList(const CommandListDescriptor & /*descriptor*/,
                             std::shared_ptr<GLState> state) : m_state{std::move(state)} {}

void GLCommandList::bindVertexBuffer(const Buffer &buffer) {
    const auto &vertexBuffer = dynamic_cast<const GLVertexBuffer &>(buffer);
    auto vertexArrayIndex = vertexBuffer.getVertexArrayIndex();
    const auto &vertexArray = m_state->vertexArrays[vertexArrayIndex];
    vertexArray->bind();
    m_state->currentVertexArrayIndex = vertexArrayIndex;
}

void GLCommandList::bindIndexBuffer(const Buffer &buffer) {
    const auto &indexBuffer = dynamic_cast<const GLIndexBuffer &>(buffer);
    const auto &vertexArray = m_state->vertexArrays[m_state->currentVertexArrayIndex];
    vertexArray->setIndexBuffer(indexBuffer);
    m_state->indexType = indexBuffer.getIndexType();
}

void GLCommandList::bindPipelineState(const PipelineState &pipeline) {
    const auto &pipelineState = dynamic_cast<const GLPipelineState &>(pipeline);
    pipelineState.bind();
}

void GLCommandList::setViewport(const Viewport &viewport) {
    auto x = static_cast<GLint>(viewport.x);
    auto y = static_cast<GLint>(viewport.y);
    auto width = static_cast<GLsizei>(viewport.width);
    auto height = static_cast<GLsizei>(viewport.height);
    glViewport(x, y, width, height);
    glDepthRangef(viewport.nearClip, viewport.farClip);
}

void GLCommandList::setClearColor(const Vector4D<float> &color) {
    glClearColor(color.r(), color.g(), color.b(), color.a());
}

void GLCommandList::setClearDepth(float depth) {
    glClearDepthf(depth);
}

void GLCommandList::setClearStencil(uint32_t stencil) {
    glClearStencil(stencil);
}

void GLCommandList::clear(uint8_t flags) {
    glClear(GLHelper::mapClearFlags(flags));
}

void GLCommandList::clearColor(const Vector4D<float> &color, uint8_t index) {
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);
    m_state->currentRenderTarget->clear(color, index);
}

void GLCommandList::clearColor(const Vector4D<int32_t> &color, uint8_t index) {
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);
    m_state->currentRenderTarget->clear(color, index);
}

void GLCommandList::clearColor(const Vector4D<uint32_t> &color, uint8_t index) {
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);
    m_state->currentRenderTarget->clear(color, index);
}

void GLCommandList::clearDepth(float depth) {
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);
    m_state->currentRenderTarget->clear(depth);
}

void GLCommandList::clearStencil(uint32_t stencil) {
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);
    m_state->currentRenderTarget->clear(stencil);
}

void GLCommandList::clearDepthStencil(float depth, uint32_t stencil) {
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);
    m_state->currentRenderTarget->clear(depth, stencil);
}

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    auto mode = m_state->primitiveTopology;
    glDrawArrays(mode, static_cast<GLint>(firstVertexIndex), static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    auto mode = m_state->primitiveTopology;
    glDrawElements(mode, vertexCount, static_cast<GLenum>(m_state->indexType), nullptr);
}

} // namespace NOX
