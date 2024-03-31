#include "opengl/gl_descriptor_set_pool.h"

namespace nox {

GLDescriptorSetPool::GLDescriptorSetPool(const DescriptorSetPoolDescriptor &descriptor) {
    m_descriptorSets.reserve(descriptor.setLayouts.size());
    for (const auto &setLayout : descriptor.setLayouts) {
        m_descriptorSets.emplace_back(setLayout);
    }
}

void GLDescriptorSetPool::update(const std::vector<DescriptorSetWriteInfo> &writesInfo) {
}

std::vector<const DescriptorSet *> GLDescriptorSetPool::getDescriptorSets() const {
    std::vector<const DescriptorSet *> descriptorSets(m_descriptorSets.size());
    for (size_t i = 0u; i < m_descriptorSets.size(); i++) {
        descriptorSets[i] = &m_descriptorSets[i];
    }
    return descriptorSets;
}

} // namespace nox
