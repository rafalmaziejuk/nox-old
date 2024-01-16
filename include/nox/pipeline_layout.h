#pragma once

#include <nox/export.h>
#include <nox/buffer.h>
#include <nox/texture.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace nox {

enum class ResourceType {
    TEXTURE
};

struct TextureResourceDescription {
    std::shared_ptr<Texture> texture;
};
using TextureResourceDescriptions = std::vector<TextureResourceDescription>;

struct DescriptionSetLayoutBinding {
    uint32_t bindingIndex;
    ResourceType resourceType;
    TextureResourceDescriptions textureResourceDescriptions;
};
using DescriptionSetLayoutBindings = std::vector<DescriptionSetLayoutBinding>;

struct DescriptionSetLayout {
    DescriptionSetLayoutBindings bindings;
};
using DescriptionSetLayouts = std::vector<DescriptionSetLayout>;

struct PipelineLayoutDescription {
    DescriptionSetLayouts setLayouts;
};

class NOX_EXPORT PipelineLayout {
  public:
    PipelineLayout(const PipelineLayout &) = delete;
    PipelineLayout &operator=(const PipelineLayout &) = delete;
    PipelineLayout(PipelineLayout &&) = delete;
    PipelineLayout &operator=(PipelineLayout &&) = delete;
    virtual ~PipelineLayout() = default;

  protected:
    PipelineLayout() = default;
};

} // namespace nox
