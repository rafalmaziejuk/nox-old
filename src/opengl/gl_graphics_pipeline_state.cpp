#include "assertion.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_program.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_shader_visitor.h"

#include <glad/gl.h>

namespace NOX {

namespace {

GLbitfield mapShaderTypeToBitfield(ShaderType type) {
    switch (type) {
    case ShaderType::VERTEX: return GL_VERTEX_SHADER_BIT;
    case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER_BIT;
    case ShaderType::TESS_CONTROL: return GL_TESS_CONTROL_SHADER_BIT;
    case ShaderType::TESS_EVALUATION: return GL_TESS_EVALUATION_SHADER_BIT;
    case ShaderType::GEOMETRY: return GL_GEOMETRY_SHADER_BIT;
    case ShaderType::COMPUTE: return GL_COMPUTE_SHADER_BIT;

    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

GLenum mapPrimitiveTopologyToEnum(PrimitiveTopology topology) {
    switch (topology) {
    case PrimitiveTopology::TRIANGLE_LIST: return GL_TRIANGLES;

    default: break;
    }

    NOX_ASSERT(false);
    return GL_NONE;
}

} // namespace

GLGraphicsPipelineState::GLGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor, GLState &state) : GLWithState{state},
                                                                                                                      m_primitiveTopology{mapPrimitiveTopologyToEnum(descriptor.primitiveTopology)} {
    glCreateProgramPipelines(1, &m_handle);
}

GLGraphicsPipelineState::~GLGraphicsPipelineState() {
    glDeleteProgramPipelines(1, &m_handle);
}

void GLGraphicsPipelineState::bind() {
    auto &state = getState();
    state.primitiveTopology = m_primitiveTopology;

    glBindProgramPipeline(m_handle);
}

bool GLGraphicsPipelineState::bindShaderStages(const ShaderStages &shaderStages) {
    GLbitfield stages = GL_NONE;
    for (const auto &shader : shaderStages) {
        GLShaderVisitor visitor{};
        shader->accept(visitor);

        m_program.attachShader(visitor.getHandle());
        stages |= mapShaderTypeToBitfield(visitor.getType());
    }

    if (!m_program.link()) {
        return false;
    }

    glUseProgramStages(m_handle, stages, m_program.getHandle());
    return true;
}

} // namespace NOX
