#pragma once

#include <nox/descriptor_set.h>
#include <nox/export.h>
#include <nox/texture.h>

#include <cstdint>
#include <vector>

namespace nox {

struct DescriptorSetPoolDescriptor {
    DescriptorSetLayouts setLayouts;
};

struct DescriptorSetWriteInfo {
    uint32_t bindingIndex;
    uint32_t arrayIndex;
    uint32_t descriptorSetIndex;
    DescriptorType descriptorType;
    std::vector<const Texture *> textures;
};

class NOX_EXPORT DescriptorSetPool {
  public:
    virtual void update(const std::vector<DescriptorSetWriteInfo> &writesInfo) = 0;

    [[nodiscard]] virtual std::vector<const DescriptorSet *> getDescriptorSets() const = 0;

  public:
    DescriptorSetPool(const DescriptorSetPool &) = delete;
    DescriptorSetPool &operator=(const DescriptorSetPool &) = delete;
    DescriptorSetPool(DescriptorSetPool &&) = delete;
    DescriptorSetPool &operator=(DescriptorSetPool &&) = delete;
    virtual ~DescriptorSetPool() = default;

  protected:
    DescriptorSetPool() = default;
};

} // namespace nox
