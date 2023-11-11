#include "renderer/opengl/gl_pipeline_state.h"
#include "renderer/opengl/gl_render_target.h"
#include "renderer/opengl/gl_program.h"

#include <glad/gl.h>

namespace NOX {

GLPipelineState::GLPipelineState(const PipelineStateDescriptor &descriptor) {
    GLProgram program{};
    GLbitfield stages = 0u;

    stages |= program.attachShader(descriptor.vertexShader.get());
    stages |= program.attachShader(descriptor.fragmentShader.get());
    program.link();

    glCreateProgramPipelines(1, &m_handle);
    glUseProgramStages(m_handle, stages, program.getHandle());
}

GLPipelineState::~GLPipelineState() {
    glDeleteProgramPipelines(1, &m_handle);
}

void GLPipelineState::bind() {
    glBindProgramPipeline(m_handle);
}

} // namespace NOX
