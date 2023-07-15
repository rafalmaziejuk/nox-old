#pragma once

#include <nox/export.h>
#include <nox/shader_types.h>

namespace NOX {

struct ShaderDescriptor {
    uint8_t stage;
};

class NOX_EXPORT Shader {
  public:
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;
    virtual ~Shader();

  protected:
    Shader() = default;
};

} // namespace NOX
