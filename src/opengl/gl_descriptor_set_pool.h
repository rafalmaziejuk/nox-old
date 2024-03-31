#pragma once

#include <nox/descriptor_set_pool.h>

#include "opengl/gl_descriptor_set.h"

namespace nox {

class GLDescriptorSetPool final : public DescriptorSetPool {
  public:
    explicit GLDescriptorSetPool(const DescriptorSetPoolDescriptor &descriptor);

    void update(const std::vector<DescriptorSetWriteInfo> &writesInfo) override;

    std::vector<const DescriptorSet *> getDescriptorSets() const override;

  private:
    std::vector<GLDescriptorSet> m_descriptorSets;
};

} // namespace nox
