#include "opengl/gl_render_pass.h"

namespace nox {

bool GLRenderPass::validateInput(const RenderPassDescription &description) {
    bool result = true;

    result &= (!description.attachmentsDescriptions.empty());

    return result;
}

GLRenderPass::GLRenderPass(const RenderPassDescription &description) : m_attachmentsDescriptions{description.attachmentsDescriptions} {}

} // namespace nox
