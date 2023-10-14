#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_command_list.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_state.h"
#include "renderer/opengl/gl_vertex_array.h"

namespace NOX {

namespace {

GLbitfield mapClearMaskToBitfield(uint32_t mask) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    GLbitfield result = 0u;

    if (mask & ClearMask::COLOR) {
        result |= GL_COLOR_BUFFER_BIT;
    }
    if (mask & ClearMask::DEPTH) {
        result |= GL_DEPTH_BUFFER_BIT;
    }
    if (mask & ClearMask::STENCIL) {
        result |= GL_STENCIL_BUFFER_BIT;
    }

    return result;
}

} // namespace

GLCommandList::GLCommandList(const CommandListDescriptor & /*descriptor*/,
                             std::shared_ptr<GLState> state) : m_state{std::move(state)} {
    NOX_LOG_TRACE_DECLARE(OPENGL);
}

void GLCommandList::bindVertexBuffer(const Buffer &buffer) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    const auto *glVertexBuffer = downcast<GLVertexBuffer>(buffer);
    auto vertexArrayIndex = glVertexBuffer->getVertexArrayIndex();
    const auto &vertexArray = m_state->vertexArrays[vertexArrayIndex];
    vertexArray->bind();
    m_state->currentVertexArrayIndex = vertexArrayIndex;
}

void GLCommandList::bindIndexBuffer(const Buffer &buffer) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    const auto *glIndexBuffer = downcast<GLIndexBuffer>(buffer);
    const auto &vertexArray = m_state->vertexArrays[m_state->currentVertexArrayIndex];
    vertexArray->setIndexBuffer(*glIndexBuffer);
    m_state->indexType = glIndexBuffer->getIndexType();
}

void GLCommandList::bindPipelineState(const PipelineState &pipeline) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    const auto *glPipelineState = downcast<GLPipelineState>(pipeline);
    glPipelineState->bind();
}

void GLCommandList::setViewport(const Viewport &viewport) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    auto x = static_cast<GLint>(viewport.x);
    auto y = static_cast<GLint>(viewport.y);
    auto width = static_cast<GLsizei>(viewport.width);
    auto height = static_cast<GLsizei>(viewport.height);
    glViewport(x, y, width, height);
    glDepthRangef(viewport.nearClip, viewport.farClip);
}

void GLCommandList::setClearColor(const Vector4D<float> &color) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glClearColor(color.r(), color.g(), color.b(), color.a());
}

void GLCommandList::setClearDepth(float depth) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glClearDepthf(depth);
}

void GLCommandList::setClearStencil(uint32_t stencil) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glClearStencil(stencil);
}

void GLCommandList::clear(uint32_t mask) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glClear(mapClearMaskToBitfield(mask));
}

void GLCommandList::clearColor(const Vector4D<float> &color, uint8_t index) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);

    m_state->currentRenderTarget->clear(color, index);
}

void GLCommandList::clearColor(const Vector4D<int32_t> &color, uint8_t index) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);

    m_state->currentRenderTarget->clear(color, index);
}

void GLCommandList::clearColor(const Vector4D<uint32_t> &color, uint8_t index) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);

    m_state->currentRenderTarget->clear(color, index);
}

void GLCommandList::clearDepth(float depth) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);

    m_state->currentRenderTarget->clear(depth);
}

void GLCommandList::clearStencil(uint32_t stencil) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);

    m_state->currentRenderTarget->clear(stencil);
}

void GLCommandList::clearDepthStencil(float depth, uint32_t stencil) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);
    NOX_ASSERT(m_state->currentRenderTarget == nullptr);

    m_state->currentRenderTarget->clear(depth, stencil);
}

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glDrawArrays(m_state->primitiveTopology, static_cast<GLint>(firstVertexIndex), static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glDrawElements(m_state->primitiveTopology, vertexCount, m_state->indexType, nullptr);
}

void GLCommandList::beginRenderPass(const RenderPass &renderPass) {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    const auto *glRenderPass = downcast<GLRenderPass>(renderPass);
    const auto &glPipelineState = glRenderPass->getPipelineState();
    const auto &glRenderTarget = glPipelineState.getRenderTarget();
    glPipelineState.bind();
    glRenderTarget.bind();
    m_state->currentRenderTarget = &glRenderTarget;
}

void GLCommandList::endRenderPass() {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    m_state->currentRenderTarget = nullptr;
}

} // namespace NOX
