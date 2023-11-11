#pragma once

#include <nox/shader.h>

#include <memory>
#include <unordered_map>

namespace NOX {

class GLShader final : public Shader {
  public:
    explicit GLShader(const ShaderDescriptor &descriptor);
    ~GLShader() override;

    ShaderType getType() const override { return m_type; }

    uint32_t getHandle() const { return m_handle; }

    bool compile(const char *source) const;

  private:
    ShaderType m_type;
    uint32_t m_handle{0u};
};

class GLShaderRegistry final : public ShaderRegistry {
  public:
    ShaderHandle registerShader(const ShaderDescriptor &descriptor, std::string_view source) override;
    void unregisterShader(ShaderHandle &handle) override;

    bool contains(const ShaderHandle &handle) const override;

    Shader &operator[](const ShaderHandle &handle) override;
    const Shader &operator[](const ShaderHandle &handle) const override;

    GLShader &getShader(const ShaderHandle &handle);
    const GLShader &getShader(const ShaderHandle &handle) const;

  private:
    std::unordered_map<size_t, std::unique_ptr<GLShader>> m_shaders;
};

} // namespace NOX
