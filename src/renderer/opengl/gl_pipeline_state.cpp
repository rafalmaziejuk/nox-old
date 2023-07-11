#include "renderer/opengl/gl_helper.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_program.h"

namespace NOX {

GLPipelineState::GLPipelineState(const PipelineStateDescriptor &descriptor) : m_renderTarget{descriptor.renderTarget} {
    GLProgram program{};
    auto vertexShaderBit = program.attachShader(descriptor.vertexShader.get());
    auto fragmentShaderBit = program.attachShader(descriptor.fragmentShader.get());
    auto stages = vertexShaderBit | fragmentShaderBit;
    program.link();

    glCreateProgramPipelines(1, &m_handle);
    glUseProgramStages(m_handle, stages, program.getHandle());
}

GLPipelineState::~GLPipelineState() {
    glDeleteProgramPipelines(1, &m_handle);
}

const GLRenderTargetBase *GLPipelineState::getRenderTarget() const {
    return downcast<GLRenderTargetBase>(*m_renderTarget);
}

void GLPipelineState::bind() const {
    glBindProgramPipeline(m_handle);
}

} // namespace NOX
