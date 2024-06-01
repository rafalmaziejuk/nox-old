#pragma once

#include <nox/shader.h>

#include <memory>

namespace nox {

class GLShader final : public Shader {
  public:
    [[nodiscard]] static std::unique_ptr<GLShader> create(const ShaderDescriptor &descriptor);

    explicit GLShader(const ShaderDescriptor &descriptor);
    ~GLShader() override;

    ShaderType getType() const override { return m_type; }

    [[nodiscard]] uint32_t getHandle() const { return m_handle; }

    [[nodiscard]] bool compile(const char *source) const;

  private:
    ShaderType m_type{ShaderType::NONE};
    uint32_t m_handle{0u};
};

} // namespace nox
