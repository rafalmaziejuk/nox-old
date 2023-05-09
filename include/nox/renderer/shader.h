#pragma once

#include <nox/export.h>
#include <nox/non_copyable.h>
#include <nox/renderer/shader_types.h>

namespace NOX {

struct ShaderDescriptor {
    ShaderType type;
};

class NOX_EXPORT Shader : public NonCopyable {
  public:
    virtual ~Shader();
};

} // namespace NOX
