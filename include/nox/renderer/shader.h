#pragma once

#include <nox/export.h>
#include <nox/renderer/shader_types.h>

namespace NOX {

struct ShaderDescriptor {
    ShaderType type;
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
