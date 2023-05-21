#pragma once

#include <vector>

namespace NOX {

class Shader;

class GLProgram {
  public:
    GLProgram();
    ~GLProgram();

    uint32_t getHandle() const { return m_handle; }

    uint32_t attachShader(const Shader *shader);
    void link();

  private:
    void detachShaders();
    bool checkLinkStatus();

  private:
    std::vector<uint32_t> m_attachedShadersHandles;
    uint32_t m_handle{0u};
};

} // namespace NOX
