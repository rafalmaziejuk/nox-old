#pragma once

#include "renderer/opengl/gl_object.h"

#include <nox/renderer/shader.h>

namespace NOX {

class GLShader final : public Shader, public GLObject {
  public:
    explicit GLShader(const ShaderDescriptor &descriptor);
    ~GLShader() override;

    ShaderType getType() const;

    void compileFromString(std::string_view source);

  private:
    void compile(const char *source);
    bool checkCompileStatus(ShaderType type) const;

  private:
    ShaderType m_type{0u};
};

} // namespace NOX
