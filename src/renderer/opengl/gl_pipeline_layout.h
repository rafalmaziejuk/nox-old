#pragma once

#include <nox/pipeline_state.h>

#include <unordered_map>

namespace NOX {

struct PipelineLayoutDescriptor;

class GLPipelineLayout {
  public:
    explicit GLPipelineLayout(const PipelineLayoutDescriptor &descriptor);

    ResourceType operator[](uint32_t binding) const;

  private:
    std::unordered_map<uint32_t, ResourceType> m_bindingResourceTypes;
};

} // namespace NOX
