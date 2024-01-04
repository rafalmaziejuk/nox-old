#include "opengl/gl_render_pass.h"

namespace nox {

bool GLRenderPass::validateInput(const RenderPassDescriptor &descriptor) {
    bool result = true;

    result &= (!descriptor.attachmentsDescriptors.empty());

    return result;
}

GLRenderPass::GLRenderPass(const RenderPassDescriptor &descriptor) : m_attachmentsDescriptors{descriptor.attachmentsDescriptors} {}

} // namespace nox
