#pragma once

#include <nox/shader.h>

namespace NOX {

class GLShader;

class ShaderVisitor {
  public:
    virtual void visit(const GLShader &shader) = 0;

  public:
    ShaderVisitor() = default;
    ShaderVisitor(const ShaderVisitor &) = delete;
    ShaderVisitor &operator=(const ShaderVisitor &) = delete;
    virtual ~ShaderVisitor() = default;
};

} // namespace NOX
