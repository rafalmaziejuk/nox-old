#pragma once

#include <unordered_map>

namespace NOX {

struct PipelineLayoutDescriptor;
enum class ResourceType : uint8_t;

class GLPipelineLayout {
  public:
    explicit GLPipelineLayout(const PipelineLayoutDescriptor &descriptor);

    ResourceType operator[](uint32_t binding) const;

  private:
    std::unordered_map<uint32_t, ResourceType> m_bindingResourceTypes;
};

} // namespace NOX
