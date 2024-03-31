#pragma once

#include <nox/descriptor_set.h>
#include <nox/export.h>
#include <nox/format.h>

#include <string>
#include <vector>

namespace nox {

struct UniformDescriptor {
    std::string name;
    UniformDataFormat dataFormat;
};
using UniformDescriptors = std::vector<UniformDescriptor>;

struct PipelineLayoutDescriptor {
    DescriptorSetLayouts setLayouts;
    UniformDescriptors uniformDescriptors;
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
