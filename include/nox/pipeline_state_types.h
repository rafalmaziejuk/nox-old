#pragma once

#include <cstdint>

namespace NOX {

enum class ResourceType : uint8_t {
    NONE,
    TEXTURE,
};

struct BindingDescriptor {
    uint32_t set;
    uint32_t binding;
    uint32_t shaderStages;
    ResourceType resourceType;
};

struct PipelineLayoutDescriptor {
    std::vector<BindingDescriptor> bindingDescriptors;
};

enum class PrimitiveTopology : uint8_t {
    NONE,
    TRIANGLE_LIST
};

} // namespace NOX
