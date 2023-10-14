#include "renderer/opengl/gl_pipeline_layout.h"
#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_program.h"

#include <glad/gl.h>

namespace NOX {

GLPipelineState::GLPipelineState(const PipelineStateDescriptor &descriptor) : m_pipelineLayout{std::make_unique<GLPipelineLayout>(descriptor.pipelineLayoutDescriptor)},
                                                                              m_renderTarget{std::dynamic_pointer_cast<GLRenderTargetBase>(descriptor.renderTarget)} {
    NOX_LOG_TRACE_DECLARE(OPENGL);
    NOX_ASSERT(m_renderTarget == nullptr);

    GLProgram program{};
    GLbitfield stages = 0u;

    stages |= program.attachShader(descriptor.vertexShader.get());
    stages |= program.attachShader(descriptor.fragmentShader.get());
    program.link();

    glCreateProgramPipelines(1, &m_handle);
    glUseProgramStages(m_handle, stages, program.getHandle());
}

GLPipelineState::~GLPipelineState() {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    glDeleteProgramPipelines(1, &m_handle);
}

const GLPipelineLayout &GLPipelineState::getPipelineLayout() const {
    NOX_LOG_TRACE_DECLARE(OPENGL);

    return *m_pipelineLayout;
}

const GLRenderTargetBase &GLPipelineState::getRenderTarget() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    return *m_renderTarget;
}

void GLPipelineState::bind() const {
    NOX_LOG_TRACE_DECLARE_ONCE(OPENGL);

    glBindProgramPipeline(m_handle);
}

} // namespace NOX
