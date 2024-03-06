#include "asserts.h"
#include "opengl/gl_buffer.h"
#include "opengl/gl_command_list.h"
#include "opengl/gl_framebuffer.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_render_pass.h"
#include "opengl/gl_state.h"

#include <glad/gl.h>

namespace nox {

bool GLCommandList::validateInput(const RenderPassBeginDescriptor &descriptor) {
    bool result = true;

    result &= (descriptor.framebuffer != nullptr);
    result &= (descriptor.renderPass != nullptr);
    result &= (!descriptor.clearValues.empty());

    return result;
}

GLCommandList::GLCommandList(const CommandListDescriptor & /*descriptor*/,
                             GLState &state) : GLWithState{state} {}

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
    state->currentFramebuffer = glFramebuffer;
    state->currentRenderPass = glRenderPass;

    const auto &attachmentDescriptors = glRenderPass->getAttachmentDescriptors();
    glFramebuffer->bind();
    glFramebuffer->clearAttachments(descriptor.clearValues, attachmentDescriptors);
    glFramebuffer->invalidateAttachments(attachmentDescriptors);
}

void GLCommandList::endRenderPass() {
    state->currentFramebuffer->unbind();

    state->currentFramebuffer = nullptr;
    state->currentRenderPass = nullptr;
}

void GLCommandList::draw(uint32_t firstVertexIndex, uint32_t vertexCount) {
    glDrawArrays(state->primitiveTopology,
                 static_cast<GLint>(firstVertexIndex),
                 static_cast<GLsizei>(vertexCount));
}

void GLCommandList::drawIndexed(uint32_t /*firstVertexIndex*/, uint32_t vertexCount) {
    glDrawElements(state->primitiveTopology,
                   static_cast<GLsizei>(vertexCount),
                   state->indexType,
                   nullptr);
}

} // namespace nox
