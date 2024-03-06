#include "asserts.h"
#include "opengl/gl_context.h"
#include "opengl/gl_program.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_swapchain.h"
#include "opengl/gl_texture.h"

#include <glad/gl.h>

namespace nox {

namespace {

ImageFormat queryDefaultFramebufferImageFormat() {
    constexpr GLuint defaultFramebufferHandle = 0u;

    GLint alphaBits = 0;
    glGetNamedFramebufferAttachmentParameteriv(defaultFramebufferHandle, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alphaBits);

    GLint componentType = 0;
    glGetNamedFramebufferAttachmentParameteriv(defaultFramebufferHandle, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE, &componentType);
    NOX_ASSERT(componentType != GL_NONE);

    switch (componentType) {
    case GL_INT:
        return ((alphaBits != 0) ? ImageFormat::RGBA8I : ImageFormat::RGB8I);

    case GL_UNSIGNED_INT:
        return ((alphaBits != 0) ? ImageFormat::RGBA8UI : ImageFormat::RGB8UI);

    case GL_SIGNED_NORMALIZED:
        return ((alphaBits != 0) ? ImageFormat::RGBA8_SNORM : ImageFormat::RGB8_SNORM);

    case GL_UNSIGNED_NORMALIZED:
        return ((alphaBits != 0) ? ImageFormat::RGBA8_UNORM : ImageFormat::RGB8_UNORM);

    default:
        NOX_ASSERT(false);
        return {};
    }
}

constexpr auto presentVertexShaderSource = R"(
            #version 460 core

            out gl_PerVertex {
	            vec4 gl_Position;
            };

		    out vec2 textureCoordinates;

		    void main() {
                const vec2 positions[4] = vec2[](
                    vec2(-1.0, -1.0),
                    vec2( 1.0, -1.0),
                    vec2(-1.0,  1.0),
                    vec2( 1.0,  1.0)
                );
                const vec2 coordinates[4] = vec2[](
                    vec2(0.0, 0.0),
                    vec2(1.0, 0.0),
                    vec2(0.0, 1.0),
                    vec2(1.0, 1.0)
                );

			    textureCoordinates = coordinates[gl_VertexID];
			    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0); 
		    }
        )";

constexpr auto presentFragmentShaderSource = R"(
            #version 460 core

            layout(binding = 0) uniform sampler2D fullscreenTexture;

            in vec2 textureCoordinates;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = texture(fullscreenTexture, textureCoordinates);
            }
        )";

} // namespace

bool GLSwapchain::validateInput(const SwapchainDescriptor &descriptor) {
    bool result = true;

    result &= GLContext::validateInput(descriptor.surfaceDescriptor);
    result &= (std::get_if<OpenGLPresentMode>(&descriptor.presentMode) != nullptr);

    return result;
}

GLSwapchain::GLSwapchain(const SwapchainDescriptor &descriptor, std::unique_ptr<GLContext> context) : m_context{std::move(context)},
                                                                                                      m_swapchainTexture{{{queryDefaultFramebufferImageFormat()},
                                                                                                                          descriptor.size}},
                                                                                                      m_size{descriptor.size} {
    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    m_context->setSwapInterval(presentMode->vSync);

    const GLShader presentVertexShader{{ShaderType::VERTEX}};
    auto result = presentVertexShader.compile(presentVertexShaderSource);
    NOX_ASSERT(result);

    const GLShader presentFragmentShader{{ShaderType::FRAGMENT}};
    result = presentFragmentShader.compile(presentFragmentShaderSource);
    NOX_ASSERT(result);

    m_presentProgram.attachShader(presentVertexShader.getHandle());
    m_presentProgram.attachShader(presentFragmentShader.getHandle());
    result = m_presentProgram.link();
    NOX_ASSERT(result);
}

std::vector<const Texture *> GLSwapchain::getSwapchainTextures() const {
    return {1u, &m_swapchainTexture};
}

void GLSwapchain::present() const {
    m_swapchainTexture.bind(0u);
    m_emptyVertexArray.bind();

    m_presentProgram.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_presentProgram.unbind();

    m_context->swapBuffers();
}

ImageFormat GLSwapchain::getSurfaceFormat() const {
    return queryDefaultFramebufferImageFormat();
}

Vector2D<uint32_t> GLSwapchain::getSize() const {
    return m_size;
}

} // namespace nox
