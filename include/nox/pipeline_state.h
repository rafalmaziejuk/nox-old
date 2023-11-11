#pragma once

#include <nox/export.h>
#include <nox/render_target.h>
#include <nox/shader.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace NOX {

enum class PrimitiveTopology : uint8_t {
    NONE,
    TRIANGLE_LIST
};

struct PipelineStateDescriptor {
    std::unique_ptr<Shader> vertexShader;
    std::unique_ptr<Shader> fragmentShader;

    PrimitiveTopology primitiveTopology;
};

class NOX_EXPORT PipelineState {
  public:
    virtual void bind() = 0;

  public:
    PipelineState(const PipelineState &) = delete;
    PipelineState &operator=(const PipelineState &) = delete;
    PipelineState(PipelineState &&) = delete;
    PipelineState &operator=(PipelineState &&) = delete;
    virtual ~PipelineState() = default;

  protected:
    PipelineState() = default;
};

} // namespace NOX
