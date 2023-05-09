#pragma once

#include "renderer/opengl/gl_object.h"

#include <vector>

namespace NOX {

class Shader;

class GLProgram final : public GLObject {
  public:
    GLProgram();
    ~GLProgram();

    uint32_t attachShader(const Shader *shader);
    void link();

  private:
    void detachShaders();
    bool checkLinkStatus();

  private:
    std::vector<uint32_t> m_attachedShadersHandles;
};

} // namespace NOX
