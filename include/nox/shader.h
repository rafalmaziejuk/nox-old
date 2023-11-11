#pragma once

#include <nox/export.h>

#include <cstdint>
#include <string>

namespace NOX {

enum class ShaderType : uint8_t {
    VERTEX,
    FRAGMENT,
    TESS_CONTROL,
    TESS_EVALUATION,
    GEOMETRY,
    COMPUTE,
    MAX
};

struct ShaderDescriptor {
    std::string name;
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

struct ShaderHandle {
    size_t id;
    bool isValid;
};

class NOX_EXPORT ShaderRegistry {
  public:
    [[nodiscard]] virtual ShaderHandle registerShader(const ShaderDescriptor &descriptor, std::string_view source) = 0;
    virtual void unregisterShader(ShaderHandle &handle) = 0;

    [[nodiscard]] virtual bool contains(const ShaderHandle &handle) const = 0;

    [[nodiscard]] virtual Shader &operator[](const ShaderHandle &handle) = 0;
    [[nodiscard]] virtual const Shader &operator[](const ShaderHandle &handle) const = 0;

  public:
    ShaderRegistry(const ShaderRegistry &) = delete;
    ShaderRegistry &operator=(const ShaderRegistry &) = delete;
    ShaderRegistry(ShaderRegistry &&) = delete;
    ShaderRegistry &operator=(ShaderRegistry &&) = delete;
    virtual ~ShaderRegistry() = default;

  protected:
    ShaderRegistry() = default;
};

} // namespace NOX
