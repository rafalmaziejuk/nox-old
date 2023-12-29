#pragma once

#include <cstdint>

namespace nox {

class GLProgram {
  public:
    GLProgram();
    ~GLProgram();

    [[nodiscard]] uint32_t getHandle() const { return m_handle; }

    void attachShader(uint32_t shaderHandle) const;
    [[nodiscard]] bool link() const;

  public:
    GLProgram(const GLProgram &) = delete;
    GLProgram &operator=(const GLProgram &) = delete;

  private:
    uint32_t m_handle{0u};
};

} // namespace nox
