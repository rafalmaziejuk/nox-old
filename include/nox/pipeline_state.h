#pragma once

#include <nox/export.h>
#include <nox/render_target.h>
#include <nox/shader.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace NOX {

enum class ResourceType : uint8_t {
    NONE,
    TEXTURE
};

struct BindingDescriptor {
    uint32_t set;
    uint32_t binding;
    uint32_t shaderStages;
    ResourceType resourceType;
};

struct PipelineLayoutDescriptor {
    std::vector<BindingDescriptor> bindingDescriptors;
};

enum class PrimitiveTopology : uint8_t {
    NONE,
    TRIANGLE_LIST
};

struct PipelineStateDescriptor {
    PipelineLayoutDescriptor pipelineLayoutDescriptor;
    std::unique_ptr<Shader> vertexShader;
    std::unique_ptr<Shader> fragmentShader;
    std::shared_ptr<RenderTarget> renderTarget;

    PrimitiveTopology primitiveTopology;
};

class NOX_EXPORT PipelineState {
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
