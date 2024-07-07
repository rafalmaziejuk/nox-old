#pragma once

#include "opengl/gl_descriptor_set.h"

#include <nox/pipeline_layout.h>

namespace nox {

class GLPipelineLayout final : public PipelineLayout {
  public:
    [[nodiscard]] static std::unique_ptr<GLPipelineLayout> create(const PipelineLayoutDescriptor &descriptor);

    [[nodiscard]] const std::vector<GLDescriptorSet> &getDescriptorSets() const {
        return m_descriptorSets;
    }

  private:
    explicit GLPipelineLayout(const PipelineLayoutDescriptor &descriptor);

  private:
    std::vector<GLDescriptorSet> m_descriptorSets;
};

} // namespace nox
