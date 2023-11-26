#pragma once

#include "shader_visitor.h"

namespace NOX {

class GLShaderVisitor final : public ShaderVisitor {
  public:
    void visit(const GLShader &shader) override {
        m_type = shader.getType();
        m_handle = shader.getHandle();
    }

    ShaderType getType() const { return m_type; }

    uint32_t getHandle() const { return m_handle; }

  private:
    ShaderType m_type;
    uint32_t m_handle{0u};
};

} // namespace NOX
