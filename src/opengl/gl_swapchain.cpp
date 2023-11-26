#include "opengl/gl_context.h"
#include "opengl/gl_swapchain.h"

#include <nox/config.h>

namespace NOX {

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor, std::shared_ptr<GLContext> context) : m_context{std::move(context)} {
    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    NOX_ASSERT(presentMode != nullptr);

    m_context->setSwapInterval(presentMode->vSync);
}

void GLSwapchain::present() const {
    m_context->swapBuffers();
}

} // namespace NOX
