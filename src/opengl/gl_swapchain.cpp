#include "asserts.h"
#include "opengl/gl_context.h"
#include "opengl/gl_program.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_swapchain.h"
#include "opengl/gl_texture.h"
#include "opengl/gl_vertex_array.h"

#include <glad/gl.h>

namespace nox {

namespace {

constexpr auto vertexShaderSource = R"(
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

constexpr auto fragmentShaderSource = R"(
            #version 460 core

            layout(binding = 0) uniform sampler2D fullscreenTexture;

            in vec2 textureCoordinates;
            out vec4 fragmentColor;

            void main() {
                fragmentColor = texture(fullscreenTexture, textureCoordinates);
            }
        )";

ImageFormat queryDefaultFramebufferImageFormat() {
    constexpr GLuint defaultFramebufferHandle = 0u;

    GLint alphaBits = 0;
    glGetNamedFramebufferAttachmentParameteriv(defaultFramebufferHandle, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alphaBits);

    return ((alphaBits != 0) ? ImageFormat::RGBA8_UNORM : ImageFormat::RGB8_UNORM);
}

std::unique_ptr<GLProgram> createPresentationProgram() {
    const auto vertexShader = GLShader::create({ShaderType::VERTEX});
    NOX_ENSURE_RETURN_NULLPTR(vertexShader != nullptr);
    NOX_ENSURE_RETURN_NULLPTR(vertexShader->compile(vertexShaderSource));

    const auto fragmentShader = GLShader::create({ShaderType::FRAGMENT});
    NOX_ENSURE_RETURN_NULLPTR(fragmentShader != nullptr);
    NOX_ENSURE_RETURN_NULLPTR(fragmentShader->compile(fragmentShaderSource));

    auto program = std::make_unique<GLProgram>();
    program->attachShader(vertexShader->getHandle());
    program->attachShader(fragmentShader->getHandle());
    NOX_ENSURE_RETURN_NULLPTR(program->link());

    return program;
}

} // namespace

std::unique_ptr<GLSwapchain> GLSwapchain::create(const SwapchainDescriptor &descriptor,
                                                 std::unique_ptr<GLContext> context,
                                                 std::shared_ptr<GLVertexArrayRegistry> registry) {
    const auto *presentMode = std::get_if<OpenGLPresentMode>(&descriptor.presentMode);
    NOX_ENSURE_RETURN_NULLPTR(presentMode != nullptr);
    context->setSwapInterval(presentMode->vSync);

    Texture2DDescriptor textureDescriptor{};
    textureDescriptor.format = queryDefaultFramebufferImageFormat();
    textureDescriptor.size = descriptor.size;
    auto texture = std::make_unique<GLTexture2D>(textureDescriptor);

    auto program = createPresentationProgram();
    NOX_ENSURE_RETURN_NULLPTR(program != nullptr);

    auto swapchain = std::make_unique<GLSwapchain>(std::move(context),
                                                   std::move(program),
                                                   std::move(texture),
                                                   std::move(registry),
                                                   descriptor.size);

    return swapchain;
}

GLSwapchain::GLSwapchain(std::unique_ptr<GLContext> context,
                         std::unique_ptr<GLProgram> program,
                         std::unique_ptr<GLTexture2D> texture,
                         std::shared_ptr<GLVertexArrayRegistry> registry,
                         Vector2D<uint32_t> size)
    : m_context{std::move(context)},
      m_presentationProgram{std::move(program)},
      m_presentationTexture{std::move(texture)},
      m_vertexArrayRegistry{std::move(registry)},
      m_size{size} {
    m_vertexArray = m_vertexArrayRegistry->registerVertexArray({});
}

GLSwapchain::~GLSwapchain() {
    m_vertexArrayRegistry->unregisterVertexArray({});
}

Vector2D<uint32_t> GLSwapchain::getSize() const {
    return m_size;
}

ImageFormat GLSwapchain::getSurfaceFormat() const {
    return m_presentationTexture->getFormat();
}

std::vector<const Texture *> GLSwapchain::getSwapchainTextures() const {
    return {1u, m_presentationTexture.get()};
}

void GLSwapchain::present() const {
    m_context->makeCurrent();

    m_vertexArray->bind();
    m_presentationTexture->bind(0u);

    m_presentationProgram->bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_presentationProgram->unbind();

    m_context->swapBuffers();
}

} // namespace nox
