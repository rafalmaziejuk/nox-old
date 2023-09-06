#pragma once

#include <nox/export.h>

#include <cstdint>

namespace NOX {

struct ShaderStage {
    enum {
        VERTEX = (1 << 0),
        FRAGMENT = (1 << 1)
    };
};

struct ShaderDescriptor {
    uint32_t stage;
};

class NOX_EXPORT Shader {
  public:
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;
    Shader(Shader &&) = delete;
    Shader &operator=(Shader &&) = delete;
    virtual ~Shader() = default;

  protected:
    Shader() = default;
};

} // namespace NOX
