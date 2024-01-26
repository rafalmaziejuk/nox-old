#include "opengl/gl_render_pass.h"

namespace nox {

bool GLRenderPass::validateInput(const RenderPassDescriptor &descriptor) {
    bool result = true;

    result &= (!descriptor.attachmentDescriptors.empty());

    return result;
}

GLRenderPass::GLRenderPass(const RenderPassDescriptor &descriptor) : m_attachmentDescriptors{descriptor.attachmentDescriptors},
                                                                     m_subpassDescriptors{descriptor.subpassDescriptors} {}

} // namespace nox
