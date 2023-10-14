#include "renderer/opengl/gl_pipeline_layout.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_texture.h"

namespace NOX {

GLRenderPass::GLRenderPass(const RenderPassDescriptor &descriptor) : m_pipelineState{std::dynamic_pointer_cast<GLPipelineState>(descriptor.pipelineState)} {
    NOX_LOG_TRACE_DECLARE(OPENGL);
    NOX_ASSERT(m_pipelineState == nullptr);
}

const GLPipelineState &GLRenderPass::getPipelineState() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    return *m_pipelineState;
}

void GLRenderPass::setInput(uint32_t binding, const Resource &resource) {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    const auto &pipelineLayout = m_pipelineState->getPipelineLayout();
    switch (pipelineLayout[binding]) {
    case ResourceType::TEXTURE: {
        const auto *glTexture = downcast<GLTexture>(resource);
        glTexture->bind(binding);
        break;
    }

    default:
        NOX_ASSERT(true);
        break;
    }
}

} // namespace NOX
