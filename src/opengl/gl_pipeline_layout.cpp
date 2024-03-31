#include "opengl/gl_pipeline_layout.h"
#include "opengl/gl_texture.h"

namespace nox {

bool GLPipelineLayout::validateInput(const PipelineLayoutDescriptor &descriptor) {
    bool result = true;

    for (const auto &layout : descriptor.setLayouts) {
        result &= (!layout.bindings.empty());
    }

    return result;
}

GLPipelineLayout::GLPipelineLayout(const PipelineLayoutDescriptor & /*descriptor*/) {}

} // namespace nox
