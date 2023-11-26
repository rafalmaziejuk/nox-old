#pragma once

#include <nox/shader.h>

namespace NOX {

class GLShader final : public Shader {
  public:
    explicit GLShader(const ShaderDescriptor &descriptor);
    ~GLShader() override;

    ShaderType getType() const override { return m_type; }

    uint32_t getHandle() const { return m_handle; }

    bool compile(const char *source) const;

    void accept(ShaderVisitor &visitor) const override;

  private:
    ShaderType m_type;
    uint32_t m_handle{0u};
};

} // namespace NOX
