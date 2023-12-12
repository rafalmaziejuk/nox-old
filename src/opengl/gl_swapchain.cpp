#include "nox_assert.h"
#include "opengl/gl_context.h"
#include "opengl/gl_swapchain.h"

namespace nox {

bool GLSwapchain::validateInput(const SwapchainDescriptor &descriptor) {
    return (GLContext::validateInput(descriptor.surfaceDescriptor)) &&
           (std::get_if<OpenGLPresentMode>(&descriptor.presentMode) != nullptr);
}

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor) {
    m_context = GLContext::create(descriptor.surfaceDescriptor);
    NOX_ASSERT(m_context != nullptr);

    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    m_context->setSwapInterval(presentMode->vSync);
}

void GLSwapchain::present() const {
    m_context->swapBuffers();
}

} // namespace nox
