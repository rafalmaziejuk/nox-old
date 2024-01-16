#include "nox_assert.h"
#include "opengl/gl_context.h"
#include "opengl/gl_swapchain.h"

#include <glad/gl.h>

namespace nox {

namespace {

ImageFormat queryDefaultFramebufferImageFormat() {
    constexpr GLuint defaultFramebufferHandle = 0u;
    ImageFormat result = ImageFormat::RGBA8_UNORM;

    GLint alphaBits = 0;
    glGetNamedFramebufferAttachmentParameteriv(defaultFramebufferHandle, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alphaBits);
    if (alphaBits == 0) {
        result = ImageFormat::RGB8_UNORM;
    }

    return result;
}

} // namespace

bool GLSwapchain::validateInput(const SwapchainDescription &description) {
    bool result = true;

    result &= GLContext::validateInput(description.surfaceDescription);
    result &= (std::get_if<OpenGLPresentMode>(&description.presentMode) != nullptr);

    return result;
}

GLSwapchain::GLSwapchain(const SwapchainDescription &description) : m_size{description.size} {
    m_context = GLContext::create(description.surfaceDescription);
    NOX_ASSERT(m_context != nullptr);

    const auto *presentMode = std::get_if<OpenGLPresentMode>(&description.presentMode);
    m_context->setSwapInterval(presentMode->vSync);

    Texture2DDescription presentableTextureDescription{};
    presentableTextureDescription.format = queryDefaultFramebufferImageFormat();
    presentableTextureDescription.size = getSize();
    m_presentableTexture = std::make_unique<GLTexture2D>(presentableTextureDescription);
    m_presentableTexture->setPresentable();
}

std::vector<const Texture *> GLSwapchain::getPresentableTextures() const {
    return {1u, m_presentableTexture.get()};
}

void GLSwapchain::present() const {
    m_context->swapBuffers();
}

ImageFormat GLSwapchain::getSurfaceFormat() const {
    return queryDefaultFramebufferImageFormat();
}

Vector2D<uint32_t> GLSwapchain::getSize() const {
    return m_size;
}

} // namespace nox
