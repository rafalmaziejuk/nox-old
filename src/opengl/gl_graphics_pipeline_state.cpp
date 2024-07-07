#include "asserts.h"
#include "opengl/gl_graphics_pipeline_state.h"
#include "opengl/gl_shader.h"

#include <glad/gl.h>

namespace nox {

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

    return GL_NONE;
}

GLenum mapPrimitiveTopologyToEnum(PrimitiveTopology topology) {
    switch (topology) {
    case PrimitiveTopology::TRIANGLE_LIST: return GL_TRIANGLES;

    default: break;
    }

    return GL_NONE;
}

} // namespace

std::unique_ptr<GLGraphicsPipelineState> GLGraphicsPipelineState::create(const GraphicsPipelineStateDescriptor &descriptor) {
    auto pipeline = std::unique_ptr<GLGraphicsPipelineState>(new GLGraphicsPipelineState{descriptor});
    NOX_ENSURE_RETURN_NULLPTR_MSG(pipeline->bindShaderStages(descriptor.shaderStages),
                                  "Couldn't bind graphics pipeline shader stages");

    return pipeline;
}

GLGraphicsPipelineState::GLGraphicsPipelineState(const GraphicsPipelineStateDescriptor &descriptor)
    : m_subpassIndex{descriptor.subpassIndex},
      m_primitiveTopology{mapPrimitiveTopologyToEnum(descriptor.primitiveTopology)} {
    glCreateProgramPipelines(1, &m_handle);
}

GLGraphicsPipelineState::~GLGraphicsPipelineState() {
    glDeleteProgramPipelines(1, &m_handle);
}

void GLGraphicsPipelineState::bind() const {
    glBindProgramPipeline(m_handle);
}

bool GLGraphicsPipelineState::bindShaderStages(const ShaderStages &shaderStages) const {
    GLbitfield stages = GL_NONE;
    for (const auto *shader : shaderStages) {
        const auto *glShader = static_cast<const GLShader *>(shader);
        m_program.attachShader(glShader->getHandle());
        stages |= mapShaderTypeToBitfield(glShader->getType());
    }

    if (!m_program.link()) {
        return false;
    }

    glUseProgramStages(m_handle, stages, m_program.getHandle());
    return true;
}

} // namespace nox
