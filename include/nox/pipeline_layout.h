#pragma once

#include <nox/export.h>
#include <nox/texture.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace nox {

enum class DescriptorType {
    NONE,
    TEXTURE,
    MAX
};

struct TextureBindingDescriptor {
    std::shared_ptr<Texture> texture;
};
using TextureBindingDescriptors = std::vector<TextureBindingDescriptor>;

struct DescriptorSetLayoutBindingDescriptor {
    uint32_t bindingIndex;
    DescriptorType descriptorType;
    TextureBindingDescriptors textureBindingDescriptors;
};
using DescriptorSetLayoutBindingDescriptors = std::vector<DescriptorSetLayoutBindingDescriptor>;

struct DescriptorSetLayoutDescriptor {
    DescriptorSetLayoutBindingDescriptors bindingDescriptors;
};
using DescriptorSetLayoutDescriptors = std::vector<DescriptorSetLayoutDescriptor>;

struct PipelineLayoutDescriptor {
    DescriptorSetLayoutDescriptors setLayoutDescriptors;
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
