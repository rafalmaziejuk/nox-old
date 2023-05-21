#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_texture.h"

namespace NOX {

GLRenderPass::GLRenderPass(const RenderPassDescriptor &descriptor) : m_pipelineState{descriptor.pipelineState} {}

void GLRenderPass::setInput(uint32_t index, const Texture &texture) {
    const auto *glTexture = downcast<GLTexture>(texture);
    glTexture->bind(index);
}

const GLPipelineState *GLRenderPass::getPipelineState() const {
    return downcast<GLPipelineState>(*m_pipelineState);
}

} // namespace NOX
