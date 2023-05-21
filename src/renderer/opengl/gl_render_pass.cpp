#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_pass.h"
#include "renderer/opengl/gl_render_target.h"

namespace NOX {

GLRenderPass::GLRenderPass(const RenderPassDescriptor &descriptor) : m_pipelineState{descriptor.pipelineState} {}

const GLPipelineState *GLRenderPass::getPipelineState() const {
    return static_cast<const GLPipelineState *>(m_pipelineState.get());
}

bool GLRenderPass::validate() const {
    if (m_pipelineState == nullptr) {
        return false;
    }

    const auto *glPipelineState = getPipelineState();
    if (glPipelineState->getRenderTarget() == nullptr) {
        return false;
    }

    return true;
}

} // namespace NOX
