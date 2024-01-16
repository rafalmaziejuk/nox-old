#pragma once

#include <nox/shader.h>

#include <string_view>

namespace nox {

class GLShader final : public Shader {
  public:
    [[nodiscard]] static bool validateInput(const ShaderDescription &description, std::string_view source);

    explicit GLShader(const ShaderDescription &description);
    ~GLShader() override;

    ShaderType getType() const override { return m_type; }

    [[nodiscard]] uint32_t getHandle() const { return m_handle; }

    bool compile(const char *source) const;

  private:
    ShaderType m_type;
    uint32_t m_handle{0u};
};

} // namespace nox
