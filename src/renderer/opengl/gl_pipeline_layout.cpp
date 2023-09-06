#include "renderer/opengl/gl_pipeline_layout.h"

#include <nox/pipeline_state.h>

namespace NOX {

GLPipelineLayout::GLPipelineLayout(const PipelineLayoutDescriptor &descriptor) {
    m_bindingResourceTypes.reserve(descriptor.bindingDescriptors.size());
    for (const auto &bindingDescriptor : descriptor.bindingDescriptors) {
        m_bindingResourceTypes[bindingDescriptor.binding] = bindingDescriptor.resourceType;
    }
}

ResourceType GLPipelineLayout::operator[](uint32_t binding) const {
    const auto &it = m_bindingResourceTypes.find(binding);
    NOX_ASSERT(it == m_bindingResourceTypes.end());
    return it->second;
}

} // namespace NOX
