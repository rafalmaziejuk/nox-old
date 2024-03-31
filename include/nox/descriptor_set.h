#pragma once

#include <nox/export.h>

#include <cstdint>
#include <vector>

namespace nox {

enum class DescriptorType {
    TEXTURE,
    INPUT_ATTACHMENT
};

struct DescriptorSetLayoutBinding {
    uint32_t bindingIndex;
    DescriptorType descriptorType;
    uint32_t descriptorCount;
};
using DescriptorSetLayoutBindings = std::vector<DescriptorSetLayoutBinding>;

struct DescriptorSetLayout {
    DescriptorSetLayoutBindings bindings;
};
using DescriptorSetLayouts = std::vector<DescriptorSetLayout>;

class NOX_EXPORT DescriptorSet {
  public:
    DescriptorSet(const DescriptorSet &) = delete;
    DescriptorSet &operator=(const DescriptorSet &) = delete;
    DescriptorSet(DescriptorSet &&) = delete;
    DescriptorSet &operator=(DescriptorSet &&) = delete;
    virtual ~DescriptorSet() = default;

  protected:
    DescriptorSet() = default;
};

} // namespace nox
