#include "opengl/gl_pipeline_layout.h"

namespace nox {

std::unique_ptr<GLPipelineLayout> GLPipelineLayout::create(const PipelineLayoutDescriptor &descriptor) {
    auto layout = std::unique_ptr<GLPipelineLayout>(new GLPipelineLayout{descriptor});
    return layout;
}

GLPipelineLayout::GLPipelineLayout(const PipelineLayoutDescriptor &descriptor) {
    for (const auto &setLayoutDescriptor : descriptor.setLayoutDescriptors) {
        m_descriptorSets.emplace_back(setLayoutDescriptor);
    }
}

} // namespace nox
