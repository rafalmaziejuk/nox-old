#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_command_list.h"
#include "renderer/opengl/gl_helper.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_state.h"
#include "renderer/opengl/gl_vertex_array.h"

namespace NOX {

GLCommandList::GLCommandList(const CommandListDescriptor & /*descriptor*/,
                             std::shared_ptr<GLState> state) : m_state{std::move(state)} {}

void GLCommandList::bindVertexBuffer(const Buffer &buffer) {
    const auto &vertexBuffer = static_cast<const GLVertexBuffer &>(buffer);
    auto vertexArrayIndex = vertexBuffer.getVertexArrayIndex();
    const auto &vertexArray = m_state->vertexArrays[vertexArrayIndex];
    vertexArray->bind();
    m_state->currentlyBoundVertexArrayIndex = vertexArrayIndex;
}

void GLCommandList::bindIndexBuffer(const Buffer &buffer) {
    const auto &indexBuffer = static_cast<const GLIndexBuffer &>(buffer);
    const auto &vertexArray = m_state->vertexArrays[m_state->currentlyBoundVertexArrayIndex];
    vertexArray->setIndexBuffer(indexBuffer);
    m_state->indexType = indexBuffer.getIndexType();
}

void GLCommandList::bindPipelineState(const PipelineState &pipeline) {
    const auto &pipelineState = static_cast<const GLPipelineState &>(pipeline);
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

void GLCommandList::setClearColor(const Vector3D<float> &color) {
    glClearColor(color.x, color.y, color.z, 1.0f);
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

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    auto mode = m_state->primitiveTopology;
    glDrawArrays(mode, static_cast<GLint>(firstVertexIndex), static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    auto mode = m_state->primitiveTopology;
    glDrawElements(mode, vertexCount, static_cast<GLenum>(m_state->indexType), nullptr);
}

} // namespace NOX
