#pragma once

#include <nox/renderer/shader.h>

namespace NOX {

class GLShader final : public Shader {
  public:
    explicit GLShader(const ShaderDescriptor &descriptor);
    ~GLShader() override;

    ShaderType getType() const { return m_type; }
    uint32_t getHandle() const { return m_handle; }

    void compileFromString(std::string_view source);

  private:
    void compile(const char *source);
    bool checkCompileStatus(ShaderType type) const;

  private:
    uint32_t m_handle{0u};
    ShaderType m_type;
};

} // namespace NOX
