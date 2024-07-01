#include "asserts.h"
#include "opengl/gl_context.h"
#include "opengl/gl_swapchain.h"

namespace nox {

std::unique_ptr<GLSwapchain> GLSwapchain::create(const SwapchainDescriptor &descriptor,
                                                 std::unique_ptr<GLContext> context) {
    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    NOX_ENSURE_RETURN_NULLPTR(presentMode != nullptr);
    context->setSwapInterval(presentMode->vSync);

    return std::make_unique<GLSwapchain>(descriptor, std::move(context));
}

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor,
                         std::unique_ptr<GLContext> context)
    : m_context{std::move(context)},
      m_size{descriptor.size} {
}

Vector2D<uint32_t> GLSwapchain::getSize() const {
    return m_size;
}

ImageFormat GLSwapchain::getSurfaceFormat() const {
    return ImageFormat::NONE;
}

std::vector<const Texture *> GLSwapchain::getSwapchainTextures() const {
    return {};
}

void GLSwapchain::present() const {
    m_context->makeCurrent();
    m_context->swapBuffers();
}

} // namespace nox
