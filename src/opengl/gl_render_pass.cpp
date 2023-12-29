#include "opengl/gl_render_pass.h"
#include "opengl/gl_render_pass_visitor.h"

namespace nox {

bool GLRenderPass::validateInput(const RenderPassDescriptor &descriptor) {
    bool result = true;

    result &= (!descriptor.attachmentsDescriptors.empty());

    return result;
}

bool GLRenderPass::validateInput(const RenderPassBeginDescriptor &descriptor) {
    bool result = true;

    result &= (descriptor.framebuffer != nullptr);
    result &= (descriptor.renderPass != nullptr);
    result &= (!descriptor.clearValues.empty());

    return result;
}

GLRenderPass::GLRenderPass(const RenderPassDescriptor &descriptor) : m_attachmentsDescriptors{descriptor.attachmentsDescriptors} {}

void GLRenderPass::accept(RenderPassVisitor &visitor) const {
    visitor.visit(*this);
}

} // namespace nox
