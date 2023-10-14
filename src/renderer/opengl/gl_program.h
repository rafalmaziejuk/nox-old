#pragma once

#include <vector>

namespace NOX {

class Shader;

class GLProgram {
  public:
    GLProgram(const GLProgram &) = delete;
    GLProgram &operator=(const GLProgram &) = delete;

  public:
    GLProgram();
    ~GLProgram();

    uint32_t getHandle() const;

    uint32_t attachShader(const Shader *shader);
    void link();

  private:
    void detachShaders();
    bool checkLinkStatus();

  private:
    std::vector<uint32_t> m_attachedShaderHandles;
    uint32_t m_handle{0u};
};

} // namespace NOX
