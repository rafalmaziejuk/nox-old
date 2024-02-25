#pragma once

#include <nox/export.h>
#include <nox/buffer.h>
#include <nox/texture.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace nox {

enum class ResourceType {
    TEXTURE,
    INPUT_ATTACHMENT
};

struct TextureResourceDescriptor {
    std::shared_ptr<Texture> texture;
};
using TextureResourceDescriptors = std::vector<TextureResourceDescriptor>;

struct DescriptorSetLayoutBinding {
    uint32_t bindingIndex;
    ResourceType resourceType;
    TextureResourceDescriptors textureResourceDescriptors;
};
using DescriptorSetLayoutBindings = std::vector<DescriptorSetLayoutBinding>;

struct DescriptorSetLayout {
    DescriptorSetLayoutBindings bindings;
};
using DescriptorSetLayouts = std::vector<DescriptorSetLayout>;

struct PipelineLayoutDescriptor {
    DescriptorSetLayouts setLayouts;
};

} // namespace nox
