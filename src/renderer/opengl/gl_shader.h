#pragma once

#include <nox/shader.h>

namespace NOX {

class GLShader final : public Shader {
  public:
    explicit GLShader(const ShaderDescriptor &descriptor);
    ~GLShader() override;

    uint32_t getHandle() const { return m_handle; }
    uint32_t getStageBit() const { return m_stageBit; }

    void compileFromString(std::string_view source) const;

  private:
    void compile(const char *source) const;

  private:
    uint32_t m_stageBit{0u};
    uint32_t m_handle{0u};
};

} // namespace NOX
