#pragma once

#include "shader_visitor.h"
#include "opengl/gl_shader.h"

namespace NOX {

class GLShaderVisitor final : public ShaderVisitor {
  public:
    void visit(const GLShader &shader) override {
        m_shader = &shader;
    }

    const GLShader &get() const { return *m_shader; }

  private:
    const GLShader *m_shader{nullptr};
};

} // namespace NOX
