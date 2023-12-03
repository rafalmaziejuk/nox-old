#include "nox_assert.h"
#include "opengl/gl_context.h"
#include "opengl/gl_swapchain.h"

namespace NOX {

bool GLSwapchain::validateInput(const SwapchainDescriptor &descriptor) {
    return (std::get_if<OpenGLPresentMode>(&descriptor.presentMode) != nullptr);
}

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor, std::shared_ptr<GLContext> context) : m_context{std::move(context)} {
    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    m_context->setSwapInterval(presentMode->vSync);
}

void GLSwapchain::present() const {
    m_context->swapBuffers();
}

} // namespace NOX
