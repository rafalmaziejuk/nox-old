#pragma once

#include <nox/pipeline_layout.h>

namespace nox {

class GLPipelineLayout final : public PipelineLayout {
  public:
    [[nodiscard]] static bool validateInput(const PipelineLayoutDescriptor &descriptor);

    explicit GLPipelineLayout(const PipelineLayoutDescriptor &descriptor);
};

} // namespace nox
