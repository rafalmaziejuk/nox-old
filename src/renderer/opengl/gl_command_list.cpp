#include "core/core.h"
#include "renderer/opengl/gl_buffer.h"
#include "renderer/opengl/gl_command_list.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_state.h"
#include "renderer/opengl/gl_vertex_array.h"

#include <glad/gl.h>

namespace NOX {

namespace {

GLbitfield mapClearMaskToBitfield(uint32_t mask) {
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

GLCommandList::GLCommandList([[maybe_unused]] const CommandListDescriptor &descriptor,
                             GLState &state) : GLWithState{state} {}

void GLCommandList::bindPipelineState(const PipelineState &pipeline) {
    const auto *glPipelineState = downcast<GLPipelineState>(pipeline);
    glPipelineState->bind();
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

void GLCommandList::clear(uint32_t mask) {
    glClear(mapClearMaskToBitfield(mask));
}

void GLCommandList::clearColor(const Vector4D<float> &color, uint8_t index) {
    getState().currentRenderTarget->clear(color, index);
}

void GLCommandList::clearColor(const Vector4D<int32_t> &color, uint8_t index) {
    getState().currentRenderTarget->clear(color, index);
}

void GLCommandList::clearColor(const Vector4D<uint32_t> &color, uint8_t index) {
    getState().currentRenderTarget->clear(color, index);
}

void GLCommandList::clearDepth(float depth) {
    getState().currentRenderTarget->clear(depth);
}

void GLCommandList::clearStencil(uint32_t stencil) {
    getState().currentRenderTarget->clear(stencil);
}

void GLCommandList::clearDepthStencil(float depth, uint32_t stencil) {
    getState().currentRenderTarget->clear(depth, stencil);
}

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    glDrawArrays(getState().primitiveTopology, static_cast<GLint>(firstVertexIndex), static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    glDrawElements(getState().primitiveTopology, vertexCount, getState().indexType, nullptr);
}

void GLCommandList::beginRenderPass(const RenderPass &renderPass) {
    const auto *glRenderPass = downcast<GLRenderPass>(renderPass);
    const auto &glPipelineState = glRenderPass->getPipelineState();
    const auto &glRenderTarget = glPipelineState.getRenderTarget();
    glPipelineState.bind();
    glRenderTarget.bind();
    getState().currentRenderTarget = &glRenderTarget;
}

void GLCommandList::endRenderPass() {
    getState().currentRenderTarget = nullptr;
}

} // namespace NOX
