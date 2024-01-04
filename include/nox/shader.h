#pragma once

#include <nox/export.h>

#include <cstdint>

namespace nox {

enum class ShaderType {
    VERTEX,
    FRAGMENT,
    TESS_CONTROL,
    TESS_EVALUATION,
    GEOMETRY,
    COMPUTE,
    MAX
};

struct ShaderDescriptor {
    ShaderType type;
};

class NOX_EXPORT Shader {
  public:
    virtual ShaderType getType() const = 0;

  public:
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;
    Shader(Shader &&) = delete;
    Shader &operator=(Shader &&) = delete;
    virtual ~Shader() = default;

  protected:
    Shader() = default;
};

} // namespace nox
